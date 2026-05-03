#include <functional>
#include <iostream>

template<typename T, typename Comparator = std::less<T>>
class AvlTree {
    struct Node {
        Node(const T& data) : data(data), left(nullptr), right(nullptr), height(1), count(1) {}

        T data;
        Node* left;
        Node* right;
        size_t height;
        size_t count;
    };

public:
    AvlTree(Comparator cmp = Comparator()) : root(nullptr), cmp(cmp) {}

    ~AvlTree() {
        destroyTree(root);
    }

    void Add(const T& data) {
        root = addInternal(root, data);
    }

    void Delete(const T& data) {
        root = deleteInternal(root, data);
    }

    const T& KthStatistic(size_t k) {
        Node* node = root;
        while (node) {
            size_t leftCount = getCount(node->left);
            if (k == leftCount)
                return node->data;
            else if (k < leftCount)
                node = node->left;
            else {
                k -= leftCount + 1;
                node = node->right;
            }
        }
        return root->data;
    }

private:
    void destroyTree(Node* node) {
        if (node) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    Node* addInternal(Node* node, const T& data) {
        if (!node)
            return new Node(data);

        if (cmp(data, node->data))
            node->left = addInternal(node->left, data);
        else
            node->right = addInternal(node->right, data);

        return doBalance(node);
    }

    Node* deleteInternal(Node* node, const T& data) {
        if (!node)
            return nullptr;

        if (cmp(data, node->data))
            node->left = deleteInternal(node->left, data);
        else if (cmp(node->data, data))
            node->right = deleteInternal(node->right, data);
        else {
            Node* left = node->left;
            Node* right = node->right;

            delete node;

            if (!right)
                return left;
            if (!left)
                return right;

            Node* replacement;
            if (getHeight(right) >= getHeight(left)) {
                Node* newRight = findAndRemoveMin(right, replacement);
                replacement->right = newRight;
                replacement->left = left;
            } else {
                Node* newLeft = findAndRemoveMax(left, replacement);
                replacement->left = newLeft;
                replacement->right = right;
            }

            return doBalance(replacement);
        }

        return doBalance(node);
    }

    Node* findAndRemoveMin(Node* node, Node*& outMin) {
        if (!node->left) {
            outMin = node;
            return node->right;
        }
        node->left = findAndRemoveMin(node->left, outMin);
        return doBalance(node);
    }

    Node* findAndRemoveMax(Node* node, Node*& outMax) {
        if (!node->right) {
            outMax = node;
            return node->left;
        }
        node->right = findAndRemoveMax(node->right, outMax);
        return doBalance(node);
    }

    size_t getHeight(Node* node) {
        return node ? node->height : 0;
    }

    size_t getCount(Node* node) {
        return node ? node->count : 0;
    }

    void fixHeight(Node* node) {
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void fixCount(Node* node) {
        node->count = getCount(node->left) + getCount(node->right) + 1;
    }

    int getBalance(Node* node) {
        return (int)getHeight(node->right) - (int)getHeight(node->left);
    }

    Node* rotateLeft(Node* node) {
        Node* tmp = node->right;
        node->right = tmp->left;
        tmp->left = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node* rotateRight(Node* node) {
        Node* tmp = node->left;
        node->left = tmp->right;
        tmp->right = node;
        fixHeight(node);
        fixHeight(tmp);
        fixCount(node);
        fixCount(tmp);
        return tmp;
    }

    Node* doBalance(Node* node) {
        fixHeight(node);
        fixCount(node);
        switch (getBalance(node)) {
            case 2: {
                if (getBalance(node->right) < 0)
                    node->right = rotateRight(node->right);
                return rotateLeft(node);
            }
            case -2: {
                if (getBalance(node->left) > 0)
                    node->left = rotateLeft(node->left);
                return rotateRight(node);
            }
            default:
                return node;
        }
    }

    Node* root;
    Comparator cmp;
};

int main() {
    int n;
    std::cin >> n;

    AvlTree<int> tree;

    for (int i = 0; i < n; ++i) {
        int a;
        size_t k;
        std::cin >> a >> k;

        if (a > 0)
            tree.Add(a);
        else
            tree.Delete(-a);

        std::cout << tree.KthStatistic(k) << "\n";
    }

    return 0;
}