#include <iostream>
#include <cassert>
#include <sstream>

class Queue {
public:
    Queue() : buffer_(new int[kInitialCapacity]),
              capacity_(kInitialCapacity),
              size_(0),
              head_(0),
              tail_(0) {}

    Queue(const Queue& other) = delete;
    Queue& operator=(const Queue& other) = delete;

    ~Queue() {
        delete[] buffer_;
    }

    void PushBack(int value) {
        if (size_ == capacity_) {
            Resize();
        }
        buffer_[tail_] = value;
        tail_ = (tail_ + 1) % capacity_;
        size_++;
    }

    int PopFront() {
        if (IsEmpty()) {
            return -1;
        }
        int value = buffer_[head_];
        head_ = (head_ + 1) % capacity_;
        size_--;
        return value;
    }

    bool IsEmpty() const { return size_ == 0; }

private:
    static const int kInitialCapacity = 4;

    int* buffer_;
    int capacity_;
    int size_;
    int head_;  
    int tail_;  

    void Resize() {
        int new_capacity = capacity_ * 2;
        int* new_buffer = new int[new_capacity];
        for (int i = 0; i < size_; i++) {
            new_buffer[i] = buffer_[(head_ + i) % capacity_];
        }
        delete[] buffer_;
        buffer_ = new_buffer;
        capacity_ = new_capacity;
        head_ = 0;
        tail_ = size_;
    }
};

void run(std::istream& input, std::ostream& output) {
    Queue queue;

    int n = 0;
    input >> n;

    for (int i = 0; i < n; i++) {
        int command = 0;
        int value = 0;
        input >> command >> value;

        switch (command) {
            case 3: {
                queue.PushBack(value);
                break;
            }
            case 2: {
                int result = queue.PopFront();
                if (result != value) {
                    output << "NO" << std::endl;
                    return;
                }
                break;
            }
        }
    }
    output << "YES" << std::endl;
}

void testQueue() {
    {
        std::stringstream input, output;
        input << "3\n3 44\n3 50\n2 44\n";
        run(input, output);
        assert(output.str() == "YES\n");
    }
    {
        std::stringstream input, output;
        input << "2\n2 -1\n3 10\n";
        run(input, output);
        assert(output.str() == "YES\n");
    }
    {
        std::stringstream input, output;
        input << "2\n3 44\n2 66\n";
        run(input, output);
        assert(output.str() == "NO\n");
    }
}

int main() {
    run(std::cin, std::cout);
    return 0;
}