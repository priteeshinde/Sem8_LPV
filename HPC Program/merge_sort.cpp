#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <omp.h>

using namespace std;

void merge(vector<int>& arr, int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
    vector<int> L(n1), R(n2);
    for (i = 0; i < n1; i++) {
        L[i] = arr[l + i];
    }
    for (j = 0; j < n2; j++) {
        R[j] = arr[m + 1 + j];
    }
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k++] = L[i++];
        } else {
            arr[k++] = R[j++];
        }
    }
    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }
}

void merge_sort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void parallel_merge_sort(vector<int>& arr) {
    #pragma omp parallel
    {
        #pragma omp single
        merge_sort(arr, 0, arr.size() - 1);
    }
}

void odd_even_transpose(vector<int>& arr) {
    int n = arr.size();
    for (int phase = 0; phase < n; ++phase) {
        // Odd phase
        #pragma omp parallel for
        for (int i = 1; i < n; i += 2) {
            if (i < n - 1 && arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
            }
        }
        
        // Even phase
        #pragma omp parallel for
        for (int i = 0; i < n; i += 2) {
            if (i < n - 1 && arr[i] > arr[i + 1]) {
                swap(arr[i], arr[i + 1]);
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of elements in the array: ";
    cin >> n;
    vector<int> arr(n);

    // Seed for random number generation
    srand(time(0));

    // Generate random values for the array
    for (int i = 0; i < n; ++i) {
        arr[i] = rand() % 100; // Generates random numbers between 0 and 99
    }

    double start, end;

    // Measure performance of sequential merge sort
    start = omp_get_wtime();
    merge_sort(arr, 0, arr.size() - 1);
    end = omp_get_wtime();
    cout << "Sequential merge sort time: " << end - start << " seconds" << endl;

    // Measure performance of parallel merge sort
    start = omp_get_wtime();
    parallel_merge_sort(arr);
    end = omp_get_wtime();
    cout << "Parallel merge sort time: " << end - start << " seconds" << endl;

    // Measure performance of odd-even transpose sort
    start = omp_get_wtime();
    odd_even_transpose(arr);
    end = omp_get_wtime();
    cout << "Odd-even transpose sort time: " << end - start << " seconds" << endl;

    cout << "Sorted array:\n";
    for (int i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;

    return 0;
}

