#include <memory>
#include <cstddef>
#include <string>
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

        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    std::unique_ptr<Node> root;
    size_t size = 0;

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
    void TraverseKLP(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        action(node->data, context);
        TraverseKLP(node->left.get(), action, context);
        TraverseKLP(node->right.get(), action, context);
    }

    template <typename Action, typename Context>
    void TraverseKPL(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        action(node->data, context);
        TraverseKPL(node->right.get(), action, context);
        TraverseKPL(node->left.get(), action, context);
    }

    template <typename Action, typename Context>
    void TraverseLKP(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        TraverseLKP(node->left.get(), action, context);
        action(node->data, context);
        TraverseLKP(node->right.get(), action, context);
    }

    template <typename Action, typename Context>
    void TraversePKL(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        TraversePKL(node->right.get(), action, context);
        action(node->data, context);
        TraversePKL(node->left.get(), action, context);
    }

    template <typename Action, typename Context>
    void TraverseLPK(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        TraverseLPK(node->left.get(), action, context);
        TraverseLPK(node->right.get(), action, context);
        action(node->data, context);
    }

    template <typename Action, typename Context> 
    void TraversePLK(Node* node, Action action, Context& context) {
        if (node == nullptr) {
            return;
        }
        TraversePLK(node->right.get(), action, context);
        TraversePLK(node->left.get(), action, context);
        action(node->data, context);
    } 

    static void AddToArray(const T& value, DynamicArray<T>& array) {
        array.Add(value);
    }

    size_t Height(Node* node) {
        if (node == nullptr) {
            return 0;
        }

        size_t leftHeight = Height(node->left.get());
        size_t rightHeight = Height(node->right.get());

        return (leftHeight > rightHeight) ? (1 + leftHeight) : (1 + rightHeight);
    }

    bool IsBalancedNode(Node* node) {
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

public:

    bool Insert(const T& item) {
        Node* current = root.get();
        if (current == nullptr) {
            root = std::make_unique<Node>(item);
            size++;
            return true;
        }
        while (current != nullptr) {
            int compare = Compare(current->data, item);
            if (compare == 1) {
                if (current->left == nullptr) {
                    current->left = std::make_unique<Node>(item);
                    size++;
                    return true;
                }
                current = current->left.get();
            }
            else if (compare == -1) {
                if (current->right == nullptr) {
                    current->right = std::make_unique<Node>(item);
                    size++;
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
                    return true;
                }
                else if (current->left != nullptr && current->right == nullptr) {
                    *link = std::move(current->left);
                    size--;
                    return true;
                }
                else if (current->left == nullptr && current->right != nullptr) {
                    *link = std::move(current->right);
                    size--;
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
        if (order == "KLP") {
            TraverseKLP(root.get(), action, context);
        }
        else if (order == "KPL") {
            TraverseKPL(root.get(), action, context);
        }
        else if (order == "LKP") {
            TraverseLKP(root.get(), action, context);
        }
        else if (order == "PKL") {
            TraversePKL(root.get(), action, context);
        }
        else if (order == "LPK") {
            TraverseLPK(root.get(), action, context);
        }
        else if (order == "PLK") {
            TraversePLK(root.get(), action, context);
        }
        else {
            throw std::invalid_argument("Invalid traversal order");
        }
    }

    BinarySearchTree<T> Where(bool (*predicate)(const T&)) {
        BinarySearchTree<T> result;
        DynamicArray<T> array;
        Traverse("KLP", AddToArray, array);
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
        Traverse("KLP", AddToArray, array);
        for (size_t i = 0; i < array.GetSize(); i++) {
            result.Insert(func(array.Get(i)));
        }
        return result;
    }

    T Reduce(T initial, T (*func)(const T&, const T&)) {
        DynamicArray<T> array;
        Traverse("KLP", AddToArray, array);
        for (size_t i = 0; i < array.GetSize(); i++) {
            initial = func(initial, array.Get(i));
        }
        return initial;
    }

    BinarySearchTree<T> Balance() {
        BinarySearchTree<T> result;
        DynamicArray<T> array;
        Traverse("LKP", AddToArray, array);
        if (array.GetSize() % 2 == 0) {
            for (size_t i = 0; i < (array.GetSize()) / 2; i++) {
                result.Insert(array.Get((GetSize() - 1) / 2 - i));
                result.Insert(array.Get(GetSize() / 2 + i));
            }
        }
        else {
            result.Insert(array.Get((GetSize() - 1) / 2));
            for (size_t i = 0; i < (array.GetSize() - 1) / 2; i++) {
                result.Insert(array.Get((array.GetSize() - 1) / 2 - i - 1));
                result.Insert(array.Get(GetSize() / 2 + i + 1));
            }
        }
        return result;
    }

    bool IsBalanced() {
        return IsBalancedNode(root.get());
    }
};