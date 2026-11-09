#ifndef BINARYSEARCHTREE_HPP
#define BINARYSEARCHTREE_HPP 

#include <memory>
#include <cstddef>
#include <string>
#include <sstream>
#include <stdexcept>
#include <utility>

#include "DynamicArray.hpp"

template <typename T>
class BinarySearchTree {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;

        Node *threadNext = nullptr;

        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;
    size_t size = 0;

    Node *threadHead = nullptr;
    struct ThreadContext {
        Node* prev = nullptr;
        Node* head = nullptr;
    };

    int Compare(const T& a, const T& b) const {
        if (a < b) {
            return -1;
        }
        if (a > b) {
            return 1;
        }
        return 0;
    }

    template <typename Action, typename Context>
    void TraverseKLR(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        action(node, context);
        TraverseKLR(node->left.get(), action, context);
        TraverseKLR(node->right.get(), action, context);
    }

    template <typename Action, typename Context>
    void TraverseKRL(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        action(node, context);
        TraverseKRL(node->right.get(), action, context);
        TraverseKRL(node->left.get(), action, context);
    }

    template <typename Action, typename Context>
    void TraverseLKR(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        TraverseLKR(node->left.get(), action, context);
        action(node, context);
        TraverseLKR(node->right.get(), action, context);
    }

    template <typename Action, typename Context>
    void TraverseRKL(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        TraverseRKL(node->right.get(), action, context);
        action(node, context);
        TraverseRKL(node->left.get(), action, context);
    }

    template <typename Action, typename Context>
    void TraverseLRK(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        TraverseLRK(node->left.get(), action, context);
        TraverseLRK(node->right.get(), action, context);
        action(node, context);
    }

    template <typename Action, typename Context> 
    void TraverseRLK(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        TraverseRLK(node->right.get(), action, context);
        TraverseRLK(node->left.get(), action, context);
        action(node, context);
    } 

    static void AddToArray(Node* node, DynamicArray<T>& array) {
        array.Add(node->data);
    }

    static void AddToThread(Node* node, ThreadContext& context) {
        node->threadNext = nullptr;

        if (context.head == nullptr) {
            context.head = node;
        }
        if (context.prev != nullptr) {
            context.prev->threadNext = node;
        }
        
        context.prev = node;
    }

    size_t Height(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        size_t leftHeight = Height(node->left.get());
        size_t rightHeight = Height(node->right.get());
        return (leftHeight > rightHeight) ? (1 + leftHeight) : (1 + rightHeight);
    }

    std::unique_ptr<Node> BuildBalancedTree(const DynamicArray<T>& array, size_t start, size_t end) {
        if (start == end)
            return nullptr;
        size_t middle = start + (end - start) / 2;
        std::unique_ptr<Node> node = std::make_unique<Node>(array.Get(middle));
        node->left = BuildBalancedTree(array, start, middle);
        node->right = BuildBalancedTree(array, middle + 1, end);
        return node;
    }

    bool IsBalancedNode(Node *node) {
        if (node == nullptr) {
            return true;
        }
        size_t leftHeight = Height(node->left.get());
        size_t rightHeight = Height(node->right.get());
        if (leftHeight > rightHeight + 1 || rightHeight > leftHeight + 1) {
            return false;
        }
        return IsBalancedNode(node->left.get()) && IsBalancedNode(node->right.get());
    }

    std::string Serialize(Node* node, const std::string& format) {
        if (node == nullptr) {
            return "";
        }
        std::string result;
        for (size_t i = 0; i < format.size(); i++) {
            if (format[i] == 'K') {
                std::ostringstream stream;
                stream << node->data;
                result += stream.str();
            }
            else if (format[i] == 'L') {
                result += Serialize(node->left.get(), format);
            }
            else if (format[i] == 'R') {
                result += Serialize(node->right.get(), format);
            }
            else {
                result += format[i];
            }
        }
        return result;
    }

    std::unique_ptr<Node> CopyNode(Node* node, size_t& size) {
        if (node == nullptr) {
            return nullptr;
        }
        std::unique_ptr<Node> copy = std::make_unique<Node>(node->data);
        size++;
        copy->left = CopyNode(node->left.get(), size);
        copy->right = CopyNode(node->right.get(), size);
        return copy;
    }

public:

    bool Insert(const T& item) {
        Node* current = root.get();
        if (current == nullptr) {
            root = std::make_unique<Node>(item);
            size++;
            threadHead = nullptr;
            return true;
        }
        while (current != nullptr) {
            int compare = Compare(current->data, item);
            if (compare == 1) {
                if (current->left == nullptr) {
                    current->left = std::make_unique<Node>(item);
                    size++;
                    threadHead = nullptr;
                    return true;
                }
                current = current->left.get();
            }
            else if (compare == -1) {
                if (current->right == nullptr) {
                    current->right = std::make_unique<Node>(item);
                    size++;
                    threadHead = nullptr;
                    return true;
                }
                current = current->right.get();
            }
            else {
                return false;
            }
        }
        return false;
    }

