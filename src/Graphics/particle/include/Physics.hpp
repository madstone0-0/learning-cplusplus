#pragma once

#include <algorithm>
#include <csignal>
#include <mlinalg/MLinalg.hpp>
#include <vector>

#include "Particle.hpp"

using namespace mlinalg;
using ParticleGroups = std::vector<std::vector<PartPtr>>;
using Partices = std::vector<PartPtr>;

enum class CollisionDetection { SWEEP_AND_PRUNE = 0, UNI_SPACE_PART, KD_TREE, BVH_TREE };

inline std::string parseCollisionMethods(CollisionDetection method) {
    switch (method) {
        case CollisionDetection::SWEEP_AND_PRUNE:
            return "Sweep and Prune";
        case CollisionDetection::UNI_SPACE_PART:
            return "Uniform Space Partitioning";
        case CollisionDetection::KD_TREE:
            return "KD Tree";
        case CollisionDetection::BVH_TREE:
            return "Bounding Volume Hierarchy";
    }
}

struct Physics {
    Physics(double dt, Partices& particles) : dt{dt}, particles{particles} {}

    [[nodiscard]] virtual Vector2<double> collisionResponse(const Vector2<double>& v,
                                                            const Vector2<double>& n) const = 0;
    virtual void handleBoxCollision(const PartPtr& particle) = 0;
    virtual void handleParticleCollision() = 0;

    virtual ~Physics() = default;

    void setWinDim(float borderXMin, float borderXMax, float borderYMin, float borderYMax) {
        this->borderXMin = borderXMin;
        this->borderXMax = borderXMax;
        this->borderYMin = borderYMin;
        this->borderYMax = borderYMax;
    }

    CollisionDetection collisionMethod{CollisionDetection::UNI_SPACE_PART};
    float borderXMin;
    float borderYMin;
    float borderXMax;
    float borderYMax;
    Vector2<double> nL{1, 0};       // Normal vector for left wall
    Vector2<double> nR{-1, 0};      // Normal vector for right wall
    Vector2<double> nB{0, -1};      // Normal vector for bottom wall
    Vector2<double> nT{0, 1};       // Normal vector for top wall
    double e{0.5};                  // Elasticity
    Vector2<double> gravity{0, 0};  // Global gravity
    double frictionCoef{0.3};
    double dt{};  // Time step
    Partices& particles;
};

struct CollisionHandler {
   private:
    double& frictionCoef;
    static constexpr double EPSILON{1e-5};
    const Vector2<double> EPSILON_VEC{EPSILON * Vector2<double>{1, 1}};

    struct CollisionData {
        Vector2<double> normal;
        double depth;
    };

    struct ParticleData {
        Vector2<double> pos;
        Vector2<double> vel;
        double radius;
        double mass;
    };

    Vector2<double> calculatePositionCorrection(const Vector2<double>& normal, double overlap,
                                                double biasFactor = 0.2) {
        return normal * (overlap * biasFactor);
    }

    [[nodiscard]] bool isColliding(const ParticleData& p1, const ParticleData& p2) const {
        double distance = (p1.pos - p2.pos).length();
        double radiusSum = p1.radius + p2.radius;
        return distance <= radiusSum + EPSILON;
    }

    [[nodiscard]] auto calculateNormal(const Vector2<double>& pos1, const Vector2<double>& pos2) const {
        auto posDiff = pos2 - pos1;
        return posDiff / (posDiff.length() + EPSILON);
    }

    [[nodiscard]] auto calculateDepth(const Vector2<double>& pos, const Vector2<double>& otherPos, double radius,
                                      double otherRadius) const {
        return 2 * otherRadius - (otherPos - pos).length();
    }

    ParticleData createParticleData(const PartPtr& particle) const {
        return {particle->gPos(), particle->gVel(), particle->gRadius(), particle->gMass() * 1.0};
    }

    [[nodiscard]] CollisionData calculateCollisionData(const ParticleData& particle,
                                                       const ParticleData& otherParticle) {
        auto normal = calculateNormal(particle.pos, otherParticle.pos);
        auto depth = calculateDepth(particle.pos, otherParticle.pos, particle.radius, otherParticle.radius);
        return {normal, depth};
    }

