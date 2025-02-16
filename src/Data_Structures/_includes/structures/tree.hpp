#pragma once
#include "interfaces/base.hpp"
#include "interfaces/tree.hpp"
#include "utils.hpp"

namespace data_structures::tree {
    using namespace data_structures::base::tree;

    template <typename T>
    class LinkedBinaryTree : public BinaryTree<T> {
       public:
        LinkedBinaryTree(const T& rootVal) : BinaryTree<T>(rootVal), n{1} {}

        BinaryNodePtr<T> validate(TreeNodePtr<T> n) {
            if (!Utils::instanceof<BinaryTreeNode<T>>(n.get())) throw std::invalid_argument("Invalid node");
            return std::dynamic_pointer_cast<BinaryTreeNode<T>>(n);
        }

        [[nodiscard]] size_t size() const override { return n; }

        BinaryNodePtr<T> left(TreeNodePtr<T> n) override {
            auto node = validate(n);
            return node->left;
        }

        BinaryNodePtr<T> right(TreeNodePtr<T> n) override {
            auto node = validate(n);
            return node->right;
        }

        BinaryNodePtr<T> addLeft(TreeNodePtr<T> n, const T& val) {
            auto parent = validate(n);
            if (parent->left != nullptr) throw std::invalid_argument("Node already has left");
            parent->left = std::make_shared<BinaryTreeNode<T>>(val);
            parent->left->parent = parent;
            this->n++;
            return parent->left;
        }

        BinaryNodePtr<T> addRight(TreeNodePtr<T> n, const T& val) {
            auto parent = validate(n);
            if (parent->right != nullptr) throw std::invalid_argument("Node already has right");
            parent->right = std::make_shared<BinaryTreeNode<T>>(val);
            parent->right->parent = parent;
            this->n++;
            return parent->right;
        }

        size_t numChildren(TreeNodePtr<T> n) override {
            auto node = validate(n);
            size_t num{};
            if (left(node) != nullptr) num++;
            if (right(node) != nullptr) num++;
            return num;
        }

        T remove(TreeNodePtr<T> n) {
            if (this->isRoot(n)) {
                throw std::invalid_argument("Cannot remove root");
            }

            BinaryNodePtr<T> node = validate(n);
            auto res = node->data;
            auto child = node->left != nullptr ? node->left : node->right;
            if (child != nullptr) child->parent = node->parent.lock();
            if (child == this->r)
                this->r = child;
            else {
                auto parent = validate(node->parent.lock());
                if (node == parent->left)
                    parent->left = child;
                else
                    parent->right = child;
            }
            this->n--;
            node.reset();
            return res;
        }

        void preorderDisplay(TreeNodePtr<T> n, string& out, const string& pre, const string& childPre) {
            auto node = validate(n);
            out += pre;
            out += node->data;
            out += "\n";
            auto kinder = this->children(node);
            for (size_t i{}; i < kinder.size(); i++) {
                auto child = kinder.at(i);
                if (i < kinder.size() - 1)
                    preorderDisplay(child, out, childPre + "├── ", childPre + "│   ");
                else
                    preorderDisplay(child, out, childPre + "└── ", childPre + "    ");
            }
        }

        string preorderDisplay() {
            string out{};
            preorderDisplay(this->root(), out, "", "");
            return out;
        }

       private:
        size_t n{};
    };

    template <typename T>
    class LinkedBinarySearchTree : public BinarySearchTree<T> {
       public:
        LinkedBinarySearchTree(const T& rootVal) : BinarySearchTree<T>{rootVal} {}

        LinkedBinarySearchTree() : BinarySearchTree<T>{} {}

        [[nodiscard]] size_t size() const override { return n; }

        BinaryNodePtr<T> left(TreeNodePtr<T> n) override {
            auto node = validate(n);
            return node->left;
        }

        BinaryNodePtr<T> right(TreeNodePtr<T> n) override {
            auto node = validate(n);
            return node->right;
        }

        size_t numChildren(TreeNodePtr<T> n) override {
            auto node = validate(n);
            size_t num{};
            if (left(node) != nullptr) num++;
            if (right(node) != nullptr) num++;
            return num;
        }

        BinaryNodePtr<T> validate(TreeNodePtr<T> n) {
            if (!Utils::instanceof<BinaryTreeNode<T>>(n.get())) throw std::invalid_argument("Invalid node");
            return std::dynamic_pointer_cast<BinaryTreeNode<T>>(n);
        }

        void insert(T key) { return insert(std::make_shared<TreeNode<T>>(key)); }

        void insert(TreeNodePtr<T> n) override {
            this->r = insertRec(this->r, n);
            this->n++;
        }

        BinaryNodePtr<T> search(T key) override {
            auto temp = this->r;
            while (temp != nullptr) {
                if (key < temp->data)
                    temp = temp->left;
                else if (key > temp->data)
                    temp = temp->right;
                else
                    return temp;
            }
            return nullptr;
        }

        void preorderDisplay(TreeNodePtr<T> n, string& out, const string& pre, const string& childPre) {
            auto node = validate(n);
            out += pre;
            out += std::to_string(node->data);
            out += "\n";
            auto kinder = this->children(node);
            for (size_t i{}; i < kinder.size(); i++) {
                auto child = kinder.at(i);
                if (i < kinder.size() - 1)
                    preorderDisplay(child, out, childPre + "├── ", childPre + "│   ");
                else
                    preorderDisplay(child, out, childPre + "└── ", childPre + "    ");
            }
        }

        string preorderDisplay() {
            string out{};
            preorderDisplay(this->root(), out, "", "");
            return out;
        }

        std::optional<T> remove(T key) override {
            auto node = search(key);
            if (node == nullptr) return std::nullopt;

            T data = node->data;

            if (this->numChildren(node) == 0) {
                if (this->isRoot(node)) {
                    this->r.reset();
                    n--;
                    return data;
                }
                auto p = node->parent.lock();
                auto parent = validate(p);
                if (parent->left == node) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
                node.reset();
            } else {
                if (this->isRoot(node)) {
                    if (node->left != nullptr) {
                        this->r = node->left;
                    } else {
                        this->r = node->right;
                    }
                    n--;
                    return data;
                }
                auto p = node->parent.lock();
                auto parent = validate(p);
                if (parent->left == node) {
                    if (node->left != nullptr) {
                        parent->left = node->left;
                    } else {
                        parent->left = node->right;
                    }
                } else {
                    if (node->left != nullptr) {
                        parent->right = node->left;
                    } else {
                        parent->right = node->right;
                    }
                }
            }

            n--;
            return data;
        }

        T removeMax() override {
            n--;
            return T{};
        }

        T removeMin() override {
            n--;
            return T{};
        }

       private:
        BinaryNodePtr<T> insertRec(BinaryNodePtr<T> root, TreeNodePtr<T> n) {
            if (root == nullptr) {
                root = std::make_shared<BinaryTreeNode<T>>(n->data);
                root->parent = n->parent;
                return root;
            }

            if (n->data <= root->data) {
                root->left = insertRec(left(root), n);
            } else if (n->data > root->data) {
                root->right = insertRec(right(root), n);
            }

            return root;
        }

        size_t n{};
    };
}  // namespace data_structures::tree
