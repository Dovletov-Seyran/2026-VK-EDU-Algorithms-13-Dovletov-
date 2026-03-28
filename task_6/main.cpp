#include <iostream>
#include <functional>
#include <cstdlib>
#include <ctime>

template <typename T, typename Comparator = std::less<T>>
int partition(T* arr, int l, int r, Comparator cmp = Comparator()) {
    int pivot_index = l + rand() % (r - l + 1);
    std::swap(arr[pivot_index], arr[l]);
    T pivot = arr[l];

    int i = r;
    int j = r;

    while (j > l) {
        if (!cmp(arr[j], pivot)) {
            std::swap(arr[i], arr[j]);
            i--;
            j--;
        } else {
            j--;
        }
    }

    std::swap(arr[l], arr[i]);
    return i;
}

template <typename T, typename Comparator = std::less<T>>
T kth_statistic(T* arr, int k, int l, int r, Comparator cmp = Comparator()) {
    int pivot_pos = partition(arr, l, r, cmp);

    if (pivot_pos == k) {
        return arr[pivot_pos];
    }
    if (pivot_pos > k) {
        return kth_statistic(arr, k, l, pivot_pos - 1, cmp);
    }
    return kth_statistic(arr, k, pivot_pos + 1, r, cmp);
}

void run(std::istream& input, std::ostream& output) {
    int n = 0;
    input >> n;

    int* arr = new int[n];
    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }

    int p10_index = n / 10;
    int p50_index = n / 2;
    int p90_index = n * 9 / 10;

    output << kth_statistic(arr, p10_index, 0, n - 1) << "\n";
    output << kth_statistic(arr, p50_index, 0, n - 1) << "\n";
    output << kth_statistic(arr, p90_index, 0, n - 1) << "\n";

    delete[] arr;
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    run(std::cin, std::cout);
    return 0;
}