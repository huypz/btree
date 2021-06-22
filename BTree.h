#ifndef BTREE_H
#define BTREE_H

#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

using std::cout; using std::endl;

class BTree {
    public:
        struct Node {
            Node(int m, bool external);
            std::ofstream& traverse(int level, const int& target, std::ofstream& out);
            void split(int* val, Node*& z);
            Node* createRoot(int val, Node* z);
            void insert(int k, int* val, Node*& z);
            Node* search(int k);
            int height();
            std::vector<int> keys;
            std::vector<Node*> C;
            int m;
            bool external;
        };

    public:
        BTree(int m);
        void insert(int k);
        std::ofstream& traverse(int target, std::ofstream& out);
        Node* search(int k);
        int height();

    private:
        Node* root;
        int m;
};

// B-Tree Node
BTree::Node::Node(int m, bool external) {
    this->m = m;
    this->external = external;
    keys.resize(0);
    C.resize(0);
}

std::ofstream& BTree::Node::traverse(int level, const int& target, std::ofstream& out) {
    int i;
    for (i = 0; i < keys.size(); i++) {
        if (!external) {
            C[i]->traverse(level + 1, target, out);
        }
        if (level == target) {
            out << keys[i] << " ";
        }
    }
    if (!external) {
        C[i]->traverse(level + 1, target, out);
    }
    return out;
}

void BTree::Node::split(int* val, Node*& z) {
    z = new Node(m, false);
    int t = (m % 2 == 0) ? (m / 2 - 1) : (m / 2);
    *val = keys[t];
    for (int i = t + 1; i < m; i++) {
        z->keys.push_back(keys[i]);
    }
    keys.resize(t);
    for (int i = t + 1; i <= m; i++) {
        z->C.push_back(C[i]);
    }
    C.resize(t + 1);
}

void BTree::Node::insert(int k, int* val, Node*& z) {
    if (!external) {
        int i = 0;
        while (i < keys.size() && k > keys[i]) i++;
        C[i]->insert(k, val, z);

        if (z != nullptr) {
            if (keys.size() < m - 1) {
                keys.insert(keys.begin() + i, *val);
                C.insert(C.begin() + i + 1, z);
                z = nullptr;
            }
            else {
                keys.insert(keys.begin() + i, *val);
                C.insert(C.begin() + i + 1, z);
                split(val, z);
            }
        }
    }
    else {
        std::vector<int>::iterator it;
        it = std::lower_bound(keys.begin(), keys.end(), k);
        keys.insert(it, k);
        if (keys.size() == m) {
            z = new Node(m, true);
            int t = (m % 2 == 0) ? (m / 2 - 1) : (m / 2);
            *val = keys[t];
            for (int i = t + 1; i < m; i++) {
                z->keys.push_back(keys[i]);
            }
            keys.resize(t);
        }
    }
}

typename BTree::Node* BTree::Node::createRoot(int val, Node* z) {
    Node* root = new Node(m, false);
    root->keys.push_back(val);
    root->C.push_back(this);
    root->C.push_back(z);
    return root;
}

typename BTree::Node* BTree::Node::search(int k) {
    if (keys.size() == 0) return nullptr;
    int i = 0;
    while (i < keys.size() && k > keys[i]) i++;
    if (keys[i] == k) return this;
    if (external) return nullptr;
    return C[i]->search(k);
}

int BTree::Node::height() {
    if (external || C.size() == 0) return 1;
    return 1 + C[0]->height();
}

// B-Tree
BTree::BTree(int m) {
    this->m = m;
    root = new Node(m, true);
}

void BTree::insert(int k) {
    Node* z = nullptr;
    int val = 0;
    root->insert(k, &val, z);

    if (z != nullptr)
        root = root->createRoot(val, z);
}

std::ofstream& BTree::traverse(int target, std::ofstream& out) {
    return root->traverse(1, target, out);
}

typename BTree::Node* BTree::search(int k) {
    return root->search(k);
}

int BTree::height() {
    if (root->keys.size() == 0) return 0;
    return root->height();
}
#endif