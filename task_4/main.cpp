#include <iostream>

struct Process {
    int priority;  
    int elapsed;   
    int total;  

    Process() : priority(0), elapsed(0), total(0) {}
    Process(int p, int t, int total) : priority(p), elapsed(t), total(total) {}
};

class ProcessComparator {
public:
    ProcessComparator() {}

    bool operator()(const Process& a, const Process& b) const {
        return a.priority * (a.elapsed + 1) > b.priority * (b.elapsed + 1);
    }
};

template <typename T, typename Comparator = ProcessComparator>
class Heap {
public:
    Heap() : buffer_(new T[kInitialCapacity]),
             capacity_(kInitialCapacity),
             size_(0),
             cmp_(Comparator()) {} 

    ~Heap() {
        delete[] buffer_;
    }

    Heap(const Heap& other) : buffer_(new T[other.capacity_]),
                               capacity_(other.capacity_),
                               size_(other.size_),
                               cmp_(other.cmp_) {
        for (int i = 0; i < size_; i++) {
            buffer_[i] = other.buffer_[i];
        }
    }

    Heap& operator=(const Heap& other) {
        if (this != &other) {
            delete[] buffer_;
            buffer_ = new T[other.capacity_];
            capacity_ = other.capacity_;
            size_ = other.size_;
            cmp_ = other.cmp_;
            for (int i = 0; i < size_; i++) {
                buffer_[i] = other.buffer_[i];
            }
        }
        return *this;
    }

    void Insert(const T& value) {
        if (size_ == capacity_) {
            Resize();
        }
        buffer_[size_] = value;
        size_++;
        SiftUp(size_ - 1);
    }

    T ExtractMin() {
        T min_value = buffer_[0];
        buffer_[0] = buffer_[size_ - 1];
        size_--;
        if (size_ > 0) {
            SiftDown(0);
        }
        return min_value;
    }

    bool IsEmpty() const { return size_ == 0; }

private:
    static const int kInitialCapacity = 4;

    T* buffer_;
    int capacity_;
    int size_;
    Comparator cmp_; 

    void Resize() {
        int new_capacity = capacity_ * 2;
        T* new_buffer = new T[new_capacity];
        for (int i = 0; i < size_; i++) {
            new_buffer[i] = buffer_[i];
        }
        delete[] buffer_;
        buffer_ = new_buffer;
        capacity_ = new_capacity;
    }

    void SiftUp(int index) {
        while (index > 0) {
            int parent = (index - 1) / 2;
            if (cmp_(buffer_[parent], buffer_[index])) {
                std::swap(buffer_[parent], buffer_[index]);
                index = parent;
            } else {
                break;
            }
        }
    }

    void SiftDown(int index) {
        while (true) {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int smallest = index;

            if (left < size_ && cmp_(buffer_[smallest], buffer_[left])) {
                smallest = left;
            }
            if (right < size_ && cmp_(buffer_[smallest], buffer_[right])) {
                smallest = right;
            }

            if (smallest == index) break;

            std::swap(buffer_[index], buffer_[smallest]);
            index = smallest;
        }
    }
};

int CountSwitches(Process* processes, int count) {
    Heap<Process, ProcessComparator> heap;

    for (int i = 0; i < count; i++) {
        heap.Insert(processes[i]);
    }

    int switches = 0;

    while (!heap.IsEmpty()) {
        Process current = heap.ExtractMin();
        switches++;

        current.elapsed += current.priority;

        if (current.elapsed < current.total) {
            heap.Insert(current);
        }
    }

    return switches;
}

int main() {
    int n = 0;
    std::cin >> n;

    Process* processes = new Process[n];
    for (int i = 0; i < n; i++) {
        processes[i].elapsed = 0;
        std::cin >> processes[i].priority >> processes[i].total;
    }

    std::cout << CountSwitches(processes, n) << std::endl;

    delete[] processes;
    return 0;
}