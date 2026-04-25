#include <iostream>

int BinarySearchPeak(const int* arr, int left, int right) {
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] < arr[mid + 1]) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    return left;
}

int FindPeak(const int* arr, int n) {
    int step = 1;
    while (step < n && arr[step] > arr[step - 1]) {
        step <<= 1; 
    }

    int left = step >> 1;     
    int right = (step < n) ? step : n - 1;

    return BinarySearchPeak(arr, left, right);
}

int main() {
    int n = 0;
    std::cin >> n;
    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        std::cin >> arr[i];
    }

    std::cout << FindPeak(arr, n) << std::endl;

    delete[] arr;  
    return 0;
}