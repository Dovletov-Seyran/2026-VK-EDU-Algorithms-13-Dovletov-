#include <iostream>
#include <sstream>

class Stack {
public:
    Stack() : buffer(new char[4]), capacity(4), size(0) {}

    Stack(const Stack& other) = delete;
    Stack& operator=(const Stack& other) = delete;

    ~Stack() {
        delete[] buffer;
    }

    void Push(char value) {
        if (size == capacity) {
            int newCapacity = capacity * 2;
            char* newBuffer = new char[newCapacity];
            for (int i = 0; i < size; i++) {
                newBuffer[i] = buffer[i];
            }
            delete[] buffer;
            buffer = newBuffer;
            capacity = newCapacity;
        }
        buffer[size] = value;
        size++;
    }

    char Pop() {
        size--;
        return buffer[size];
    }

    char Top() {
        return buffer[size - 1];
    }

    bool IsEmpty() {
        return size == 0;
    }

private:
    char* buffer;
    int capacity;
    int size;
};

void run(std::istream& input, std::ostream& output) {
    std::string src, dst;
    input >> src >> dst;

    Stack stack;
    int j = 0;

    for (int i = 0; i < (int)src.size(); i++) {
        stack.Push(src[i]);
        while (!stack.IsEmpty() && stack.Top() == dst[j]) {
            stack.Pop();
            j++;
        }
    }

    output << (j == (int)dst.size() ? "YES" : "NO") << std::endl;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}