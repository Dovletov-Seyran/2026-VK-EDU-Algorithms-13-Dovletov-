#include <iostream>
#include <sstream>

struct Box {
    int sides[3];
    int index;
};

bool fits(const Box& a, const Box& b) {
    return a.sides[0] < b.sides[0] &&
           a.sides[1] < b.sides[1] &&
           a.sides[2] < b.sides[2];
}

void sortSides(Box& box) {
    for (int i = 1; i < 3; i++) {
        int key = box.sides[i];
        int j = i - 1;
        while (j >= 0 && box.sides[j] > key) {
            box.sides[j + 1] = box.sides[j];
            j--;
        }
        box.sides[j + 1] = key;
    }
}

void insertionSort(Box* arr, int n) {
    for (int i = 1; i < n; i++) {
        Box key = arr[i];
        int j = i - 1;
        while (j >= 0 && fits(key, arr[j])) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

void run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;

    Box* boxes = new Box[n];
    for (int i = 0; i < n; i++) {
        boxes[i].index = i;
        input >> boxes[i].sides[0] >> boxes[i].sides[1] >> boxes[i].sides[2];
        sortSides(boxes[i]);
    }

    insertionSort(boxes, n);

    for (int i = 0; i < n; i++) {
        output << boxes[i].index;
        if (i < n - 1) output << " ";
    }
    output << std::endl;

    delete[] boxes;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}