# AVL Tree Implementation in C

An **AVL (Adelson-Velsky and Landis) Tree** implementation in C that performs self-balancing operations to maintain the AVL property, ensuring O(log n) time complexity for insertion, deletion, search and traversal operations.

## Usage

This AVL tree implementation is **fully generic** that can store any data type. You need to provide three function pointers for proper operation:

1. **compare_func_t**: For comparing two data elements
2. **print_func_t**: For printing data elements (optional, for visualization)
3. **free_func_t**: For freeing memory of data elements

### Basic Example with Integers

```c
#include "AVL.h"

// Helper functions for int data type
int int_compare(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    return (ia > ib) - (ia < ib);
}

void int_print(const void *data) {
    printf("%d", *(const int *)data);
}

void int_free(void *data) {
    free(data);
}

int *create_int(int value) {
    int *ptr = malloc(sizeof(int));
    if (!ptr) {
        perror("Failed to allocate memory for int");
        exit(EXIT_FAILURE);
    }
    *ptr = value;
    return ptr;
}

int main() {
    AVLNode *root = NULL;

    // insert some integers into the AVL tree
    root = insert(root, create_int(50), int_compare);
    root = insert(root, create_int(30), int_compare);
    root = insert(root, create_int(70), int_compare);
    root = insert(root, create_int(20), int_compare);
    root = insert(root, create_int(40), int_compare);

    // search a value
    int search_value = 30;
    AVLNode *found = search(root, &search_value, int_compare);
    if (found) {
        printf("Found: %d\n", *(int*)found->data);
    }

    // tree traversals
    printf("Inorder traversal: ");
    inorderTraversal(root, int_print);
    printf("\n");

    // get tree information
    printf("Tree size: %d nodes\n", getSize(root));
    printf("Tree height: %d\n", getHeight(root));

    // find minimum and maximum values
    AVLNode *minNode = findMin(root);
    AVLNode *maxNode = findMax(root);
    if (minNode) printf("Minimum value: %d\n", *(int*)minNode->data);
    if (maxNode) printf("Maximum value: %d\n", *(int*)maxNode->data);

    // validate tree
    if (isValidAVL(root) && isValidBST(root, NULL, NULL, int_compare)) {
        printf("Tree is a valid AVL tree\n");
    }

    // visualize the tree
    printf("Tree structure:\n");
    printAVL(root, "", true, int_print);

    // clean up memory
    freeAVLTree(root, int_free);
    return 0;
}
```

### Example with Custom Data Types

```c
// example with string data
typedef struct {
    char name[50];
    int age;
} Person;

int person_compare(const void *a, const void *b)
{
    const Person *pa = (const Person *)a, *pb = (const Person *)b;
    return (pa->age > pb->age) - (pa->age < pb->age);
}

void person_print(const void *data) {
    Person *p = (Person*)data;
    printf("%s(%d)", p->name, p->age);
}

void person_free(void *data) {
    free(data);
}

// usage
Person *create_person(const char *name, int age) {
    Person *p = malloc(sizeof(Person));
    strcpy(p->name, name);
    p->age = age;
    return p;
}

AVLNode *people_tree = NULL;
people_tree = insert(people_tree, create_person("Alice", 25), person_compare);
people_tree = insert(people_tree, create_person("Bob", 30), person_compare);
```

## API Reference

### Core Operations

| Function                                 | Purpose                   | Time Complexity |
| ---------------------------------------- | ------------------------- | --------------- |
| `insert(root, data, compare)`            | Insert node and rebalance | O(log n)        |
| `delete(root, data, compare, free_data)` | Delete node and rebalance | O(log n)        |
| `search(root, data, compare)`            | Search for a key          | O(log n)        |

### Tree Traversal Functions

| Function                               | Purpose                           | Time Complexity |
| -------------------------------------- | --------------------------------- | --------------- |
| `inorderTraversal(root, print_data)`   | Print nodes in ascending order    | O(n)            |
| `preorderTraversal(root, print_data)`  | Print nodes in preorder sequence  | O(n)            |
| `postorderTraversal(root, print_data)` | Print nodes in postorder sequence | O(n)            |

### Utility Functions

