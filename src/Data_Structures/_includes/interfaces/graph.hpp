#pragma once
#include <functional>
#include <set>
#include <vector>

#include "base.hpp"
#include "structures/base.hpp"

namespace data_structures::graph {
    template <typename T>
    struct Edge;

    template <typename T>
    using EdgePtr = shared_ptr<Edge<T>>;

    template <typename T>
    struct Edge {
        Edge() = default;
        Edge(const T& data) : data{data} {}
        virtual bool isAdjacentTo(EdgePtr<T> node) const = 0;
        T& operator*() { return data; }
        virtual ~Edge() = default;

       protected:
        T data{};
    };

    template <typename T>
    struct Node;

    template <typename T>
    using NodePtr = shared_ptr<Node<T>>;

    template <typename T>
    struct Node {
        Node() = default;
        Node(const T& data) : data{data} {}
        virtual bool isAdjacentTo(NodePtr<T> node) const = 0;
        T& operator*() { return data; }
        virtual ~Node() = default;

       protected:
        T data{};
    };

    template <typename T>
    using EdgeList = std::vector<EdgePtr<T>>;

    template <typename T>
    using NodeList = std::vector<NodePtr<T>>;

    template <typename T>
    using EdgeEndpoint = std::array<NodePtr<T>, 2>;

    template <typename N>
    using VisitFunction = std::function<void(NodePtr<N>&)>;

    template <typename N>
    using NodeSet = std::set<NodePtr<N>>;

    template <typename N, typename E>
    struct Graph : public Sized {
       public:
        virtual NodeList<N> nodes() const = 0;
        virtual EdgeList<E> edges() const = 0;
        virtual NodePtr<N> addNode(const N& val) = 0;
        virtual EdgePtr<E> addEdge(const NodePtr<N>& v, const NodePtr<N>& w, const E& val) = 0;
        virtual void removeNode(const NodePtr<N>& v) = 0;
        virtual void removeEdge(const EdgePtr<E>& e) = 0;
        virtual ~Graph() = default;

        void dfs(VisitFunction<N> visit, NodeSet<N>& visited, NodePtr<N> n) {
            visit(n);
            visited.insert(n);
            for (auto& v : nodes())
                if (n->isAdjacentTo(v) && !visited.contains(v)) dfs(visit, visited, v);
        }

        void dfs(VisitFunction<N> visit) {
            NodeSet<N> visited;
            for (auto& n : nodes()) {
                if (!visited.contains(n)) {
                    dfs(visit, visited, n);
                }
            }
        }

        void dfs(VisitFunction<N> visit, NodePtr<N> startingNode) {
            NodeSet<N> visited;
            dfs(visit, visited, startingNode);
        }

        void dfsStack(std::function<void(NodePtr<N>&)> visit) {
            NodeSet<N> visited;
            data_structures::base::LinkedStack<NodePtr<N>> S{};
            const auto& V{nodes()};
            for (auto& v : V) {
                if (!visited.contains(v)) {
                    S.push(v);
                    while (!S.isEmpty()) {
                        auto w = S.pop();
                        if (!visited.contains(w)) {
                            visit(w);
                            visited.insert(w);
                            for (auto& u : V)
                                if (w->isAdjacentTo(u) && !visited.contains(u)) S.push(u);
                        }
                    }
                }
            }
        }

        void bfs(VisitFunction<N> visit) {
            NodeSet<N> visited;
            data_structures::base::LinkedQueue<NodePtr<N>> Q{};
            const auto& V{nodes()};
            for (auto& v : V) {
                if (!visited.contains(v)) {
                    Q.enqueue(v);
                    while (!Q.isEmpty()) {
                        auto w = Q.dequeue();
                        if (!visited.contains(w)) {
                            visit(w);
                            visited.insert(w);
                            for (auto& u : V)
                                if (w->isAdjacentTo(u) && !visited.contains(u)) Q.enqueue(u);
                        }
                    }
                }
            }
        }
    };
}  // namespace data_structures::graph

namespace data_structures::graph::undirected {

    template <typename N, typename E>
    struct UGraph : public Graph<N, E> {
       public:
        virtual ~UGraph() = default;
    };
}  // namespace data_structures::graph::undirected

namespace data_structures::graph::directed {

    template <typename N, typename E>
    struct DGraph : public Graph<N, E> {
        virtual ~DGraph() = default;
    };
}  // namespace data_structures::graph::directed
