#include <iostream>

struct Event {
    int position;  
    int type;  

    Event() : position(0), type(0) {}
    Event(int pos, int t) : position(pos), type(t) {}
};

class EventComparator {
public:
    EventComparator() {}

    bool operator()(const Event& a, const Event& b) const {
        if (a.position != b.position) {
            return a.position < b.position;
        }
        return a.type < b.type; 
    }
};

template <typename T, typename Comparator>
void Merge(T* arr, T* temp, int left, int mid, int right, Comparator cmp) {
    for (int i = left; i <= right; i++) {
        temp[i] = arr[i];
    }

    int i = left;    
    int j = mid + 1;
    int k = left;    

    while (i <= mid && j <= right) {
        if (cmp(temp[i], temp[j])) {
            arr[k] = temp[i];
            i++;
        } else {
            arr[k] = temp[j];
            j++;
        }
        k++;
    }

    while (i <= mid) {
        arr[k] = temp[i];
        i++;
        k++;
    }

}

template <typename T, typename Comparator>
void MergeSortHelper(T* arr, T* temp, int left, int right, Comparator cmp) {
    if (left >= right) return; 

    int mid = left + (right - left) / 2;
    MergeSortHelper(arr, temp, left, mid, cmp);       
    MergeSortHelper(arr, temp, mid + 1, right, cmp);  
    Merge(arr, temp, left, mid, right, cmp);           
}

template <typename T, typename Comparator = std::less<T>>
void MergeSort(T* arr, int size, Comparator cmp = Comparator()) {
    if (size <= 1) return;
    T* temp = new T[size]; 
    MergeSortHelper(arr, temp, 0, size - 1, cmp);
    delete[] temp;
}

int CalcPaintedLength(Event* events, int count) {
    MergeSort(events, count, EventComparator());

    int length = 0;     
    int active = 0;       
    int start = 0;        

    for (int i = 0; i < count; i++) {
        if (active > 0) {
            length += events[i].position - events[i - 1].position;
        }

        active += events[i].type;  
    }

    return length;
}

int main() {
    int n = 0;
    std::cin >> n;

    Event* events = new Event[n * 2];

    for (int i = 0; i < n; i++) {
        int left = 0;
        int right = 0;
        std::cin >> left >> right;
        events[i * 2] = Event(left, 1);    
        events[i * 2 + 1] = Event(right, -1); 
    }

    std::cout << CalcPaintedLength(events, n * 2) << std::endl;

    delete[] events;
    return 0;
}