#pragma once
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

#include "base.hpp"
#include "interfaces/graph.hpp"
#include "utils.hpp"

namespace data_structures::graph::undirected {
    template <typename N, typename E>
    class EdgeListUGraph : public UGraph<N, E> {
       public:
        template <typename T>
        class ELUGEdge;

        template <typename T>
        class ELUGNode;

        template <typename T>
        using ELUGNodePtr = shared_ptr<ELUGNode<T>>;

        template <typename T>
        using ELUGEdgePtr = shared_ptr<ELUGEdge<T>>;

        ELUGNodePtr<N> validateNode(NodePtr<N> n) {
            if (!Utils::instanceof<ELUGNode<N>>(n.get())) throw std::invalid_argument("Invalid node");
            return std::dynamic_pointer_cast<ELUGNode<N>>(n);
        }

        ELUGEdgePtr<N> validateEdge(EdgePtr<E> e) {
            if (!Utils::instanceof<ELUGEdge<E>>(e.get())) throw std::invalid_argument("Invalid edge");
            return std::dynamic_pointer_cast<ELUGEdge<E>>(e);
        }

        friend class ELUGNode<N>;

        template <typename T>
        class ELUGEdge : public Edge<T> {
           public:
            ELUGEdge() : Edge<T>{} {}

            ELUGEdge(const T& data) : Edge<T>{data} {}

            const EdgeEndpoint<N> endNodes() const { return incidentNodes; }

            NodePtr<N> opposite(NodePtr<N> node) const {
                if (!isIncidentOn(node)) throw std::runtime_error("Not incident");
                return incidentNodes.at(0) == node ? incidentNodes.at(1) : incidentNodes.at(0);
            }

            bool isAdjacentTo(EdgePtr<T> e) const override {
                ELUGEdgePtr<T> edge = std::dynamic_pointer_cast<ELUGEdge<T>>(e);
                const auto& nodeList = edge->endNodes();
                NodeList<N> intersection{};
                rg::set_intersection(incidentNodes, nodeList, std::back_inserter(intersection));
                return !intersection.empty();
            }

            bool isIncidentOn(const NodePtr<N>& node) const {
                return !static_cast<bool>(node != incidentNodes.at(0) && node != incidentNodes.at(1));
            }

           private:
            friend class EdgeListUGraph;
            EdgeEndpoint<N> incidentNodes{};
        };

        template <typename T>
        class ELUGNode : public graph::Node<T>, public std::enable_shared_from_this<ELUGNode<T>> {
           public:
            ELUGNode(EdgeListUGraph* graph) : Node<T>{}, graph{graph} {}

            ELUGNode(const T& data, EdgeListUGraph* graph) : Node<T>{data}, graph{graph} {}

            const EdgeList<E> incidentEdges() const {
                EdgeList<E> res{};
                rg::copy_if(graph->edges(), std::back_inserter(res),
                            [self = this->shared_from_this()](const EdgePtr<E>& e) {
                                auto edge = self->graph->validateEdge(e);
                                return edge->isIncidentOn(
                                    std::const_pointer_cast<Node<T>>(std::dynamic_pointer_cast<const Node<N>>(self)));
                            });
                return res;
            }

            bool isAdjacentTo(NodePtr<T> node) const override {
                for (const auto& e : graph->edges()) try {
                        if (&(*graph->validateEdge(e)->opposite(node)) == &(*this)) return true;

                    } catch (const std::runtime_error& e) {
                        continue;
                    }
                return false;
            }

           private:
            EdgeListUGraph* graph{};
            friend class EdgeListUGraph<N, E>;
        };

        EdgeListUGraph() = default;

        EdgeListUGraph(const EdgeListUGraph& other) : edgeList{other.edgeList}, nodeList{other.nodeList} {}

        EdgeListUGraph(EdgeListUGraph&& other) noexcept
            : edgeList{std::move(other.edgeList)}, nodeList{std::move(other.nodeList)} {}

        EdgeListUGraph& operator=(EdgeListUGraph other) {
            swap(*this, other);
            return *this;
        }

        EdgeListUGraph& operator=(EdgeListUGraph&& other) noexcept {
            nodeList = std::move(other.nodeList);
            edgeList = std::move(other.edgeList);
            return *this;
        }

        NodeList<N> nodes() const override { return nodeList; }

        EdgeList<E> edges() const override { return edgeList; }

        NodePtr<N> addNode(const N& val) override {
            auto node = std::make_shared<ELUGNode<N>>(val, this);
            nodeList.push_back(node);
            return node;
        }

        EdgePtr<E> addEdge(const NodePtr<N>& v, const NodePtr<N>& w, const E& val) override {
            auto edge = std::make_shared<ELUGEdge<E>>(val);
            edge->incidentNodes.at(0) = v;
            edge->incidentNodes.at(1) = w;
            edgeList.push_back(edge);
            return edge;
        }

