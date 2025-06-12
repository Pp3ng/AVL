#include "AVL.h"

// === Data Helper Functions ===
static int int_compare(const void *a, const void *b)
{
    int ia = *(const int *)a, ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

static void int_free(void *data) { free(data); }

static int *create_int(int value)
{
    int *ptr = malloc(sizeof(int));
    if (!ptr)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    *ptr = value;
    return ptr;
}

// === Test Framework ===
typedef struct
{
    int total, passed, failed;
} TestStats;
static TestStats stats = {0};

#define TEST(name)                       \
    static void test_##name(void);       \
    static void run_##name(void)         \
    {                                    \
        printf("\n--- %s ---\n", #name); \
        test_##name();                   \
    }                                    \
    static void test_##name(void)

#define ASSERT(condition, message)     \
    do                                 \
    {                                  \
        stats.total++;                 \
        if (condition)                 \
        {                              \
            stats.passed++;            \
            printf("✓ %s\n", message); \
        }                              \
        else                           \
        {                              \
            stats.failed++;            \
            printf("✗ %s\n", message); \
        }                              \
    } while (0)

#define RUN_TEST(name) run_##name()

// === Utility Functions ===
static void **create_int_array(int values[], int size)
{
    void **arr = malloc(size * sizeof(void *));
    if (!arr)
    {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < size; i++)
        arr[i] = create_int(values[i]);
    return arr;
}

static bool validate_avl(AVLNode *root, const char *context)
{
    bool is_bst = isValidBST(root, NULL, NULL, int_compare);
    bool is_avl = isValidAVL(root);
    bool valid = is_bst && is_avl;
    ASSERT(valid, context);
    if (!valid)
        printf("   BST: %s, AVL: %s\n", is_bst ? "✓" : "✗", is_avl ? "✓" : "✗");
    return valid;
}

// === Core Operation Tests ===
TEST(basic_operations)
{
    AVLNode *root = NULL;
    int values[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(values) / sizeof(values[0]);

    // Insert operations
    for (int i = 0; i < n; i++)
        root = insert(root, create_int(values[i]), int_compare);

    validate_avl(root, "Tree validity after insertions");
    ASSERT(getSize(root) == n, "Tree size correct after insertions");

    // Search operations
    int existing = 50, non_existing = 100;
    ASSERT(search(root, &existing, int_compare) != NULL, "Search existing value");
    ASSERT(search(root, &non_existing, int_compare) == NULL, "Search non-existing value");

    // Delete operation
    int delete_val = 20;
    root = delete(root, &delete_val, int_compare, int_free);
    validate_avl(root, "Tree validity after deletion");
    ASSERT(search(root, &delete_val, int_compare) == NULL, "Deleted value not found");
    ASSERT(getSize(root) == n - 1, "Tree size correct after deletion");

    freeAVLTree(root, int_free);
}

TEST(rotations)
{
    // LL rotation test
    AVLNode *root1 = NULL;
    int ll_values[] = {30, 20, 10};
    for (int i = 0; i < 3; i++)
        root1 = insert(root1, create_int(ll_values[i]), int_compare);
    ASSERT(*(int *)root1->data == 20, "LL rotation: correct root");
    validate_avl(root1, "LL rotation: tree validity");

    // RR rotation test
    AVLNode *root2 = NULL;
    int rr_values[] = {10, 20, 30};
    for (int i = 0; i < 3; i++)
        root2 = insert(root2, create_int(rr_values[i]), int_compare);
    ASSERT(*(int *)root2->data == 20, "RR rotation: correct root");
    validate_avl(root2, "RR rotation: tree validity");

    freeAVLTree(root1, int_free);
    freeAVLTree(root2, int_free);
}

TEST(edge_cases)
{
    // Null tree operations
    AVLNode *null_root = NULL;
    int val = 10;
    ASSERT(search(null_root, &val, int_compare) == NULL, "NULL tree search");
    ASSERT(getSize(null_root) == 0, "NULL tree size");
    ASSERT(isValidAVL(null_root) == true, "NULL tree validity");

    // Single node operations
    AVLNode *single = insert(NULL, create_int(42), int_compare);
    ASSERT(*(int *)single->data == 42, "Single node creation");
    ASSERT(getSize(single) == 1, "Single node size");
    validate_avl(single, "Single node validity");

    // Duplicate insertion test
    AVLNode *before = single;
    int *dup = create_int(42);
    single = insert(single, dup, int_compare);
    ASSERT(before == single, "Duplicate insertion: no structural change");
    ASSERT(getSize(single) == 1, "Duplicate insertion: size unchanged");
    free(dup);

    freeAVLTree(single, int_free);
}

TEST(stress_performance)
{
    const int size = 10000000;
    AVLNode *root = NULL;

    // Sequential insertion test
    for (int i = 1; i <= size; i++)
        root = insert(root, create_int(i), int_compare);

    ASSERT(getSize(root) == size, "Stress test: correct size after insertions");
    validate_avl(root, "Stress test: tree validity after insertions");

    // Delete odd numbers
    int deleted_count = 0;
    for (int i = 1; i <= size; i += 2)
    {
        if (search(root, &i, int_compare))
        {
            root = delete(root, &i, int_compare, int_free);
            deleted_count++;
        }
    }

    int expected_size = size - deleted_count;
    ASSERT(getSize(root) == expected_size, "Stress test: correct size after deletions");
    validate_avl(root, "Stress test: tree validity after deletions");

    freeAVLTree(root, int_free);
}

TEST(utility_functions)
{
    int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int size = sizeof(values) / sizeof(values[0]);
    void **arr = create_int_array(values, size);

    // Array to tree conversion
    AVLNode *root = createAVLFromArray(arr, size, int_compare);
    ASSERT(root != NULL, "Array to tree conversion");
    ASSERT(getSize(root) == size, "Array conversion: correct size");
    validate_avl(root, "Array conversion: tree validity");

    // Min/Max finding
    AVLNode *min_node = findMin(root);
    AVLNode *max_node = findMax(root);
    ASSERT(min_node && *(int *)min_node->data == 1, "Find minimum value");
    ASSERT(max_node && *(int *)max_node->data == 10, "Find maximum value");

    freeAVLTree(root, int_free);
    free(arr);
}

// Range query callback structure
typedef struct
{
    int *results;
    int count;
    int capacity;
} RangeResult;

static void range_callback(const void *data, void *context)
{
    RangeResult *result = (RangeResult *)context;
    if (result->count < result->capacity)
        result->results[result->count++] = *(const int *)data;
}

TEST(queries)
{
    int values[] = {50, 30, 70, 20, 40, 60, 80, 10, 25, 35, 45, 55, 65, 75, 90, 100};
    int size = sizeof(values) / sizeof(values[0]);
    void **arr = create_int_array(values, size);
    AVLNode *root = createAVLFromArray(arr, size, int_compare);

    validate_avl(root, "Query test tree validity");

    // Range query test
    RangeResult range_result = {0};
    range_result.results = malloc(16 * sizeof(int));
    range_result.capacity = 16;
    int min_val = 30, max_val = 70;
    rangeQuery(root, &min_val, &max_val, int_compare, range_callback, &range_result);

    ASSERT(range_result.count > 0, "Range query found values");

    // Count range test
    int count = countRange(root, &min_val, &max_val, int_compare);
    ASSERT(count == range_result.count, "Count range matches query result");

    // Kth element tests
    AVLNode *k3_smallest = findKthSmallest(root, 3);
    AVLNode *k3_largest = findKthLargest(root, 3);
    ASSERT(k3_smallest != NULL, "Find 3rd smallest element");
    ASSERT(k3_largest != NULL, "Find 3rd largest element");

    // Rank test
    int search_val = 50;
    int rank = getRank(root, &search_val, int_compare);
    ASSERT(rank > 0, "Get rank of existing element");

    // Edge cases
    ASSERT(findKthSmallest(root, 0) == NULL, "Invalid k (0) returns NULL");
    ASSERT(findKthSmallest(root, size + 1) == NULL, "Invalid k (too large) returns NULL");

    int non_existent = 999;
    ASSERT(getRank(root, &non_existent, int_compare) == 0, "Non-existent element rank is 0");

    free(range_result.results);
    freeAVLTree(root, int_free);
    free(arr);
}

// === Test Summary and Main ===
static void print_summary(void)
{
    printf("\n=== Test Summary ===\n");
    printf("Total: %d | Passed: %d | Failed: %d\n",
           stats.total, stats.passed, stats.failed);
    printf("Success Rate: %.1f%%\n",
           stats.total > 0 ? (100.0 * stats.passed / stats.total) : 0.0);

    if (stats.failed == 0)
        printf("🎉 All tests passed!\n");
    else
        printf("❌ %d test(s) failed\n", stats.failed);
}

int main(void)
{
    printf("AVL Tree Test Suite\n");
    printf("===================\n");

    // Run all test suites
    RUN_TEST(basic_operations);
    RUN_TEST(rotations);
    RUN_TEST(edge_cases);
    RUN_TEST(stress_performance);
    RUN_TEST(utility_functions);
    RUN_TEST(queries);

    // Print final results
    print_summary();

    return stats.failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
