#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// If LINKED_LIST is defined, then we need to undefined SIMPLIFIED (vice versa)
// #define SIMPLIFIED
#define LINKED_LIST

#ifdef SIMPLIFIED
char stack[100];
int top = -1;

int push(char c)
{
    if (top == 100)
        return -1; // Check for stack overflow
    stack[++top] = c;
}

char pop()
{
    if (top == -1)
        return -1; // Check for stack underflow
    else
        return stack[top--];
}
#endif

#ifdef LINKED_LIST
// Define structures
typedef struct node
{
    int data;
    struct node *link;
} Node;

typedef struct head
{
    int count;
    Node *top;
} Head;

// Function declarations
Head *createstack()
{
    Head *ptr = malloc(sizeof(Head));
    ptr->count = 0;
    ptr->top = NULL;

    return ptr;
}

int push(Head *ptr, char data)
{
    if (ptr->count == 100)
        return -1; // Check if stack is full

    // Everytime we push, we create a new node
    Node *node = malloc(sizeof(Node));
    node->data = data;
    node->link = ptr->top; // Link the new node to the top of the stack

    ptr->top = node; // Update the top of the stack
    ptr->count++;    // Update the count of the stack
}

char pop(Head *ptr)
{
    if (ptr->count == 0)
        return -1; // Check if the stack is empty

    // Create a temporary node to point to the top of the stack
    Node *node = ptr->top; // Copy the top of the stack to a temporary node

    char c = node->data;   // Return the data of the node
    ptr->top = node->link; // Update the top of the stack to the next node
    ptr->count--;          // Update the count of the stack
    if (ptr->count == 0)
        ptr->top = NULL; // Update the top of the stack to NULL if the stack is empty

    free(node); // Free the temporary node
    return c;
}

char stackTop(Head *ptr, char data)
{
    if (ptr->count == 0)
        return -1; // Check if the stack is empty
    else
        return ptr->top->data; // Return the data of the node
}
#endif

int main()
{

    // Open file
    FILE *in = fopen("input_hw2.txt", "r");
    FILE *out = fopen("output2.txt", "w");
    if (in == NULL)
        perror("Failed: ");

// Create stack & variables
#ifdef LINKED_LIST
    Head *stack = createstack();
#endif
    char data, out_data; // Read data and output data
    int cases = 0, operand = 0;
    fscanf(in, "%d", &cases); // Read the first line for the number of cases

    // Loop through the cases
    printf("%d", cases);
    for (int i = 0; i < cases; i++)
    {
        fscanf(in, "%d", &operand); // Read the first line for the number of operands

        while (1)
        {
            fscanf(in, "%c", &data); // Read the character

#ifdef LINKED_LIST
            if (data == '(')
            {
                if (push(stack, data) == -1)
                    printf("Failed: Stack is full\n");
            }
            else if (data == ')')
            {
                out_data = pop(stack);
                if (out_data == -1)
                    printf("Failed: Stack is empty\n");
                while (out_data != '(')
                {
                    fprintf(out, "%c", out_data);
                    out_data = pop(stack);
                }
            }
            else if (data == '+' || data == '-' || data == '*' || data == '/')
            {
                if (push(stack, data) == -1)
                    printf("Failed: Stack is full\n");
                // Priority test for operator (Not included for now)
            }
            else if (data == '\n')
            {
                continue; // Ignore newline
            }
            else
            {
                fprintf(out, "%c", data);
                operand--;
            }

            if (stack->count == 0 && operand == 0)
            {
                fprintf(out, "\n");
                break; // Break if operand is 0 (This also apply to EOF)
            }
#endif

#ifdef SIMPLIFIED
            if (data == '(')
            {
                if (push(data) == -1)
                    printf("Stack overflow\n");
            }
            else if (data == ')')
            {
                out_data = pop();
                if (out_data == -1)
                    printf("Stack underflow\n");
                while (out_data != '(')
                {
                    fprintf(out, "%c", out_data);
                    out_data = pop();
                }
            }
            else if (data == '+' || data == '-' || data == '*' || data == '/')
            {
                if (push(data) == -1)
                    printf("Stack overflow\n");
            }
            else if (data == '\n')
            {
                continue;
            }
            else
            {
                fprintf(out, "%c", data);
                operand--;
            }

            if (top == -1 && operand == 0)
            {
                fprintf(out, "\n");
                break;
            }
#endif
        }
    }

// Close file and free memory
#ifdef LINKED_LIST
    free(stack);
#endif
    fclose(in);
    fclose(out);

    return 0;
}