    ParticleData calculateNewParticleData(const ParticleData& particle, const ParticleData& otherParticle,
                                          const CollisionData& colData) {
        const auto [normal, depth] = colData;
        const auto& [pos, vel, radius, mass] = particle;
        const auto& [otherPos, otherVel, otherRadius, otherMass] = otherParticle;

        auto massComponent = (2 * otherMass) / ((mass + otherMass) + EPSILON);
        auto centerDiff = pos - otherPos;
        auto vCInner = (vel - otherVel) * centerDiff;
        auto centerDiffNorm = centerDiff.length();
        auto newVel = vel - static_cast<double>(massComponent) *
                                (vCInner / ((centerDiffNorm * centerDiffNorm) + EPSILON)).at(0) * centerDiff;
        auto newPos = pos - normal * depth / 2;
        // auto newPos = pos - calculatePositionCorrection(normal, depth / 2);
        return {newPos, newVel, radius, mass};
    }

    ParticleData applyFriction(const ParticleData& particle, const ParticleData& otherParticle,
                               const CollisionData& colData) {
        const auto [normal, depth] = colData;
        const auto& [pos, vel, radius, mass] = particle;

        const auto& [otherPos, otherVel, otherRadius, otherMass] = otherParticle;

        auto velDiff = vel - otherVel;
        auto normComp = (normal * (velDiff)).at(0) * normal;
        auto tanComp = vel - normComp;
        auto tanVelWF = tanComp * (1 - frictionCoef);
        auto newVelWF = normComp + tanVelWF;

        return {pos, newVelWF, radius, mass};
    }

    void applyCollision(const PartPtr& particle, const ParticleData& newParticleData) {
        particle->sVel(newParticleData.vel);
        particle->sPos(newParticleData.pos);
    }

   public:
    CollisionHandler(double& frictionCoef) : frictionCoef{frictionCoef} {}

    void handleCollision(const PartPtr& particle, const PartPtr& otherParticle) {
        // Create particle data
        auto particleData = createParticleData(particle);
        auto otherParticleData = createParticleData(otherParticle);

        // Check if particles are colliding
        if (!isColliding(particleData, otherParticleData)) return;

        // Calculate collision data
        auto collisionData = calculateCollisionData(particleData, otherParticleData);
        auto otherCollisionData = calculateCollisionData(otherParticleData, particleData);

        // Calculate new particle data
        auto pData = calculateNewParticleData(particleData, otherParticleData, collisionData);
        auto otherPData = calculateNewParticleData(otherParticleData, particleData, otherCollisionData);

        // Apply friction
        auto pDataWF = applyFriction(pData, otherParticleData, collisionData);
        auto otherPDataWF = applyFriction(otherPData, particleData, otherCollisionData);

        // Apply collision to particles
        applyCollision(particle, pDataWF);
        applyCollision(otherParticle, otherPDataWF);
    }
};

struct SimplePhysics : public Physics {
    Vector2<int>& dim;
    CollisionHandler collisionHandler{frictionCoef};

    SimplePhysics(double dt, Partices& particles, Vector2<int>& dim, float borderXMin = 0, float borderXMax = 0,
                  float borderYMin = 0, float borderYMax = 0)
        : Physics{dt, particles}, dim{dim} {
        setWinDim(borderXMin, borderXMax, borderYMin, borderYMax);
    }

    ~SimplePhysics() override = default;

    [[nodiscard]] Vector2<double> collisionResponse(const Vector2<double>& v, const Vector2<double>& n) const override {
        // velocity after collision = velocity - (1 + elasticity) * normal * (velocity dot normal)
        // This inverts the velocity component that is perpendicular to the normal
        // Taking into account friction velocity after collision is now
        // velocity - (1 + elasticity) * normal velocity - friction coefficient * tangential velocity * time step
        // Where normal velocity = normal * (velocity * normal) and
        // tangential velocity = velocity - normal velocity
        auto vn = n * (v * n).at(0);
        auto vt = v - vn;
        return v - (1 + e) * vn - frictionCoef * vt * dt;
    }

