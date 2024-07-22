#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "header.h"

// #define DEBUG

int main() {

    // Open file
    FILE *input = fopen("hw3_input.txt", "r");
    FILE *output = fopen("output2.txt", "w");
    if (input == NULL) {
        perror("Failed");
        return -1;
    }

    // Variables
    Node *tree = NULL;
    int cases = 0;
    int sum = 0;
    int num_sum = 0;
    char str[10];
    char line[MAX_LENGTH];

    // Get the number of cases from the 1st line in text file
    fscanf(input, "%d", &cases);

    #ifdef DEBUG
    printf("DEBUG -> Number of cases: %d\n", cases);
    #endif

    // Loop through each cases
    for (int i = 0; i < cases; i++) {

        // Get the instruction from 1st line
        if (fscanf(input, "%s %d", str, &num_sum) == 2) {
            #ifdef DEBUG
            printf("DEBUG -> %s %d\n", str, num_sum);
            #endif
            fgetc(input); // Consume the newline
        } else {
            perror("Failed");
        }
        
        // Get the data to make the tree from 2nd line
        if (fgets(line, sizeof(line), input) != NULL) {
            line[strcspn(line, "\n")] = '\0';
            printf("Data  -> %s\n", line);
        } else {
            perror("Failed");
        }
        
        // Store the data in tree
        char *token = strtok(line, " \n");
        while (token != NULL) {
            Node *temp = createNode(atoi(token));
            
            #ifdef DEBUG
            printf("DEBUG -> %s\t", token);
            #endif
            
            tree = addBST(tree, temp);
            token = strtok(NULL, " \n");
        }

        // Check the tree using BFS
        #ifdef DEBUG 
        BST(tree);
        #endif

        // BFS or DFS instructions
        if (strcmp(str, "BFS:") == 0) {

            printf("BFS   -> True(%d)\n", num_sum);
            BFS(tree, &sum, &num_sum);
            printf("Total sum: %d\n\n", sum);

        } else if (strcmp(str, "DFS:") == 0) {

            printf("DFS   -> True(%d)\n", num_sum);
            preOrder(tree, &sum, &num_sum);
            printf("Total sum: %d\n\n", sum);
        
        } else {
            printf("Error : Unknown command\n");
        }

        // Write to output file
        fprintf(output, "%d\n", sum);

        // Reset the tree and all variables
        memset(q, 0, sizeof(Node));
        front, rear = -1;
        tree = NULL;
        sum = 0;
        num_sum = 0; 
        memset(str, 0, sizeof(str));
        memset(line, 0, sizeof(str));  
    }
    
    // Close file
    fclose(input);
    fclose(output);
    
    return 0;
}