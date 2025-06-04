#include "AVL.h"

// get node height
int getHeight(const AVLNode *node)
{
    return node ? node->height : 0;
}

// get balance factor
int getBalance(const AVLNode *node)
{
    return node ? getHeight(node->left) - getHeight(node->right) : 0;
}

// update node height
void updateHeight(AVLNode *node)
{
    if (!node)
        return;
    node->height = 1 + MAX(getHeight(node->left), getHeight(node->right));
}

// create a new AVL node
AVLNode *createNode(void *data)
{
    AVLNode *node = malloc(sizeof(AVLNode));
    if (!node)
    {
        perror("Failed to allocate memory for AVLNode");
        exit(EXIT_FAILURE);
    }

    node->data = data;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

// right rotation
AVLNode *rotateRight(AVLNode *node)
{
    if (!node || !node->left)
        return node;

    AVLNode *pivot = node->left;
    node->left = pivot->right;
    pivot->right = node;

    updateHeight(node);
    updateHeight(pivot);
    return pivot;
}

// left rotation
AVLNode *rotateLeft(AVLNode *node)
{
    if (!node || !node->right)
        return node;

    AVLNode *pivot = node->right;
    node->right = pivot->left;
    pivot->left = node;

    updateHeight(node);
    updateHeight(pivot);
    return pivot;
}

// rebalance AVL tree after insertion or deletion
AVLNode *rebalance(AVLNode *node)
{
    if (!node)
        return node;

    updateHeight(node);
    int balance = getBalance(node);

    // left-left case
    if (balance > 1 && getBalance(node->left) >= 0)
        return rotateRight(node);

    // left-right case
    if (balance > 1 && getBalance(node->left) < 0)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // right-right case
    if (balance < -1 && getBalance(node->right) <= 0)
        return rotateLeft(node);

    // right-left case
    if (balance < -1 && getBalance(node->right) > 0)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node; // no rotation needed
}

// insert and keep balance
AVLNode *insert(AVLNode *node, void *data, compare_func_t compare)
{
    // 1. standard BST insertion
    if (!node)
        return createNode(data);

    int cmp = compare(data, node->data);
    if (cmp < 0)
        node->left = insert(node->left, data, compare);
    else if (cmp > 0)
        node->right = insert(node->right, data, compare);
    else
        return node; // no duplicates allowed

    return rebalance(node);
}

// search for a key in the AVL tree
AVLNode *search(AVLNode *node, void *data, compare_func_t compare)
{
    if (!node)
        return NULL;

    int cmp = compare(data, node->data);
    if (cmp == 0)
        return node;
    else if (cmp < 0)
        return search(node->left, data, compare);
    else
        return search(node->right, data, compare);
}

// find minimum node in a subtree
AVLNode *findMin(AVLNode *node)
{
    if (!node)
        return NULL;

    while (node->left)
        node = node->left;

    return node;
}

// delete a node and keep balance
AVLNode *delete(AVLNode *node, void *data, compare_func_t compare, free_func_t free_data)
{
    // 1. standard BST deletion
    if (!node)
        return node;

    int cmp = compare(data, node->data);
    if (cmp < 0)
        node->left = delete(node->left, data, compare, free_data);
    else if (cmp > 0)
        node->right = delete(node->right, data, compare, free_data);
    else
    {
        // node to be deleted found
        if (!node->left || !node->right)
        {
            // node with only one child or no child
            AVLNode *temp = node->left ? node->left : node->right;

            if (!temp)
            {
                // no child case
                if (free_data)
                    free_data(node->data);
                free(node);
                return NULL;
            }
            else
            {
                // one child case: replace node with its child
                if (free_data)
                    free_data(node->data);
                free(node);
                return temp;
            }
        }
        else
        {
            // node with two children: get the inorder successor
            AVLNode *temp = findMin(node->right);

            // copy the inorder successor's data to this node
            void *temp_data = temp->data; // save the successor's data pointer
            if (free_data)
                free_data(node->data); // free current node's data
            node->data = temp_data;    // assign successor's data to current node

            // delete the inorder successor(data is already moved)
            node->right = delete(node->right, temp->data, compare, NULL);
        }
    }

    return rebalance(node);
}

// create AVL tree from array
AVLNode *createAVLFromArray(void *arr[], int size, compare_func_t compare)
{
    if (!arr || size <= 0)
        return NULL;

    AVLNode *root = NULL;
    for (int i = 0; i < size; i++)
        root = insert(root, arr[i], compare);
    return root;
}

void printAVL(const AVLNode *root, const char *prefix, bool isLast, print_func_t print_data)
{
    if (!root)
        return;

    printf("%s%s", prefix, isLast ? "└── " : "├── ");
    print_data(root->data);
    printf("[h:%d,b:%+d]\n", root->height, getBalance(root));

    if (root->left || root->right)
    {
        char newPrefix[256];
        snprintf(newPrefix, sizeof(newPrefix), "%s%s", prefix, isLast ? "    " : "│   ");

        if (root->right)
            printAVL(root->right, newPrefix, root->left == NULL, print_data);
        if (root->left)
            printAVL(root->left, newPrefix, true, print_data);
    }
}

// inorder traversal
void inorderTraversal(const AVLNode *root, print_func_t print_data)
{
    if (!root)
        return;

    inorderTraversal(root->left, print_data);
    print_data(root->data);
    printf(" ");
    inorderTraversal(root->right, print_data);
}

// preorder traversal
void preorderTraversal(const AVLNode *root, print_func_t print_data)
{
    if (!root)
        return;

    print_data(root->data);
    printf(" ");
    preorderTraversal(root->left, print_data);
    preorderTraversal(root->right, print_data);
}

// postorder traversal
void postorderTraversal(const AVLNode *root, print_func_t print_data)
{
    if (!root)
        return;

    postorderTraversal(root->left, print_data);
    postorderTraversal(root->right, print_data);
    print_data(root->data);
    printf(" ");
}

// get tree size (number of nodes)
int getTreeSize(const AVLNode *root)
{
    if (!root)
        return 0;

    return 1 + getTreeSize(root->left) + getTreeSize(root->right);
}

// find maximum node in a subtree
AVLNode *findMax(AVLNode *node)
{
    if (!node)
        return NULL;

    while (node->right)
        node = node->right;

    return node;
}

// free AVL tree
void freeAVLTree(AVLNode *root, free_func_t free_data)
{
    if (!root)
        return;

    freeAVLTree(root->left, free_data);
    freeAVLTree(root->right, free_data);
    if (free_data)
        free_data(root->data);
    free(root);
}

// validate if tree is a valid binary search tree
bool isValidBST(const AVLNode *root, void *minVal, void *maxVal, compare_func_t compare)
{
    if (!root)
        return true;

    if ((minVal && compare(root->data, minVal) <= 0) ||
        (maxVal && compare(root->data, maxVal) >= 0))
        return false;

    return isValidBST(root->left, minVal, root->data, compare) &&
           isValidBST(root->right, root->data, maxVal, compare);
}

// validate if tree is a valid AVL tree
bool isValidAVL(const AVLNode *root)
{
    if (!root)
        return true;

    // check balance factor
    int balance = getBalance(root);
    if (ABS(balance) > 1)
        return false;

    // check height consistency
    int expectedHeight = 1 + MAX(getHeight(root->left), getHeight(root->right));
    if (root->height != expectedHeight)
        return false;

    // recursively check subtrees
    return isValidAVL(root->left) && isValidAVL(root->right);
}
