#include "AVL.h"

// functions to compare, print, and free int data
int int_compare(const void *a, const void *b)
{
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

void int_print(const void *data)
{
    printf("%d", *(const int *)data);
}

void int_free(void *data)
{
    free(data);
}

int *create_int(int value)
{
    int *ptr = malloc(sizeof(int));
    if (!ptr)
    {
        perror("Failed to allocate memory for int");
        exit(EXIT_FAILURE);
    }
    *ptr = value;
    return ptr;
}

// test result statistics
typedef struct
{
    int total_tests;
    int passed_tests;
    int failed_tests;
} TestResult;

TestResult test_result = {0, 0, 0};

// self-validation macro
#define ASSERT(condition, message)           \
    do                                       \
    {                                        \
        test_result.total_tests++;           \
        if (condition)                       \
        {                                    \
            test_result.passed_tests++;      \
            printf("✓ PASS: %s\n", message); \
        }                                    \
        else                                 \
        {                                    \
            test_result.failed_tests++;      \
            printf("✗ FAIL: %s\n", message); \
        }                                    \
    } while (0)

// validate AVL tree integrity
bool validateAVL(AVLNode *root, const char *context)
{
    bool isBST = isValidBST(root, NULL, NULL, int_compare);
    bool isAVL = isValidAVL(root);
    bool valid = isBST && isAVL;

    ASSERT(valid, context);
    if (!valid)
        printf("   BST valid: %s, AVL valid: %s\n",
               isBST ? "Yes" : "No", isAVL ? "Yes" : "No");
    return valid;
}

// basic operations test
void testBasicOperations()
{
    printf("\n=== Basic Operations Test ===\n");
    AVLNode *root = NULL;

    // insert test
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(values) / sizeof(values[0]);

    for (int i = 0; i < n; i++)
        root = insert(root, create_int(values[i]), int_compare);

    validateAVL(root, "AVL tree validity after insertion");
    ASSERT(getTreeSize(root) == n, "Tree size correct");

    // demonstrate tree visualization
    printf("   Tree structure:\n");
    printAVL(root, "   ", true, int_print);

    // search test
    int search_val_50 = 50;
    int search_val_100 = 100;
    ASSERT(search(root, &search_val_50, int_compare) != NULL, "Search existing node (50)");
    ASSERT(search(root, &search_val_100, int_compare) == NULL, "Search non-existing node (100)");

    // delete test
    int delete_val = 20;
    root = delete(root, &delete_val, int_compare, int_free);
    validateAVL(root, "AVL tree validity after deletion");
    ASSERT(search(root, &delete_val, int_compare) == NULL, "Search verification after deletion");
    ASSERT(getTreeSize(root) == n - 1, "Tree size correct after deletion");

    freeAVLTree(root, int_free);
}

// rotation test
void testRotations()
{
    printf("\n=== Rotation Test ===\n");

    // test right rotation (LL case)
    AVLNode *root1 = NULL;
    root1 = insert(root1, create_int(30), int_compare);
    root1 = insert(root1, create_int(20), int_compare);
    root1 = insert(root1, create_int(10), int_compare);
    ASSERT(*(int *)root1->data == 20, "Root node correct after LL rotation");
    validateAVL(root1, "AVL validity after LL rotation");

    // test left rotation (RR case)
    AVLNode *root2 = NULL;
    root2 = insert(root2, create_int(10), int_compare);
    root2 = insert(root2, create_int(20), int_compare);
    root2 = insert(root2, create_int(30), int_compare);
    ASSERT(*(int *)root2->data == 20, "Root node correct after RR rotation");
    validateAVL(root2, "AVL validity after RR rotation");

    freeAVLTree(root1, int_free);
    freeAVLTree(root2, int_free);
}

// edge cases test
void testEdgeCases()
{
    printf("\n=== Edge Cases Test ===\n");

    // null tree operations
    AVLNode *nullRoot = NULL;
    int search_val = 10;
    ASSERT(search(nullRoot, &search_val, int_compare) == NULL, "NULL tree search");
    ASSERT(getTreeSize(nullRoot) == 0, "NULL tree size");
    ASSERT(isValidAVL(nullRoot) == true, "NULL tree AVL validity");

    // single node tree
    AVLNode *singleRoot = insert(NULL, create_int(42), int_compare);
    ASSERT(*(int *)singleRoot->data == 42, "Single node tree creation");
    ASSERT(getTreeSize(singleRoot) == 1, "Single node tree size");
    validateAVL(singleRoot, "Single node tree AVL validity");

    // duplicate insertion
    AVLNode *beforeDup = singleRoot;
    int *dup_val = create_int(42);
    singleRoot = insert(singleRoot, dup_val, int_compare);
    ASSERT(beforeDup == singleRoot, "Duplicate insertion does not change tree structure");
    ASSERT(getTreeSize(singleRoot) == 1, "Size unchanged after duplicate insertion");

    free(dup_val);

    freeAVLTree(singleRoot, int_free);
}

// stress test
#define STRESS_TEST_SIZE 10000000

void testStress()
{
    printf("\n=== Stress Test ===\n");
    AVLNode *root = NULL;

    // insert 1-STRESS_TEST_SIZE sequentially
    for (int i = 1; i <= STRESS_TEST_SIZE; i++)
        root = insert(root, create_int(i), int_compare);

    ASSERT(getTreeSize(root) == STRESS_TEST_SIZE, "Tree size correct after large insertion");
    validateAVL(root, "AVL validity after large insertion");

    // delete all odd numbers
    int deletedCount = 0;
    for (int i = 1; i <= STRESS_TEST_SIZE; i += 2)
    {
        AVLNode *before = search(root, &i, int_compare);
        if (before)
        {
            root = delete(root, &i, int_compare, int_free);
            deletedCount++;
        }
    }

    int expectedSize = STRESS_TEST_SIZE - deletedCount;
    int actualSize = getTreeSize(root);
    printf("   Deleted %d nodes, expected size: %d, actual size: %d\n",
           deletedCount, expectedSize, actualSize);
    ASSERT(actualSize == expectedSize, "Tree size correct after large deletion");
    validateAVL(root, "AVL validity after large deletion");

    freeAVLTree(root, int_free);
}

// utility functions test
void testUtilities()
{
    printf("\n=== Utility Functions Test ===\n");

    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(values) / sizeof(values[0]);

    // create array of int pointers
    void *arr[10];
    for (int i = 0; i < size; i++)
    {
        arr[i] = create_int(values[i]);
    }

    AVLNode *root = createAVLFromArray(arr, size, int_compare);
    ASSERT(root != NULL, "Create tree from array");
    ASSERT(getTreeSize(root) == size, "Tree size correct from array creation");
    validateAVL(root, "AVL validity of tree created from array");

    // test minimum and maximum values
    AVLNode *minNode = findMin(root);
    AVLNode *maxNode = findMax(root);
    ASSERT(minNode && *(int *)minNode->data == 1, "Find minimum value");
    ASSERT(maxNode && *(int *)maxNode->data == 10, "Find maximum value");

    freeAVLTree(root, int_free);
}

// print test result summary
void printTestSummary()
{
    printf("\n=== Test Result Summary ===\n");
    printf("Total tests: %d\n", test_result.total_tests);
    printf("Passed: %d\n", test_result.passed_tests);
    printf("Failed: %d\n", test_result.failed_tests);
    printf("Success rate: %.1f%%\n",
           test_result.total_tests > 0 ? (100.0 * test_result.passed_tests / test_result.total_tests) : 0.0);

    if (test_result.failed_tests == 0)
        printf("🎉 All tests passed!\n");
    else
        printf("❌ %d tests failed\n", test_result.failed_tests);
}

int main()
{
    printf("AVL Tree Test Program\n");
    printf("================================\n");

    // run all tests
    testBasicOperations();
    testRotations();
    testEdgeCases();
    testStress();
    testUtilities();

    // print result summary
    printTestSummary();

    return test_result.failed_tests == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