        void removeNode(const NodePtr<N>& v) override {
            auto node = validateNode(v);
            const auto& edges = node->incidentEdges();

            for (auto& e : node->incidentEdges()) {
                removeEdge(e);
            }

            auto it = rg::find(nodeList, node);
            nodeList.erase(it);
        }

        void removeEdge(const EdgePtr<E>& e) override {
            auto edge = validateEdge(e);
            auto it = rg::find(edgeList, edge);
            edgeList.erase(it);
        }

        [[nodiscard]] size_t numEdges() const { return edgeList.size(); }

        [[nodiscard]] size_t size() const override { return nodeList.size(); }

        [[nodiscard]] bool isEmpty() const override { return nodeList.empty(); }

        ~EdgeListUGraph() override {
            edgeList.clear();
            nodeList.clear();
        }

       private:
        void swap(EdgeListUGraph& first, EdgeListUGraph& second) {
            using std::swap;
            swap(first.nodeList, second.nodeList);
            swap(first.edgeList, second.edgeList);
        }

        NodeList<N> nodeList{};
        EdgeList<E> edgeList{};
    };
}  // namespace data_structures::graph::undirected

namespace data_structures::graph::directed {
    template <typename N, typename E>
    class EdgeListDGraph : public DGraph<N, E> {
       public:
        template <typename T>
        class ELDGEdge;

        template <typename T>
        class ELDGNode;

        template <typename T>
        using ELDGNodePtr = shared_ptr<ELDGNode<T>>;

        template <typename T>
        using ELDGEdgePtr = shared_ptr<ELDGEdge<T>>;

        ELDGNodePtr<N> validateNode(NodePtr<N> n) const {
            if (!Utils::instanceof<ELDGNode<N>>(n.get())) throw std::invalid_argument("Invalid node");
            return std::dynamic_pointer_cast<ELDGNode<N>>(n);
        }

        ELDGEdgePtr<N> validateEdge(EdgePtr<E> e) const {
            if (!Utils::instanceof<ELDGEdge<E>>(e.get())) throw std::invalid_argument("Invalid edge");
            return std::dynamic_pointer_cast<ELDGEdge<E>>(e);
        }

        friend class ELDGNode<N>;

        template <typename T>
        class ELDGEdge : public Edge<T> {
           public:
            ELDGEdge() : Edge<T>{} {}

            ELDGEdge(const T& data) : Edge<T>{data} {}

            const NodePtr<N> startNode() const { return from; }

            const NodePtr<N> endNode() const { return to; }

            using EdgeToFromPair = std::array<NodePtr<N>, 2>;

            EdgeToFromPair toFromPair() const { return {to, from}; }

            bool isAdjacentTo(EdgePtr<T> e) const override {
                ELDGEdgePtr<T> edge = std::dynamic_pointer_cast<ELDGEdge<T>>(e);
                const auto& eToFromPair = edge->toFromPair();
                NodeList<N> intersection{};
                rg::set_intersection(this->toFromPair(), eToFromPair, std::back_inserter(intersection));
                return !intersection.empty();
            }

            bool isIncidentOn(const NodePtr<N>& node) const { return !static_cast<bool>(node != from && node != to); }

           private:
            friend class EdgeListDGraph;
            NodePtr<N> from{};
            NodePtr<N> to{};
        };

        template <typename T>
        class ELDGNode : public graph::Node<T>, public std::enable_shared_from_this<ELDGNode<T>> {
           public:
            ELDGNode(EdgeListDGraph* graph) : Node<T>{}, graph{graph} {}

            ELDGNode(const T& data, EdgeListDGraph* graph) : Node<T>{data}, graph{graph} {}

            ELDGNode(const ELDGNode& other) : Node<T>{other.data}, graph{other.graph} {}

            ELDGNode(ELDGNode&& other) noexcept : Node<T>{std::move(other.data)}, graph{other.graph} {
                other.graph = nullptr;
            }

            const EdgeList<E> incidentEdges() const {
                EdgeList<E> res{};
                rg::copy_if(graph->edges(), std::back_inserter(res),
                            [self = this->shared_from_this()](const EdgePtr<E>& e) {
                                auto edge = self->graph->validateEdge(e);
                                return edge->isIncidentOn(
                                    std::const_pointer_cast<Node<T>>(std::dynamic_pointer_cast<const Node<N>>(self)));
                            });
                return res;
            }

            [[nodiscard]] size_t inDegree() const {
                size_t indeg{};
                for (const auto& e : graph->edges()) {
                    auto edge = graph->validateEdge(e);
                    if (edge->endNode().get() == this) indeg++;
                }
                return indeg;
            }

