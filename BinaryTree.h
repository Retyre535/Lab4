#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iostream>
#include <vector>
#include <functional>
#include <stack>
#include <queue>
#include <sstream>
#include <string>
#include <stdexcept>
#include <memory>
#include <algorithm>
#include "Sequences.h"
#include <complex>

template<typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node *left;
        Node *right;

        Node(const T &item) : data(item), left(nullptr), right(nullptr) {
        }
    };

    Node *root;

    void deleteTree(Node *node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    Node *copyTree(Node *node) {
        if (!node) return nullptr;

        Node *newNode = new Node(node->data);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);

        return newNode;
    }

    Node *insertNode(Node *node, const T &item) {
        if (!node) return new Node(item);

        if (compareItems(item, node->data) < 0) {
            node->left = insertNode(node->left, item);
        } else if (compareItems(item, node->data) > 0) {
            node->right = insertNode(node->right, item);
        }

        return node;
    }

    Node *findNode(Node *node, const T &item) const {
        if (!node) return nullptr;

        int compResult = compareItems(item, node->data);
        if (compResult == 0) {
            return node;
        } else if (compResult < 0) {
            return findNode(node->left, item);
        } else {
            return findNode(node->right, item);
        }
    }

    Node *findMinNode(Node *node) const {
        if (!node) return nullptr;

        while (node->left) {
            node = node->left;
        }
        return node;
    }

    Node *deleteNode(Node *node, const T &item) {
        if (!node) return nullptr;

        int compResult = compareItems(item, node->data);
        if (compResult < 0) {
            node->left = deleteNode(node->left, item);
        } else if (compResult > 0) {
            node->right = deleteNode(node->right, item);
        } else {
            if (!node->left) {
                Node *temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Node *temp = node->left;
                delete node;
                return temp;
            }

            Node *temp = findMinNode(node->right);
            node->data = temp->data;
            node->right = deleteNode(node->right, temp->data);
        }

        return node;
    }

    Node *mapTree(Node *node, const std::function<T(const T &)> &func) const {
        if (!node) return nullptr;

        Node *newNode = new Node(func(node->data));
        newNode->left = mapTree(node->left, func);
        newNode->right = mapTree(node->right, func);

        return newNode;
    }

    Node *whereTree(Node *node, const std::function<bool(const T &)> &predicate) const {
        if (!node) return nullptr;

        Node *resultNode = nullptr;

        if (predicate(node->data)) {
            resultNode = new Node(node->data);
            resultNode->left = whereTree(node->left, predicate);
            resultNode->right = whereTree(node->right, predicate);
        } else {
            Node *leftResult = whereTree(node->left, predicate);
            Node *rightResult = whereTree(node->right, predicate);

            if (leftResult || rightResult) {
                resultNode = new Node(node->data);
                resultNode->left = leftResult;
                resultNode->right = rightResult;
            }
        }

        return resultNode;
    }

    bool isSubtree(Node *mainTree, Node *subtree) const {
        if (!subtree) return true;
        if (!mainTree) return false;

        if (areIdentical(mainTree, subtree)) {
            return true;
        }

        return isSubtree(mainTree->left, subtree) || isSubtree(mainTree->right, subtree);
    }

    bool areIdentical(Node *a, Node *b) const {
        if (!a && !b) return true;
        if (!a || !b) return false;

        return (a->data == b->data) &&
               areIdentical(a->left, b->left) &&
               areIdentical(a->right, b->right);
    }

    void traverseInOrder(Node *node, std::vector<T> &result) const {
        if (!node) return;

        traverseInOrder(node->left, result);
        result.push_back(node->data);
        traverseInOrder(node->right, result);
    }

    void traversePreOrder(Node *node, std::vector<T> &result) const {
        if (!node) return;

        result.push_back(node->data);
        traversePreOrder(node->left, result);
        traversePreOrder(node->right, result);
    }

    void traversePostOrder(Node *node, std::vector<T> &result) const {
        if (!node) return;

        traversePostOrder(node->left, result);
        traversePostOrder(node->right, result);
        result.push_back(node->data);
    }

    void traverseKLP(Node *node, std::vector<T> &result) const {
        if (!node) return;

        result.push_back(node->data);
        traverseKLP(node->left, result);
        traverseKLP(node->right, result);
    }

    void traverseKPL(Node *node, std::vector<T> &result) const {
        if (!node) return;

        result.push_back(node->data);
        traverseKPL(node->right, result);
        traverseKPL(node->left, result);
    }

    void traverseLPK(Node *node, std::vector<T> &result) const {
        if (!node) return;

        traverseLPK(node->left, result);
        traverseLPK(node->right, result);
        result.push_back(node->data);
    }

    void traverseLKP(Node *node, std::vector<T> &result) const {
        if (!node) return;

        traverseLKP(node->left, result);
        result.push_back(node->data);
        traverseLKP(node->right, result);
    }

    void traversePLK(Node *node, std::vector<T> &result) const {
        if (!node) return;

        traversePLK(node->right, result);
        traversePLK(node->left, result);
        result.push_back(node->data);
    }

    void traversePKL(Node *node, std::vector<T> &result) const {
        if (!node) return;

        traversePKL(node->right, result);
        result.push_back(node->data);
        traversePKL(node->left, result);
    }

    void serializeTree(Node *node, std::string &result, const std::string &format) const {
        if (!node) {
            result += "null,";
            return;
        }

        for (size_t i = 0; i < format.size(); i++) {
            char c = format[i];
            if (c == 'K' || c == 'k') {
                result += std::to_string(node->data) + ",";
            } else if (c == 'L' || c == 'l') {
                serializeTree(node->left, result, format);
            } else if (c == 'P' || c == 'p') {
                serializeTree(node->right, result, format);
            }
        }
    }

    Node *deserializeTree(std::istringstream &iss, const std::string &format) {
        std::string val;
        if (!std::getline(iss, val, ',')) return nullptr;

        if (val == "null") return nullptr;

        Node *node = new Node(static_cast<T>(std::stoi(val)));

        for (size_t i = 0; i < format.size(); i++) {
            char c = format[i];
            if (c == 'L' || c == 'l') {
                node->left = deserializeTree(iss, format);
            } else if (c == 'P' || c == 'p') {
                node->right = deserializeTree(iss, format);
            }
        }

        return node;
    }

    Node *extractSubtree(Node *node, const T &rootValue) const {
        if (!node) return nullptr;

        if (node->data == rootValue) {
            Node *newNode = new Node(node->data);
            if (node->left) {
                newNode->left = new Node(node->left->data);
                if (node->left->left) {
                    Node *tempLeft = copySubtree(node->left->left);
                    newNode->left->left = tempLeft;
                }
                if (node->left->right) {
                    Node *tempRight = copySubtree(node->left->right);
                    newNode->left->right = tempRight;
                }
            }
            if (node->right) {
                newNode->right = new Node(node->right->data);
                if (node->right->left) {
                    Node *tempLeft = copySubtree(node->right->left);
                    newNode->right->left = tempLeft;
                }
                if (node->right->right) {
                    Node *tempRight = copySubtree(node->right->right);
                    newNode->right->right = tempRight;
                }
            }
            return newNode;
        }

        Node *leftResult = extractSubtree(node->left, rootValue);
        if (leftResult) return leftResult;

        return extractSubtree(node->right, rootValue);
    }

    Node *copySubtree(Node *node) const {
        if (!node) return nullptr;

        Node *newNode = new Node(node->data);
        newNode->left = copySubtree(node->left);
        newNode->right = copySubtree(node->right);

        return newNode;
    }

    Node *balanceTree(std::vector<T> &sortedArray, int start, int end) {
        if (start > end) return nullptr;

        int mid = start + (end - start) / 2;
        Node *node = new Node(sortedArray[mid]);

        node->left = balanceTree(sortedArray, start, mid - 1);
        node->right = balanceTree(sortedArray, mid + 1, end);

        return node;
    }

