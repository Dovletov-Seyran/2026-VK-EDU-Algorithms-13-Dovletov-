#include <functional>
#include <iostream>
#include <queue>
#include <vector>

template<typename T, typename Comparator = std::less<T>>
class BTree {
public:
    struct Node {
        Node(bool leaf) : leaf(leaf) {}

        ~Node() {
            for (Node* child : children)
                delete child;
        }

        bool leaf;
        std::vector<T> keys;
        std::vector<Node*> children;
    };

    BTree(size_t min_degree, Comparator cmp = Comparator())
        : t(min_degree), root(nullptr), cmp(cmp) {}

    ~BTree() {
        if (root)
            delete root;
    }

    void Insert(const T& key) {
        if (!root)
            root = new Node(true);

        if (isNodeFull(root)) {
            Node* newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            splitChild(root, 0);
        }

        insertNonFull(root, key);
    }

    void PrintByLayers(std::ostream& out) {
        if (!root)
            return;

        std::queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            size_t layerSize = q.size();
            bool firstInLayer = true;

            for (size_t i = 0; i < layerSize; ++i) {
                Node* node = q.front();
                q.pop();

                for (const T& key : node->keys) {
                    if (!firstInLayer)
                        out << " ";
                    out << key;
                    firstInLayer = false;
                }

                for (Node* child : node->children)
                    q.push(child);
            }
            out << "\n";
        }
    }

private:
    bool isNodeFull(Node* node) {
        return node->keys.size() == 2 * t - 1;
    }

    void splitChild(Node* node, size_t index) {
        Node* y = node->children[index];
        Node* z = new Node(y->leaf);

        z->keys.reserve(t - 1);
        for (size_t j = 0; j < t - 1; ++j)
            z->keys.push_back(y->keys[j + t]);

        if (!y->leaf) {
            z->children.reserve(t);
            for (size_t j = 0; j < t; ++j)
                z->children.push_back(y->children[j + t]);
            y->children.resize(t);
        }

        T midKey = y->keys[t - 1];
        y->keys.resize(t - 1);

        node->children.insert(node->children.begin() + index + 1, z);
        node->keys.insert(node->keys.begin() + index, midKey);
    }

    void insertNonFull(Node* node, const T& key) {
        int pos = (int)node->keys.size() - 1;

        if (node->leaf) {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && cmp(key, node->keys[pos])) {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        } else {
            while (pos >= 0 && cmp(key, node->keys[pos]))
                pos--;

            if (isNodeFull(node->children[pos + 1])) {
                splitChild(node, pos + 1);
                if (cmp(node->keys[pos + 1], key))
                    pos++;
            }
            insertNonFull(node->children[pos + 1], key);
        }
    }

    size_t t;
    Node* root;
    Comparator cmp;
};

int main() {
    size_t t;
    std::cin >> t;

    BTree<unsigned int> tree(t);

    unsigned int key;
    while (std::cin >> key)
        tree.Insert(key);

    tree.PrintByLayers(std::cout);

    return 0;
}