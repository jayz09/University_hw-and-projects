#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// #define DEBUG

// Function declaration and global variable
void hanoi(int n, char a, char b, char c);
int cost = 0;

int main() {

    // Open file
    FILE *in = fopen("input_hw1.txt", "r");
    FILE *out = fopen("output.txt", "w");
    if (in == NULL) perror("Failed: ");
    if (out == NULL) perror("Failed: ");

    // Read and store file
    int size = 0, i = 0;
    fscanf(in, "%d", &size);                                // Read the 1st one, for how many cases
    int *arr = (int*)malloc(sizeof(int) * size);
    if (arr == NULL) perror("Failed: ");

    while (1) {
        int ret = fscanf(in, "%d", &arr[i]);
        if (ret != 1) break;                                // Break if EOF
        i++;
    }

    #ifdef DEBUG
    printf("Size : %d cases\n", size);
    for (int i = 0; i < size; i++) {
        printf("%d  ", arr[i]);
    }
    printf("\n----------------------------------------------------------------------\n");
    #endif

    // Loop
    for (int i = 0; i < size; i++) {
        hanoi(arr[i], 'a', 'c', 'b');                       // a(source) --> c(destination), using b(auxiliary)
        
        // Write the cost
        #ifdef DEBUG
        printf("%d\n", cost);
        #endif

        if (i == (size-1)) {
            fprintf(out, "%d", cost);
        } else {
            fprintf(out, "%d\n", cost);
        }
        cost = 0;
    }
    
    // Close file and free memory
    free(arr);
    arr = NULL;

    fclose(in);
    fclose(out);

    return 0;
}

void hanoi(int n, char a, char b, char c) {                 //a as "src", b as "dst", c as "aux"
    // Base Case
    if (n == 0) return;

    // General Case
    hanoi(n-1, a, c ,b);

    #ifdef DEBUG
    printf("%c(disk %d) --> %c\t\tCost = %d\n", a, n, b, n);
    #endif

    hanoi(n-1, c, b, a);

    // Count the cost
    cost += n;
}