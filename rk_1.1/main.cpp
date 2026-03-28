#include <iostream>

int BinarySearch(const int* arr, int n, int k) {
    int left = 0;
    int right = n;

    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < k) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

void run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;

    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }

    int k = 0;
    input >> k;

    output << BinarySearch(arr, n, k) << std::endl;

    delete[] arr;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}