    void handleBoxCollision(const PartPtr& particle) override {
        const auto& pos = particle->gPos();
        const auto& vel = particle->gVel();
        const auto& radius = particle->gRadius();

        // Left wall collision
        if (pos.at(0) - radius <= borderXMin) {
            if ((vel * nL).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionResponse(vel, nL);
                particle->sVel(newVel);

                auto depth = -(pos.at(0) - radius);
                particle->sPos(pos + nL * depth);
            }
        }

        // Right wall collision
        if (pos.at(0) + radius >= borderXMax) {
            if ((vel * nR).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionResponse(vel, nR);
                particle->sVel(newVel);

                auto depth = (pos.at(0) - radius) - borderXMax + radius * 2;
                particle->sPos(pos + nR * depth);
            }
        }

        // Bottom wall collision
        if (pos.at(1) + radius >= borderYMax) {
            if ((vel * nB).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionResponse(vel, nB);
                particle->sVel(newVel);

                auto depth = (pos.at(1) - radius) - borderYMax + radius * 2;
                particle->sPos(pos + nB * depth);
            }
        }

        // Top wall collision
        if (pos.at(1) - radius <= borderYMin) {
            if ((vel * nT).at(0) < 0) {  // Check if the object is moving towards the wall
                auto newVel = collisionResponse(vel, nT);
                particle->sVel(newVel);

                auto depth = -(pos.at(1) - radius);
                particle->sPos(pos + nT * depth);
            }
        }
    }

    void handleParticleCollision() override {
        ParticleGroups particleGroups;
        switch (collisionMethod) {
            case CollisionDetection::SWEEP_AND_PRUNE:
                particleGroups = sweepAndPrune();
                break;
            case CollisionDetection::UNI_SPACE_PART:
                particleGroups = uniSpacePartitioning();
                break;
            case CollisionDetection::KD_TREE:
                particleGroups = kdTree();
                break;
            case CollisionDetection::BVH_TREE:
                particleGroups = bvhTree();
                break;
        }

        for (const auto& particles : particleGroups) {
            handleParticleCollision(particles.at(0), particles);
        }
    }

    void handleParticleCollision(const PartPtr& particle, const std::vector<PartPtr>& particles) {
        for (const auto& otherParticle : particles) {
            if (otherParticle == particle) continue;
            collisionHandler.handleCollision(particle, otherParticle);
        }
    }

    // Broad-phase collision detection methods
    ParticleGroups sweepAndPrune(bool xAxis = true) {
        std::vector<std::vector<PartPtr>> res;
        std::sort(particles.begin(), particles.end(), [&xAxis](const PartPtr& a, const PartPtr& b) {
            if (xAxis) {
                return a->gPos().at(0) < b->gPos().at(0);
            } else {
                return a->gPos().at(1) < b->gPos().at(1);
            }
        });

        std::vector<PartPtr> activeIntervals;
        for (const auto& particle : particles) {
            const auto& pos = particle->gPos();
            const auto& radius = particle->gRadius();
            activeIntervals.erase(
                std::remove_if(activeIntervals.begin(), activeIntervals.end(),
                               [&](const PartPtr& p) {
                                   const auto& otherPos = p->gPos();
                                   const auto& otherRadius = p->gRadius();
                                   int axisIndex = xAxis ? 0 : 1;
                                   return (pos.at(axisIndex) - radius > otherPos.at(axisIndex) + otherRadius);
                               }),
                activeIntervals.end());
            activeIntervals.push_back(particle);
            if (activeIntervals.size() > 1) res.push_back(activeIntervals);
        }

        return res;
    }

