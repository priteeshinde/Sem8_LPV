// %%writefile bubblesort.cpp

#include <iostream>  // For standard I/O operations
#include <vector>  // For using dynamic arrays (vectors)
#include <chrono>  // For measuring time
#include <omp.h>  // For OpenMP support
 //#include <pthread.h> 

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

        // Parallelize the odd phase with OpenMP
        #pragma omp parallel for
        for (int i = 1; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {  // If adjacent elements are out of order
                swap(arr[i], arr[i + 1]);  // Swap them
                isSorted = false;  // Mark that sorting is not complete
            }
        }

        // Parallelize the even phase with OpenMP
        #pragma omp parallel for
        for (int i = 0; i < n - 1; i += 2) {
            if (arr[i] > arr[i + 1]) {  // If adjacent elements are out of order
                swap(arr[i], arr[i + 1]);  // Swap them
                isSorted = false;  // Mark that sorting is not complete
            }
        }
    }
}

int main() {
    int n;
    cout << "Enter the number of elements: ";
    cin >> n;  // Get the number of elements to sort

    // Create a vector and ask the user to enter elements
    vector<int> arr(n);
    cout << "Enter " << n << " elements to be sorted:\n";
    for (int i = 0; i < n; i++) {
        cin >> arr[i];  // User inputs the elements
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
    cout << "Sequential bubble sort time: " << elapsed_seq.count() << " ms" << endl;
    cout << "OpenMP odd-even bubble sort time: " << elapsed_par.count() << " ms" << endl;

    cout << "Sorted array: ";
    for (int i : arr) {
        cout << i << " ";  // Display sorted elements
    }
    cout << endl;

    return 0;
}