public:
    BinaryTree() : root(nullptr) {
    }

    BinaryTree(const BinaryTree &other) {
        root = copyTree(other.root);
    }

    BinaryTree &operator=(const BinaryTree &other) {
        if (this != &other) {
            deleteTree(root);
            root = copyTree(other.root);
        }
        return *this;
    }

    BinaryTree(BinaryTree &&other) noexcept : root(other.root) {
        other.root = nullptr;
    }

    BinaryTree &operator=(BinaryTree &&other) noexcept {
        if (this != &other) {
            deleteTree(root);
            root = other.root;
            other.root = nullptr;
        }
        return *this;
    }

    ~BinaryTree() {
        deleteTree(root);
    }

    void insert(const T &item) {
        root = insertNode(root, item);
    }

    bool contains(const T &item) const {
        return findNode(root, item) != nullptr;
    }

    void remove(const T &item) {
        root = deleteNode(root, item);
    }

    BinaryTree<T> map(const std::function<T(const T &)> &func) const {
        BinaryTree<T> result;
        result.root = mapTree(root, func);
        return result;
    }

    BinaryTree<T> where(const std::function<bool(const T &)> &predicate) const {
        BinaryTree<T> result;
        result.root = whereTree(root, predicate);
        return result;
    }

    bool containsSubtree(const BinaryTree<T> &subtree) const {
        if (!subtree.root) return true;
        if (!root) return false;

        return isSubtree(root, subtree.root);
    }

    BinaryTree<T> extractSubtree(const T &rootValue) const {
        BinaryTree<T> result;
        result.root = extractSubtree(root, rootValue);
        return result;
    }

    BinaryTree<T> merge(const BinaryTree<T> &other) const {
        BinaryTree<T> result(*this);

        std::vector<T> values;
        std::vector<T> otherValues = other.traverseInOrder();

        for (const T &value: otherValues) {
            result.insert(value);
        }

        return result;
    }

    void balance() {
        std::vector<T> sortedValues;
        traverseInOrder(root, sortedValues);

        deleteTree(root);
        root = balanceTree(sortedValues, 0, sortedValues.size() - 1);
    }

    std::vector<T> traverseInOrder() const {
        std::vector<T> result;
        traverseInOrder(root, result);
        return result;
    }

    std::vector<T> traversePreOrder() const {
        std::vector<T> result;
        traversePreOrder(root, result);
        return result;
    }

    std::vector<T> traversePostOrder() const {
        std::vector<T> result;
        traversePostOrder(root, result);
        return result;
    }

    std::vector<T> traverseKLP() const {
        std::vector<T> result;
        traverseKLP(root, result);
        return result;
    }

    std::vector<T> traverseKPL() const {
        std::vector<T> result;
        traverseKPL(root, result);
        return result;
    }

    std::vector<T> traverseLPK() const {
        std::vector<T> result;
        traverseLPK(root, result);
        return result;
    }

    std::vector<T> traverseLKP() const {
        std::vector<T> result;
        traverseLKP(root, result);
        return result;
    }

    std::vector<T> traversePLK() const {
        std::vector<T> result;
        traversePLK(root, result);
        return result;
    }

    std::vector<T> traversePKL() const {
        std::vector<T> result;
        traversePKL(root, result);
        return result;
    }

    std::vector<T> traverseCustom(const std::string &format) const {
        std::vector<T> result;
        traverseCustom(root, result, format);
        return result;
    }

    void traverseCustom(Node *node, std::vector<T> &result, const std::string &format) const {
        if (!node) return;

        for (char c: format) {
            if (c == 'K' || c == 'k') {
                result.push_back(node->data);
            } else if (c == 'L' || c == 'l') {
                traverseCustom(node->left, result, format);
            } else if (c == 'P' || c == 'p') {
                traverseCustom(node->right, result, format);
            }
        }
    }

    std::string saveToString(const std::string &format = "KLP") const {
        std::string result;
        serializeTree(root, result, format);
        return result;
    }

    void loadFromString(const std::string &str, const std::string &format = "KLP") {
        deleteTree(root);
        std::istringstream iss(str);
        root = deserializeTree(iss, format);
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    int size() const {
        return traverseInOrder().size();
    }

    int compareItems(const T &a, const T &b) const {
        if (a < b) return -1;
        if (b < a) return 1;
        return 0;
    }
};

