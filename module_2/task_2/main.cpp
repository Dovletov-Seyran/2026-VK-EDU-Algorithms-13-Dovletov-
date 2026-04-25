#include <iostream>
#include <stack>
#include <vector>

template<typename T>
struct DefaultComparator {
    bool operator()(const T& left, const T& right) const {
        return left < right;
    }
};

template<typename T, typename Comparator = DefaultComparator<T>>
class BinaryTree {
    struct Node {
        Node(const T& data) : data(data), left(nullptr), right(nullptr) {}

        T data;
        Node* left;
        Node* right;
    };

public:
    BinaryTree(Comparator cmp = Comparator()) : root(nullptr), comparator(cmp) {}

    ~BinaryTree() {
        if (root == nullptr) {
            return;
        }

        std::stack<Node*> stack;
        stack.push(root);

        while (!stack.empty()) {
            Node* node = stack.top();
            stack.pop();

            if (node->left != nullptr) {
                stack.push(node->left);
            }
            if (node->right != nullptr) {
                stack.push(node->right);
            }

            delete node;
        }
    }

    void Add(const T& data) {
        if (root == nullptr) {
            root = new Node(data);
            return;
        }

        Node* current = root;

        while (true) {
            if (comparator(current->data, data) || !comparator(data, current->data)) {
                if (current->right == nullptr) {
                    current->right = new Node(data);
                    return;
                }
                current = current->right;
            } else {
                if (current->left == nullptr) {
                    current->left = new Node(data);
                    return;
                }
                current = current->left;
            }
        }
    }

    void PostOrder(std::ostream& out) const {
        if (root == nullptr) {
            return;
        }

        std::stack<Node*> stack;
        Node* current = root;
        Node* lastVisited = nullptr;

        while (current != nullptr || !stack.empty()) {
            while (current != nullptr) {
                stack.push(current);
                current = current->left;
            }

            Node* peekNode = stack.top();

            if (peekNode->right != nullptr && lastVisited != peekNode->right) {
                current = peekNode->right;
            } else {
                out << peekNode->data << " ";
                lastVisited = peekNode;
                stack.pop();
            }
        }
    }

private:
    Node* root;
    Comparator comparator;
};

int main() {
    size_t n = 0;
    std::cin >> n;

    BinaryTree<int> tree;

    for (size_t i = 0; i < n; i++) {
        int value = 0;
        std::cin >> value;
        tree.Add(value);
    }

    tree.PostOrder(std::cout);
    std::cout << std::endl;

    return 0;
}