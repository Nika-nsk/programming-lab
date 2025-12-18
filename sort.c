void BubbleSort (int A[], int n, int *C, int *M) {
    *C = 0;
    *M = 0;
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j<n - i; J++) {
            (*C)++;
            if (A[j] > A[j+1]) {
                int temp = A[j];
                A[j] = A[j+1];
                A[j+1] = temp;
                (*M)+=3;
            }
        }
    }
}