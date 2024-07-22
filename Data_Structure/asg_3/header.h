#define MAX_NODES 50
#define MAX_LENGTH 200

// Leaf Nodes
typedef struct node {
    int value;
    struct node *left;
    struct node *right;
}Node;

// Queue
Node *q[100];
int front, rear = -1;


// Function declarations
Node *createNode(int value);
Node *addBST(Node *root, Node *newNode);
void enq(Node *n);
Node *deq();
void BST(Node *root);
void preOrder(Node *root, int *sum, int *num_sum);
void BFS(Node *root, int *sum, int *num_sum);

// Function body
Node *createNode(int value) {
    Node *n = malloc(sizeof(Node));
    n->value = value;
    n->right = n->left = NULL;
    return n;
}

Node *addBST(Node *root, Node *newNode) {
    if (root == NULL) {
        root = newNode;
        return root;
    }
    if (newNode->value < root->value) {
        root->left = addBST(root->left, newNode);
    } else {
        root->right = addBST(root->right, newNode);
    }
    return root;
}

void enq(Node *n) {
    if (front == -1) front++;
    q[++rear] = n;
}   

Node* deq() {
    Node *n = q[front++];
    if (front > rear) front = rear = -1;
    return n;
}

void BST(Node *root) {
    Node *current = root;
    printf("BST   -> ");
    while (current != NULL) {
        printf("%d\t", current->value);
        if (current->left != NULL) enq(current->left);
        if (current->right != NULL) enq(current->right);        
        if (front != -1) current = deq();
        else current = NULL;
    }
    printf("\n");
}

void BFS(Node *root, int *sum, int *num_sum) {
    Node *current = root;
    while (current != NULL) {
        printf("%d --> ", current->value);
        if (*num_sum != 0) {
            *sum += current->value;
            (*num_sum)--;
        } 
        if (current->left != NULL) enq(current->left);
        if (current->right != NULL) enq(current->right);        
        if (front != -1) current = deq();
        else current = NULL;
    }
}

void preOrder(Node *root, int *sum, int *num_sum) {
    if (root == NULL) return;
    printf("%d --> ", root->value);
    if (*num_sum != 0) {
        *sum += root->value;
        (*num_sum)--;
    }
    preOrder(root->left, sum, num_sum);
    preOrder(root->right, sum, num_sum);
}