    bool Find(const T& item) const {
        Node *current = root.get();
        while (current != nullptr) {
            int compare = Compare(current->data, item);
            if (compare == 1) {
                current = current->left.get();
            }
            else if (compare == -1) {
                current = current->right.get();
            }
            else if (compare == 0) {
                return true;
            }
        }
        return false;
    }

    bool Remove(const T& item) {
        std::unique_ptr<Node> *link = &root;
        Node *current = link->get();
        while (current != nullptr) {
            int compare = Compare(current->data, item);
            if (compare == 1) {
                link = &(current->left);
                current = link->get();
            }
            else if (compare == -1) {
                link = &(current->right);
                current = link->get();
            }
            else if (compare == 0) {
                if (current->left == nullptr && current->right == nullptr) {
                    link->reset();
                    size--;
                    threadHead = nullptr;
                    return true;
                }
                else if (current->left != nullptr && current->right == nullptr) {
                    *link = std::move(current->left);
                    size--;
                    threadHead = nullptr;
                    return true;
                }
                else if (current->left == nullptr && current->right != nullptr) {
                    *link = std::move(current->right);
                    size--;
                    threadHead = nullptr;
                    return true;
                }
                else {
                    std::unique_ptr<Node>* currentLink = &current->right;
                    current = current->right.get();
                    while (current->left != nullptr) {
                        currentLink = &(current->left);
                        current = currentLink->get();
                    }
                    (*link)->data = (*currentLink)->data;
                    *currentLink = std::move(current->right);
                    size--;
                    threadHead = nullptr;
                    return true;
                }
            }
        }
        return false;
    }

    size_t GetSize() const {
        return size;
    }

    bool IsEmpty() const {
        return size == 0;
    }

    template <typename Action, typename Context>
    void Traverse(const std::string& order, Action action, Context& context) {
        if (order == "KLR") {
            TraverseKLR(root.get(), action, context);
        }
        else if (order == "KRL") {
            TraverseKRL(root.get(), action, context);
        }
        else if (order == "LKR") {
            TraverseLKR(root.get(), action, context);
        }
        else if (order == "RKL") {
            TraverseRKL(root.get(), action, context);
        }
        else if (order == "LRK") {
            TraverseLRK(root.get(), action, context);
        }
        else if (order == "RLK") {
            TraverseRLK(root.get(), action, context);
        }
        else {
            throw std::invalid_argument("Invalid traversal order");
        }
    }

    BinarySearchTree<T> Where(bool (*predicate)(const T&)) {
        BinarySearchTree<T> result;
        DynamicArray<T> array;
        Traverse("KLR", AddToArray, array);
        for (size_t i = 0; i < array.GetSize(); i++) {
            if (predicate(array.Get(i))) {
                result.Insert(array.Get(i));
            }
        }
        return result;
    }

    BinarySearchTree<T> Map(T (*func)(const T&)) {
        BinarySearchTree<T> result;
        DynamicArray<T> array;
        Traverse("KLR", AddToArray, array);
        for (size_t i = 0; i < array.GetSize(); i++) {
            result.Insert(func(array.Get(i)));
        }
        return result;
    }

    T Reduce(T initial, T (*func)(const T&, const T&)) {
        DynamicArray<T> array;
        Traverse("KLR", AddToArray, array);
        for (size_t i = 0; i < array.GetSize(); i++) {
            initial = func(initial, array.Get(i));
        }
        return initial;
    }

    BinarySearchTree<T> Balance() {
        BinarySearchTree<T> result;
        DynamicArray<T> array;
        Traverse("LKR", AddToArray, array);
        result.root = BuildBalancedTree(array, 0, array.GetSize());
        result.size = array.GetSize();;
        return result;
    }

    bool IsBalanced() {
        return IsBalancedNode(root.get());
    }

    void Thread(const std::string& order) {
        ThreadContext context;
        Traverse(order, AddToThread, context);
        threadHead = context.head;
    }

    template <typename Action, typename Context>
    void TraverseThreaded(Action action, Context& context) {
        Node *current = threadHead;
        while (current != nullptr) {
            action(current->data, context);
            current = current->threadNext;
        }
    }

    template <typename Action, typename Context>
    void ThreadAndTraverse(const std::string& order, Action action, Context& context) {
        Thread(order);
        TraverseThreaded(action, context);
    }

    std::string ToString(const std::string& format) {
        return Serialize(root.get(), format);
    }

    BinarySearchTree<T> CreateSubTree(const T& item) {
        BinarySearchTree<T> result;
        Node *current = root.get();
        while (current != nullptr) {
            int compare = Compare(current->data, item);
            if (compare == 1) {
                current = current->left.get();
            }
            else if (compare == -1) {
                current = current->right.get();
            }
            else if (compare == 0) {
                result.root = CopyNode(current, result.size);
                return result;
            }
        }
        return result;
    }

};

#endif