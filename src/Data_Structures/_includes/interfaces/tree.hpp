#pragma once
#include "base.hpp"
namespace data_structures::base::tree {
    template <typename T>
    struct TreeNode {
        T data{};
        std::weak_ptr<TreeNode<T>> parent{};

        TreeNode(const T& data) : data{data} {}

        virtual bool operator==(TreeNode other) const { return this == &other; }
        virtual ~TreeNode() = default;

        virtual operator std::string() { return std::format("({})", data); }
    };

    template <typename T>
    using TreeNodePtr = shared_ptr<TreeNode<T>>;

    template <typename T>
    struct BinaryTreeNode : public TreeNode<T> {
        shared_ptr<BinaryTreeNode<T>> left{};
        shared_ptr<BinaryTreeNode<T>> right{};
        BinaryTreeNode(const T& data) : TreeNode<T>(data) {}
    };

    template <typename T>
    using BinaryNodePtr = shared_ptr<BinaryTreeNode<T>>;
}  // namespace data_structures::base::tree

namespace data_structures::tree {
    using namespace base::tree;
    template <typename T>
    struct Tree : Sized {
       protected:
        virtual std::vector<TreeNodePtr<T>> preorderSubtree(TreeNodePtr<T> n, std::vector<TreeNodePtr<T>>& snap) {
            snap.push_back(n);
            for (const auto& child : children(n)) {
                preorderSubtree(child, snap);
            }
            return snap;
        }

        virtual std::vector<TreeNodePtr<T>> postorderSubtree(TreeNodePtr<T> n, std::vector<TreeNodePtr<T>>& snap) {
            for (const auto& child : children(n)) {
                postorderSubtree(child, snap);
            }
            snap.push_back(n);
            return snap;
        }

       public:
        virtual TreeNodePtr<T> root() = 0;
        virtual std::vector<TreeNodePtr<T>> children(TreeNodePtr<T> n) = 0;
        [[nodiscard]] bool isEmpty() const override { return size() == 0; }
        virtual bool isRoot(TreeNodePtr<T> n) { return root() == n; }
        [[nodiscard]] virtual size_t numChildren(TreeNodePtr<T> n) = 0;
        virtual ~Tree() = default;

        virtual std::vector<TreeNodePtr<T>> preorder() {
            std::vector<TreeNodePtr<T>> snap{};
            if (!isEmpty()) preorderSubtree(root(), snap);
            return snap;
        }

        virtual std::vector<TreeNodePtr<T>> postorder() {
            std::vector<TreeNodePtr<T>> snap{};
            if (!isEmpty()) postorderSubtree(root(), snap);
            return snap;
        }

        size_t depth(TreeNodePtr<T> n) {
            if (isRoot(n)) return 0;
            return 1 + depth(n->parent.lock());
        }

        size_t height(TreeNodePtr<T> n) {
            size_t h{};
            for (const auto& child : children(n)) h = std::max(h, height(child) + 1);
            return h;
        }
    };

    template <typename T>
    class BinaryTree : public Tree<T> {
       protected:
        virtual std::vector<TreeNodePtr<T>> inorderSubtree(TreeNodePtr<T> n, std::vector<TreeNodePtr<T>>& snap) {
            if (!n) return snap;

            auto left = this->left(n);
            inorderSubtree(left, snap);

            snap.push_back(n);

            auto right = this->right(n);
            inorderSubtree(right, snap);

            return snap;
        }

        BinaryNodePtr<T> r;

       public:
        std::vector<TreeNodePtr<T>> children(TreeNodePtr<T> n) override {
            auto res = std::vector<TreeNodePtr<T>>{};
            if (left(n) != nullptr) res.push_back(left(n));
            if (right(n) != nullptr) res.push_back(right(n));
            return res;
        }

        TreeNodePtr<T> root() override { return r; }

        virtual BinaryNodePtr<T> left(TreeNodePtr<T> n) = 0;
        virtual BinaryNodePtr<T> right(TreeNodePtr<T> n) = 0;

        BinaryTree(T root) : r{new BinaryTreeNode<T>{root}} {}

        virtual ~BinaryTree() { r.reset(); }

        virtual std::vector<TreeNodePtr<T>> inorder() {
            std::vector<TreeNodePtr<T>> snap{};
            if (!this->isEmpty()) inorderSubtree(root(), snap);
            return snap;
        }
    };

    template <typename T>
    class BinarySearchTree : public BinaryTree<T> {
       public:
        BinarySearchTree(T root) : BinaryTree<T>{root} {}
        virtual void insert(TreeNodePtr<T> n) = 0;
        virtual BinaryNodePtr<T> search(T key) = 0;
        virtual std::optional<T> remove(T key) = 0;
        virtual T removeMax() = 0;
        virtual T removeMin() = 0;
    };

}  // namespace data_structures::tree