| Function                                     | Purpose                       | Time Complexity |
| -------------------------------------------- | ----------------------------- | --------------- |
| `findMin(node)`                              | Find minimum value in subtree | O(log n)        |
| `findMax(node)`                              | Find maximum value in subtree | O(log n)        |
| `createAVLFromArray(arr, size, compare)`     | Build AVL tree from array     | O(n log n)      |
| `getSize(root)`                              | Count total number of nodes   | O(1)            |
| `printAVL(root, prefix, isLast, print_data)` | Visualize tree structure      | O(n)            |
| `freeAVLTree(root, free_data)`               | Free all nodes and memory     | O(n)            |

### Validation Functions

| Function                                    | Purpose                      | Time Complexity |
| ------------------------------------------- | ---------------------------- | --------------- |
| `isValidBST(root, minVal, maxVal, compare)` | Validate binary search tree  | O(n)            |
| `isValidAVL(root)`                          | Validate AVL tree properties | O(n)            |

### Low-Level Helper Functions

These functions are typically used internally but are available for advanced use cases:

| Function             | Purpose                           | Time Complexity |
| -------------------- | --------------------------------- | --------------- |
| `createNode(data)`   | Create a new node with given data | O(1)            |
| `getHeight(node)`    | Get height of a node              | O(1)            |
| `getBalance(node)`   | Get balance factor of a node      | O(1)            |
| `updateHeight(node)` | Update height after modifications | O(1)            |
| `updateSize(node)`   | Update size after modifications   | O(1)            |
| `rotateRight(node)`  | Perform right rotation            | O(1)            |
| `rotateLeft(node)`   | Perform left rotation             | O(1)            |
| `rebalance(node)`    | Rebalance tree at given node      | O(1)            |

## Tree Visualization

The `printAVL()` function provides a visual representation of the tree structure with height and balance factor information:

```
└── 30[h:3,b:-1]
    ├── 40[h:2,b:+1]
    │   └── 50[h:1,b:0]
    └── 20[h:2,b:0]
        ├── 25[h:1,b:0]
        └── 10[h:1,b:0]
```

- `h:` shows the height of each node
- `b:` shows the balance factor (left height - right height)

## Array to Tree Construction

You can build an AVL tree from an array of any data type:

```c
// example with integers
int values[] = {10, 20, 30, 40, 50, 25};
int size = sizeof(values) / sizeof(values[0]);

// create array of pointers
void *arr[6];
for (int i = 0; i < size; i++) {
    arr[i] = create_int(values[i]);
}

AVLNode *root = createAVLFromArray(arr, size, int_compare);
```

## Generic Data Type Support

This implementation supports **any data type** through function pointers:

### Requirements for Custom Data Types

1. **Comparison Function**: Must return negative, zero, or positive value

   ```c
   int my_compare(const void *a, const void *b);
   ```

2. **Print Function** (optional): For tree visualization

   ```c
   void my_print(const void *data);
   ```

3. **Free Function**: For proper memory management
   ```c
   void my_free(void *data);
   ```

### Memory Management

- All data is stored as `void*` pointers
- You're responsible for allocating and providing the free function
- The tree manages node memory, you manage data memory
- Use `freeAVLTree(root, your_free_function)` to clean up

## Testing

Run the test suite:

```bash
make && ./test
```

Tests include:

- **Basic Operations**: Insert, delete, search operations with tree size validation
- **Rotation Tests**: Left and right rotations (LL and RR cases) with balance verification
- **Edge Cases**: NULL tree operations, single node trees, duplicate insertions
- **Stress Testing**: Large-scale insertions (100,000 nodes) and deletions with performance validation
- **Utility Functions**: Array-to-tree creation, min/max finding, tree traversals
- **AVL Property Validation**: BST property and AVL balance factor verification for all test cases

## Building and Running

### Prerequisites

- GCC compiler with C99 support
- Make utility

### Build Commands

```bash
# Build test binary
make

# Clean build artifacts
make clean

# Build with debug flags
make debug

# Run tests
make run
```

## TODO

- [ ] more query functions (e.g., range queries)
- [ ] more utility functions (e.g., tree serialization)
- [ ] rank operations (e.g., find k-th smallest/largest)

## Acknowledgments

- Based on the original [AVL tree algorithm by Adelson-Velsky and Landis (1962)](https://zhjwpku.com/assets/pdf/AED2-10-avl-paper.pdf)
- Implementation verified using [AVL Tree Visualization](https://www.cs.usfca.edu/~galles/visualization/AVLtree.html)
