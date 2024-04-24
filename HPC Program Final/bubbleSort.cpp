// %%writefile bubblesort.cpp

#include <iostream>  // For standard I/O operations
#include <vector>  // For using dynamic arrays (vectors)
#include <chrono>  // For measuring time
#include <iomanip>  // For setprecision
#include <omp.h>  // For OpenMP support
#include <cstdlib>  // For rand() and srand()
#include <ctime>  // For time()

using namespace std;

// Function to perform sequential bubble sort
void sequential_bubble_sort(vector<int>& arr) {
    bool isSorted = false;

    while (!isSorted) {
        isSorted = true;  // Assume sorted unless proven otherwise

        // Traverse the array
        for (int i = 0; i < arr.size() - 1; i++) {
            if (arr[i] > arr[i + 1]) {  // If adjacent elements are out of order
                swap(arr[i], arr[i + 1]);  // Swap them
                isSorted = false;  // Mark that sorting is not complete
            }
        }
    }
}

// Function to perform odd-even bubble sort with OpenMP
void parallel_bubble_sort_odd_even(vector<int>& arr) {
    bool isSorted = false;
    int n = arr.size();

    while (!isSorted) {
        isSorted = true; // Assume sorted unless proven otherwise
        bool localIsSorted = true; // Assume sorted in each thread unless proven otherwise

        // Parallelize the odd phase with OpenMP
        #pragma omp parallel for reduction(&&: localIsSorted)
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {  // If adjacent elements are out of order
                swap(arr[i], arr[i + 1]);  // Swap them
                localIsSorted = false;  // Mark that sorting is not complete in this thread
            }
        }
        // Update the global sorting status based on the sorting status in each thread
        isSorted = isSorted && localIsSorted;

        // Parallelize the even phase with OpenMP
        #pragma omp parallel for reduction(&&: localIsSorted)
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {  // If adjacent elements are out of order
                swap(arr[i], arr[i + 1]);  // Swap them
                localIsSorted = false;  // Mark that sorting is not complete in this thread
            }
        }
        // Update the global sorting status based on the sorting status in each thread
        isSorted = isSorted && localIsSorted;
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;  // Get the number of elements to sort

    // Generate random input
    vector<int> arr(n);
    srand(time(0));  // Seed the random number generator with current time
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 1000;  // Generate random numbers between 0 and 999
    }

    // Measure time for sequential bubble sort
    auto start_seq = chrono::high_resolution_clock::now();
    sequential_bubble_sort(arr);  // Perform sequential bubble sort
    auto end_seq = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed_seq = end_seq - start_seq;

    // Measure time for OpenMP parallel bubble sort
    auto start_par = chrono::high_resolution_clock::now();
    parallel_bubble_sort_odd_even(arr);  // Perform parallel bubble sort with OpenMP
    auto end_par = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> elapsed_par = end_par - start_par;

    // Display results
    cout << "Sequential bubble sort time: " << fixed << setprecision(5) << elapsed_seq.count() << " ms" << endl;
    cout << "OpenMP odd-even bubble sort time: " << fixed << setprecision(5) << elapsed_par.count() << " ms" << endl;

    if (elapsed_par.count() < elapsed_seq.count()) {
        cout << "Parallel execution is faster than sequential." << endl;
    } else {
        cout << "Parallel execution is not faster than sequential." << endl;
    }

    cout << "Sorted array: ";
    for (int i : arr) {
        cout << i << " ";  // Display sorted elements
    }
    cout << endl;

    return 0;
}
