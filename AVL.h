#ifndef AVL_H
#define AVL_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <string.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))

// AVL tree constants
#define AVL_MAX_BALANCE 1

// function pointer types for generic operations
typedef int (*compare_func_t)(const void *a, const void *b);
typedef void (*print_func_t)(const void *data);
typedef void (*free_func_t)(void *data);

// define AVL node structure
typedef struct AVLNode
{
    void *data;            // pointer to data stored in node
    struct AVLNode *left;  // pointer to left child
    struct AVLNode *right; // pointer to right child
    int height;            // height of this node
    int size;              // number of nodes in subtree rooted at this node
} AVLNode;

// basic operations
int getHeight(const AVLNode *node);
int getSize(const AVLNode *node);
int getBalance(const AVLNode *node);
void updateHeight(AVLNode *node);
void updateSize(AVLNode *node);
AVLNode *createNode(void *data);

// rotation operations
AVLNode *rotateRight(AVLNode *node);
AVLNode *rotateLeft(AVLNode *node);
AVLNode *rebalance(AVLNode *node);

// tree operations
AVLNode *insert(AVLNode *node, void *data, compare_func_t compare);
AVLNode *delete(AVLNode *node, void *data, compare_func_t compare, free_func_t free_data);
AVLNode *search(AVLNode *node, void *data, compare_func_t compare);
AVLNode *findMin(AVLNode *node);
AVLNode *findMax(AVLNode *node);
AVLNode *createAVLFromArray(void *arr[], int size, compare_func_t compare);

// utility functions
void printAVL(const AVLNode *root, const char *prefix, bool isLast, print_func_t print_data);
void inorderTraversal(const AVLNode *root, print_func_t print_data);
void preorderTraversal(const AVLNode *root, print_func_t print_data);
void postorderTraversal(const AVLNode *root, print_func_t print_data);
void freeAVLTree(AVLNode *root, free_func_t free_data);

// validation functions
bool isValidBST(const AVLNode *root, void *minVal, void *maxVal, compare_func_t compare);
bool isValidAVL(const AVLNode *root);

#endif // AVL_H