template<typename R, typename... Args>
class BinaryTree<std::function<R(Args...)> > {
private:
    struct Node {
        std::function<R(Args...)> data;
        Node *left;
        Node *right;
        size_t id;

        Node(const std::function<R(Args...)> &item, size_t nodeId)
            : data(item), left(nullptr), right(nullptr), id(nodeId) {
        }
    };

    Node *root;
    size_t nextId;

    void deleteTree(Node *node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    Node *copyTree(Node *node) {
        if (!node) return nullptr;

        Node *newNode = new Node(node->data, node->id);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);

        return newNode;
    }

    Node *insertNode(Node *node, const std::function<R(Args...)> &item) {
        if (!node) return new Node(item, nextId++);

        node->right = insertNode(node->right, item);
        return node;
    }

    void traverseInOrder(Node *node, std::vector<std::function<R(Args...)> > &result) const {
        if (!node) return;

        traverseInOrder(node->left, result);
        result.push_back(node->data);
        traverseInOrder(node->right, result);
    }

public:
    BinaryTree() : root(nullptr), nextId(0) {
    }

    BinaryTree(const BinaryTree &other) : nextId(other.nextId) {
        root = copyTree(other.root);
    }

    BinaryTree &operator=(const BinaryTree &other) {
        if (this != &other) {
            deleteTree(root);
            root = copyTree(other.root);
            nextId = other.nextId;
        }
        return *this;
    }

