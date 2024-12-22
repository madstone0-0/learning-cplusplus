#pragma once

#include <csignal>
#include <mlinalg/MLinalg.hpp>
#include <vector>

#include "Particle.hpp"

using namespace mlinalg;
using ParticleGroups = std::vector<std::vector<PartPtr>>;
using Partices = std::vector<PartPtr>;

enum class CollisionDetection { SWEEP_AND_PRUNE = 0, UNI_SPACE_PART, KD_TREE };

struct Physics {
    Physics(double dt, Partices& particles) : dt{dt}, particles{particles} {}

    [[nodiscard]] virtual Vector2<double> collisionResponse(const Vector2<double>& v,
                                                            const Vector2<double>& n) const = 0;
    virtual void handleBoxCollision(const PartPtr& particle) = 0;
    virtual void handleParticleCollision() = 0;

    virtual ~Physics() = 0;

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

struct SimplePhysics : public Physics {
    float borderXMin;
    float borderYMin;
    float borderXMax;
    float borderYMax;
    Vector2<int>& dim;
    CollisionDetection collisionMethod{CollisionDetection::KD_TREE};

    SimplePhysics(double dt, Partices& particles, Vector2<int>& dim, float borderXMin = 0, float borderXMax = 0,
                  float borderYMin = 0, float borderYMax = 0)
        : Physics{dt, particles}, dim{dim} {
        setWinDim(borderXMin, borderXMax, borderYMin, borderYMax);
    }

    void setWinDim(float borderXMin, float borderXMax, float borderYMin, float borderYMax) {
        this->borderXMin = borderXMin;
        this->borderXMax = borderXMax;
        this->borderYMin = borderYMin;
        this->borderYMax = borderYMax;
    }

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
        }

        for (const auto& particles : particleGroups) {
            handleParticleCollision(particles.at(0), particles);
        }
    }

    void handleParticleCollision(const PartPtr& particle, const std::vector<PartPtr>& particles) {
        for (const auto& otherParticle : particles) {
            if (otherParticle == particle) continue;
            const auto epsilon{1e-5};
            const auto epsilonVector{epsilon * Vector2<double>{1, 1}};

            const auto& pos = particle->gPos();
            const auto& otherPos = otherParticle->gPos();
            const auto& radius = particle->gRadius();
            const auto& otherRadius = otherParticle->gRadius();

            const auto& vel = particle->gVel();
            const auto& otherVel = otherParticle->gVel();
            const auto& mass = particle->gMass();
            const auto& otherMass = otherParticle->gMass();

            if (pos.dist(otherPos) <= radius + otherRadius) {
                // Collision
                // auto posDiff1{otherPos - pos};
                // auto posDiff2{pos - otherPos};
                // auto n1 = posDiff1 / posDiff1.length();
                // auto n2 = posDiff2 / posDiff2.length();
                // auto dV = otherVel - vel;
                // auto vn1 = n1 * (dV * n1).at(0);
                // auto vn2 = n2 * (dV * n2).at(0);
                // auto vt1 = vel - vn1;
                // auto vt2 = otherVel - vn2;
                // auto d1 = 2 * radius - posDiff1.length();
                // auto d2 = 2 * otherRadius - posDiff2.length();
                //
                // auto newVel = vel + vn1 * (1 + e) / 2 + frictionCoef * vt1 * dt / 2;
                // auto p1 = pos - n1 * d1 / 2;
                //
                // auto newVelOther = otherVel - vn2 * (1 + e) / 2 - frictionCoef * vt2 * dt / 2;
                // auto p2 = otherPos - n2 * d2 / 2;
                //
                // particle->sVel(newVel);
                // particle->sPos(p1);
                // otherParticle->sVel(newVelOther);
                // otherParticle->sPos(p2);

                auto posDiff1{otherPos - pos};
                auto posDiff2{pos - otherPos};
                auto n1 = posDiff1 / (posDiff1.length() + epsilon);
                auto n2 = posDiff2 / (posDiff2.length() + epsilon);
                auto d1 = 2 * radius - posDiff1.length();
                auto d2 = 2 * otherRadius - posDiff2.length();

                auto massComponent = (2 * otherMass) / ((mass + otherMass) + epsilon);
                auto centerDiff = pos - otherPos;
                auto vCInner = (vel - otherVel) * centerDiff;
                auto centerDiffNorm = centerDiff.length();
                auto newVel = vel - static_cast<double>(massComponent) *
                                        (vCInner / ((centerDiffNorm * centerDiffNorm) + epsilon)).at(0) * centerDiff;
                auto p1 = pos - n1 * d1 / 2;

                auto massComponentOther = (2 * mass) / ((mass + otherMass) + epsilon);
                auto centerDiffOther = otherPos - pos;
                auto vCInnerOther = (otherVel - vel) * centerDiffOther;
                auto centerDiffNormOther = centerDiffOther.length();
                auto newVelOther =
                    otherVel - static_cast<double>(massComponentOther) *
                                   (vCInnerOther / ((centerDiffNormOther * centerDiffNormOther) + epsilon)).at(0) *
                                   centerDiffOther;
                auto p2 = otherPos - n2 * d2 / 2;

                auto velDiff1 = newVel - newVelOther;
                auto normComp1 = (n1 * (velDiff1)).at(0) * n1;
                auto tanComp1 = vel - normComp1;
                auto tanVelWF1 = tanComp1 * (1 - frictionCoef);
                auto newVelWF = normComp1 + tanVelWF1;

                auto velDiff2 = newVelOther - newVel;
                auto normComp2 = (n2 * (velDiff2)).at(0) * n2;
                auto tanComp2 = vel - normComp2;
                auto tanVelWF2 = tanComp2 * (1 - frictionCoef);
                auto newVelOtherWF = normComp2 + tanVelWF2;

                // particle->sVel(newVel);
                particle->sVel(newVelWF);
                particle->sPos(p1);
                otherParticle->sVel(newVelOtherWF);
                // otherParticle->sVel(newVelOther);
                otherParticle->sPos(p2);
            }
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
        if (particles.size() <= 20) {
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
};