    ParticleGroups uniSpacePartitioning(size_t gridSpace = 10) {
        // Make grid from screen
        vector<vector<PartPtr>> res;
        using std::vector;
        auto rows = (size_t)std::ceil(dim.at(1) / (double)gridSpace);
        auto cols = (size_t)std::ceil(dim.at(0) / (double)gridSpace);
        // cout << std::format("Rows: {}, Cols: {}\n", rows, cols);
        vector<vector<vector<PartPtr>>> grid(rows, vector<vector<PartPtr>>(cols, vector<PartPtr>{}));

        // Assign particles to cells
        for (const auto& particle : particles) {
            const auto& pos = particle->gPos();
            auto row = std::min((size_t)pos.at(1) / gridSpace, rows - 1);
            auto col = std::min((size_t)pos.at(0) / gridSpace, cols - 1);
            grid[row][col].push_back(particle);
        }

        // Only particles in the same cells should be checked for collisions
        res.reserve(rows * cols);
        for (size_t i{}; i < rows; i++) {
            for (size_t j{}; j < cols; j++) {
                if (grid[i][j].size() > 1) res.push_back(grid[i][j]);
            }
        }
        return res;
    }

    void buildKdTree(ParticleGroups& groups, std::vector<PartPtr>& particles, bool xAxis) {
        constexpr size_t baseThreshold = 20;
        size_t threshold = std::max(baseThreshold, particles.size() / 10);

        if (particles.size() <= threshold) {
            if (particles.size() > 1) groups.push_back(particles);
            return;
        }

        // Find the median index
        size_t median = particles.size() / 2;

        std::nth_element(particles.begin(), particles.begin() + median, particles.end(),
                         [xAxis](const PartPtr& a, const PartPtr& b) {
                             return (xAxis ? a->gPos().at(0) : a->gPos().at(1)) <
                                    (xAxis ? b->gPos().at(0) : b->gPos().at(1));
                         });

        // Split into left and right subgroups
        std::vector<PartPtr> left(particles.begin(), particles.begin() + median);
        std::vector<PartPtr> right(particles.begin() + median + 1, particles.end());

        // Recurse on left and right groups along the next axis
        buildKdTree(groups, left, !xAxis);
        buildKdTree(groups, right, !xAxis);
    }

    ParticleGroups kdTree(bool xAxis = false) {
        std::vector<std::vector<PartPtr>> res;
        buildKdTree(res, particles, false);
        return res;
    }

    void buildBVH(ParticleGroups& groups, std::vector<PartPtr>& particles, size_t start, size_t end) {
        constexpr size_t baseThreshold = 20;
        size_t particleCount = end - start;

        // Threshold for creating leaf nodes
        size_t threshold = std::max(baseThreshold, particleCount / 10);
        if (particleCount <= threshold) {
            if (particleCount > 1) {
                groups.emplace_back(particles.begin() + start, particles.begin() + end);
            }
            return;
        }

        // Compute bounding box
        double xMin = std::numeric_limits<double>::max();
        double xMax = std::numeric_limits<double>::lowest();
        double yMin = std::numeric_limits<double>::max();
        double yMax = std::numeric_limits<double>::lowest();

        for (size_t i = start; i < end; ++i) {
            const auto& pos = particles[i]->gPos();
            xMin = std::min(xMin, pos.at(0));
            xMax = std::max(xMax, pos.at(0));
            yMin = std::min(yMin, pos.at(1));
            yMax = std::max(yMax, pos.at(1));
        }

        // Decide the splitting axis based on bounding box dimensions
        bool splitAxis = (xMax - xMin) > (yMax - yMin);

        // Find the median index for splitting
        size_t median = start + particleCount / 2;
        std::nth_element(particles.begin() + start, particles.begin() + median, particles.begin() + end,
                         [splitAxis](const PartPtr& a, const PartPtr& b) {
                             return splitAxis ? a->gPos().at(0) < b->gPos().at(0) : a->gPos().at(1) < b->gPos().at(1);
                         });

        // Recurse on left and right subgroups
        buildBVH(groups, particles, start, median);
        buildBVH(groups, particles, median + 1, end);
    }

    ParticleGroups bvhTree() {
        ParticleGroups groups;
        if (!particles.empty()) {
            buildBVH(groups, particles, 0, particles.size());
        }
        return groups;
    }
};

using PhysicsType = unique_ptr<Physics>;
