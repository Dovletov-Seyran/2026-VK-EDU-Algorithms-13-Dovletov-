#include <iostream>
#include <cassert>
#include <cstring>

const int kAlphabetSize = 256;

void msd_sort(char** arr, int l, int r, int d) {
    if (l >= r) return;

    int countBufSize = kAlphabetSize + 2;
    int* countBuf = new int[countBufSize](); 

    for (int i = l; i <= r; i++) {
        int c = (d < (int)strlen(arr[i])) ? (unsigned char)arr[i][d] : -1;
        countBuf[c + 2]++;  // +2 чтобы -1 стал индексом 1
    }

    for (int i = 1; i < countBufSize; i++) {
        countBuf[i] += countBuf[i - 1];
    }

    char** tmpBuf = new char*[r - l + 1];
    for (int i = l; i <= r; i++) {
        int c = (d < (int)strlen(arr[i])) ? (unsigned char)arr[i][d] : -1;
        int pos = countBuf[c + 1]++;  
        tmpBuf[pos] = arr[i];
    }

    for (int i = l; i <= r; i++) {
        arr[i] = tmpBuf[i - l];
    }

    delete[] tmpBuf;

    for (int c = 0; c < kAlphabetSize; c++) {
        int group_l = l + countBuf[c];
        int group_r = l + countBuf[c + 1] - 1;
        if (group_l < group_r) {
            msd_sort(arr, group_l, group_r, d + 1);
        }
    }

    delete[] countBuf;
}

void run(std::istream& input, std::ostream& output) {
    const int kMaxStrings = 100000;
    char** arr = new char*[kMaxStrings];
    int n = 0;

    const int kMaxLen = 1024;
    char buffer[kMaxLen];

    while (n < kMaxStrings && input >> buffer) {
        int len = strlen(buffer);
        arr[n] = new char[len + 1];
        for (int i = 0; i <= len; i++) {
            arr[n][i] = buffer[i];
        }
        n++;
    }

    msd_sort(arr, 0, n - 1, 0);

    for (int i = 0; i < n; i++) {
        output << arr[i] << "\n";
    }

    for (int i = 0; i < n; i++) {
        delete[] arr[i];
    }
    delete[] arr;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}