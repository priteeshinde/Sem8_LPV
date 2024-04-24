#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // Create temporary arrays
    int L[n1], R[n2];

    // Copy data to temporary arrays L[] and R[]
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    // Merge the temporary arrays back into arr[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        // Sort first and second halves
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);

        // Merge the sorted halves
        merge(arr, l, m, r);
    }
}

void parallel_merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            parallel_merge_sort(arr, l, m);

            #pragma omp section
            parallel_merge_sort(arr, m + 1, r);
        }

        merge(arr, l, m, r);
    }
}

int main() {
    int n;

    printf("Enter the number of elements: ");
    scanf("%d", &n);

    int arr[n];

    printf("Enter %d elements:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Measure performance of sequential merge sort
    double start_time = omp_get_wtime();
    merge_sort(arr, 0, n - 1);
    double sequential_time = omp_get_wtime() - start_time;

    printf("\nSorted array using sequential merge sort:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nSequential Merge Sort Time: %f seconds\n", sequential_time);

    // Measure performance of parallel merge sort
    start_time = omp_get_wtime();
    parallel_merge_sort(arr, 0, n - 1);
    double parallel_time = omp_get_wtime() - start_time;

    printf("\nSorted array using parallel merge sort:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nParallel Merge Sort Time: %f seconds\n", parallel_time);

    return 0;
}
/*
// ---------------------------------------------------------------------------------------------------

Enter the number of elements: 9
Enter 9 elements:
9 8 7 4 5 6 3 1 2  

Sorted array using sequential merge sort:   
1 2 3 4 5 6 7 8 9
Sequential Merge Sort Time: 0.000000 seconds

Sorted array using parallel merge sort:     
1 2 3 4 5 6 7 8 9
Parallel Merge Sort Time: 0.002000 seconds 

*/