            [[nodiscard]] size_t outDegree() const {
                size_t outdeg{};
                for (const auto& e : graph->edges()) {
                    auto edge = graph->validateEdge(e);
                    if (edge->startNode().get() == this) outdeg++;
                }
                return outdeg;
            }

            bool isAdjacentTo(NodePtr<T> node) const override {
                for (const auto& e : graph->edges()) try {
                        if (auto edge = *graph->validateEdge(e);
                            &(*edge.startNode()) == &(*this) && &(*edge.endNode()) == &(*node))
                            return true;

                    } catch (const std::runtime_error& e) {
                        continue;
                    }
                return false;
            }

           private:
            EdgeListDGraph* graph{};
            friend class EdgeListDGraph<N, E>;
        };

        EdgeListDGraph() = default;

        EdgeListDGraph(const EdgeListDGraph& other) {
            std::unordered_map<NodePtr<N>, NodePtr<N>> nodeMap;

            for (const auto& n : other.nodeList) {
                auto node = addNode(**n);
                nodeMap[n] = node;
            }

            for (auto& e : other.edgeList) {
                auto edge = other.validateEdge(e);
                auto start = nodeMap[edge->startNode()];
                auto end = nodeMap[edge->endNode()];
                addEdge(start, end, **e);
            }
        }

        EdgeListDGraph(EdgeListDGraph&& other) noexcept
            : edgeList{std::move(other.edgeList)}, nodeList{std::move(other.nodeList)} {}

        EdgeListDGraph& operator=(EdgeListDGraph other) {
            std::unordered_map<NodePtr<N>, NodePtr<N>> nodeMap;

            for (const auto& n : other.nodeList) {
                auto node = addNode(**n);
                nodeMap[n] = node;
            }

            for (auto& e : other.edgeList) {
                auto edge = other.validateEdge(e);
                auto start = nodeMap[edge->startNode()];
                auto end = nodeMap[edge->endNode()];
                addEdge(start, end, **e);
            }
            return *this;
        }

        EdgeListDGraph& operator=(EdgeListDGraph&& other) noexcept {
            nodeList = std::move(other.nodeList);
            edgeList = std::move(other.edgeList);
            return *this;
        }

        NodeList<N> toposort() {
            NodeList<N> res{};
            res.reserve(size());
            auto topograph{*this};

            data_structures::base::LinkedQueue<NodePtr<N>> zeroInDegQ{};
            for (const auto& n : topograph.nodes()) {
                auto node = topograph.validateNode(n);
                if (node->inDegree() == 0) {
                    zeroInDegQ.enqueue(n);
                }
            }

            while (!zeroInDegQ.isEmpty()) {
                auto n = zeroInDegQ.dequeue();
                auto node = topograph.validateNode(n);
                res.push_back(n);

                for (const auto& e : node->incidentEdges()) {
                    auto edge = topograph.validateEdge(e);
                    auto endN = edge->endNode();
                    auto endNode = topograph.validateNode(endN);
                    topograph.removeEdge(e);
                    if (endNode->inDegree() == 0) {
                        zeroInDegQ.enqueue(endN);
                    }
                }

                topograph.removeNode(n);
            }

            return res;
        }

        NodeList<N> nodes() const override { return nodeList; }

        EdgeList<E> edges() const override { return edgeList; }

        NodePtr<N> addNode(const N& val) override {
            auto node = std::make_shared<ELDGNode<N>>(val, this);
            nodeList.push_back(node);
            return node;
        }

        EdgePtr<E> addEdge(const NodePtr<N>& from, const NodePtr<N>& to, const E& val) override {
            auto edge = std::make_shared<ELDGEdge<E>>(val);
            edge->from = from;
            edge->to = to;
            edgeList.push_back(edge);
            return edge;
        }

        void removeNode(const NodePtr<N>& v) override {
            auto node = validateNode(v);
            const auto& edges = node->incidentEdges();

            for (auto& e : node->incidentEdges()) {
                removeEdge(e);
            }

            auto it = rg::find(nodeList, node);
            nodeList.erase(it);
        }

        void removeEdge(const EdgePtr<E>& e) override {
            auto edge = validateEdge(e);
            auto it = rg::find(edgeList, edge);
            edgeList.erase(it);
        }

        [[nodiscard]] size_t numEdges() const { return edgeList.size(); }

        [[nodiscard]] size_t size() const override { return nodeList.size(); }

        [[nodiscard]] bool isEmpty() const override { return nodeList.empty(); }

        ~EdgeListDGraph() override {
            edgeList.clear();
            nodeList.clear();
        }

       private:
        NodeList<N> nodeList{};
        EdgeList<E> edgeList{};
    };
}  // namespace data_structures::graph::directed