    BinaryTree(BinaryTree &&other) noexcept : root(other.root), nextId(other.nextId) {
        other.root = nullptr;
    }

    BinaryTree &operator=(BinaryTree &&other) noexcept {
        if (this != &other) {
            deleteTree(root);
            root = other.root;
            nextId = other.nextId;
            other.root = nullptr;
        }
        return *this;
    }

    ~BinaryTree() {
        deleteTree(root);
    }

    void insert(const std::function<R(Args...)> &item) {
        root = insertNode(root, item);
    }

    std::vector<std::function<R(Args...)> > traverseInOrder() const {
        std::vector<std::function<R(Args...)> > result;
        traverseInOrder(root, result);
        return result;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    int size() const {
        std::vector<std::function<R(Args...)> > values;
        traverseInOrder(root, values);
        return values.size();
    }
};

template<typename ValueType>
class BinaryTree<std::complex<ValueType> > {
private:
    struct Node {
        std::complex<ValueType> data;
        Node *left;
        Node *right;

        Node(const std::complex<ValueType> &item) : data(item), left(nullptr), right(nullptr) {
        }
    };

    Node *root;

    int compareComplex(const std::complex<ValueType> &a, const std::complex<ValueType> &b) const {
        if (a.real() < b.real()) return -1;
        if (a.real() > b.real()) return 1;
        if (a.imag() < b.imag()) return -1;
        if (a.imag() > b.imag()) return 1;
        return 0;
    }

    void deleteTree(Node *node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

    Node *copyTree(Node *node) {
        if (!node) return nullptr;

        Node *newNode = new Node(node->data);
        newNode->left = copyTree(node->left);
        newNode->right = copyTree(node->right);

        return newNode;
    }

    Node *insertNode(Node *node, const std::complex<ValueType> &item) {
        if (!node) return new Node(item);

        int compResult = compareComplex(item, node->data);
        if (compResult < 0) {
            node->left = insertNode(node->left, item);
        } else if (compResult > 0) {
            node->right = insertNode(node->right, item);
        }

        return node;
    }

    Node *findNode(Node *node, const std::complex<ValueType> &item) const {
        if (!node) return nullptr;

        int compResult = compareComplex(item, node->data);
        if (compResult == 0) {
            return node;
        } else if (compResult < 0) {
            return findNode(node->left, item);
        } else {
            return findNode(node->right, item);
        }
    }

    void traverseInOrder(Node *node, std::vector<std::complex<ValueType> > &result) const {
        if (!node) return;

        traverseInOrder(node->left, result);
        result.push_back(node->data);
        traverseInOrder(node->right, result);
    }

public:
    BinaryTree() : root(nullptr) {
    }

    BinaryTree(const BinaryTree &other) {
        root = copyTree(other.root);
    }

    BinaryTree &operator=(const BinaryTree &other) {
        if (this != &other) {
            deleteTree(root);
            root = copyTree(other.root);
        }
        return *this;
    }

    ~BinaryTree() {
        deleteTree(root);
    }

    void insert(const std::complex<ValueType> &item) {
        root = insertNode(root, item);
    }

    bool contains(const std::complex<ValueType> &item) const {
        return findNode(root, item) != nullptr;
    }

    std::vector<std::complex<ValueType> > traverseInOrder() const {
        std::vector<std::complex<ValueType> > result;
        traverseInOrder(root, result);
        return result;
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    int size() const {
        std::vector<std::complex<ValueType> > values;
        traverseInOrder(root, values);
        return values.size();
    }
};

#endif
