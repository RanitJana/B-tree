#include <bits/stdc++.h>
using namespace std;

// A B-tree node
template <typename T, int ORDER>
class BTreeNode
{
public:
    // total number of keys in a node
    T keys[ORDER - 1];
    // total number of children of a node
    BTreeNode *children[ORDER];
    // current number of keys stored in tree
    int n;
    // wheather current node is leaf or not
    bool leaf;

    // initialize the node
    BTreeNode(bool leaf = true)
    {
        for (int i = 0; i < ORDER; i++)
            children[i] = NULL;
        this->leaf = leaf;
        n = 0;
    }
};

template <typename T, int ORDER>
class BTree
{
    BTreeNode<T, ORDER> *root;

    // method to traverse
    void traverse(BTreeNode<T, ORDER> *root);
    // method to search
    BTreeNode<T, ORDER> search(BTreeNode<T, ORDER> *root, T key);

    // method to insert a value in non full node
    void insertNonFull(BTreeNode<T, ORDER> *root, T value);

    // method to split a child or a node
    void splitChild(BTreeNode<T, ORDER> *parent, int i);

    // method to erase a key
    void remove(BTreeNode<T, ORDER> *root, T key);

    // method to erase a key from a leaf node
    void removeFromLeaf(BTreeNode<T, ORDER> *root, int idx);

    // method to erase a key from a non leaf node
    void removeFromNonLeaf(BTreeNode<T, ORDER> *root, int idx);

    T getSuccessor(BTreeNode<T, ORDER> *root, int idx);

    T getPredecessor(BTreeNode<T, ORDER> *root, int idx);

    void merge(BTreeNode<T, ORDER> *root, int idx);

public:
    // initialize the object
    BTree()
    {
        root = new BTreeNode<T, ORDER>(true);
    }
    // method to insert a new key
    void insert(T value);

    // method to verify if root exist and traverse
    void traverse();

    // method to get information of desired key
    BTreeNode<T, ORDER> search(T key);

    // method to erase
    void remove(T key);
};

template <typename T, int ORDER>
void BTree<T, ORDER>::traverse()
{
    // if root is null then return
    if (!root)
    {
        cout << "Tree is empty\n";
        return;
    }
    // otherwise traverse
    traverse(root);
}

template <typename T, int ORDER>
void BTree<T, ORDER>::traverse(BTreeNode<T, ORDER> *root)
{
    // traverse all the keys of current node
    int i;
    for (i = 0; i < root->n; i++)
    {
        // if current node is not a leaf node then traverse the child first
        if (!root->leaf)
            traverse(root->children[i]);
        // after completing the child.. print current key
        cout << root->keys[i] << " ";
    }

    // check wheather right most child is present or not
    if (!root->leaf)
        traverse(root->children[i]);
}

template <typename T, int ORDER>
BTreeNode<T, ORDER> BTree<T, ORDER>::search(T key)
{
    // return if tree is empty
    if (!root)
    {
        cout << "Tree is empty\n";
        return NULL;
    }
    // search and return the node
    return search(root, key);
}

template <typename T, int ORDER>
BTreeNode<T, ORDER> BTree<T, ORDER>::search(BTreeNode<T, ORDER> *root, T key)
{
    // start with first index
    int i = 0;

    // search for the value which is immediate greater or equal to the desired key
    while (i < root->n && key > root->keys[i])
        i++;

    // if the key is found the return the current node;
    if (i < root->n && key == root->keys[i])
        return root;

    // if current node is leaf then return NULL because there is children
    if (root->leaf)
        return NULL;

    // search the right most child or the child of immediate greater key
    return search(root->children[i], key);
}

template <typename T, int ORDER>
void BTree<T, ORDER>::splitChild(BTreeNode<T, ORDER> *parent, int i) // wrong
{
    // left child
    BTreeNode<T, ORDER> *left = parent->children[i];

    // create right child
    BTreeNode<T, ORDER> *right = new BTreeNode<T, ORDER>(left->leaf);

    // find middle index
    int middleIndex = (ORDER - 1) / 2;

    // transfer all the values in the right child that is present in right side of middle index
    for (int j = middleIndex + 1; j < left->n; j++)
        right->keys[j - (middleIndex + 1)] = left->keys[j];

    // set the number of keys int right child
    right->n = left->n - (middleIndex + 1);

    // fix the number of keys present now for left child
    left->n = middleIndex;

    // if left child or right child is not a leaf then transfter the child nodes
    if (!left->leaf)
    {
        for (int j = middleIndex + 1; j < ORDER; j++)
        {
            right->children[j - (middleIndex + 1)] = left->children[j];
            // assign null to left child's children value
            left->children[j] = NULL;
        }
    }

    // right shift all the children in parent node
    for (int j = parent->n; j > i; j--)
        parent->children[j + 1] = parent->children[j];

    // set the new child to parent
    parent->children[i + 1] = right;

    // right shift all the keys in parent node
    for (int j = parent->n - 1; j >= i; j--)
        parent->keys[j + 1] = parent->keys[j];

    // assign new value
    parent->keys[i] = left->keys[middleIndex];

    // increase the total key count
    parent->n++;
}

template <typename T, int ORDER>
void BTree<T, ORDER>::insertNonFull(BTreeNode<T, ORDER> *root, T value)
{
    // detect last index
    int idx = root->n - 1;

    if (root->leaf)
    {
        // search pivot position for the value
        while (idx >= 0 && value < root->keys[idx])
        {
            root->keys[idx + 1] = root->keys[idx];
            idx--;
        }
        // assign the value
        root->keys[idx + 1] = value;
        // increase the count of keys
        root->n++;
    }
    else
    {
        // search pivot position for the value
        int start = 0, end = root->n - 1, mid;

        while (start <= end)
        {
            mid = (start + end) >> 1;

            if (value <= root->keys[mid])
            {
                idx = mid;
                start = mid + 1;
            }
            else
                end = mid - 1;
        }

        // current index's key is greater then the value
        // so the value must be inserted in the left child of current index
        // so check wheather the child is already full or not

        if (root->children[idx]->n == ORDER - 1)
        {
            // chilBTreeNode<T, ORDER>d is full , so we have to split the child
            splitChild(root, idx);

            // now, if the current index's value is lesser then index++
            if (root->keys[idx] < value)
                idx++;
        }
        // now insert for non emty
        insertNonFull(root->children[idx], value);
    }
}

template <typename T, int ORDER>
void BTree<T, ORDER>::insert(T value)
{
    if (root->n == ORDER - 1)
    {
        BTreeNode<T, ORDER> *newRoot = new BTreeNode<T, ORDER>(false);
        newRoot->children[0] = root;

        root = newRoot;

        splitChild(root, 0);
    }

    insertNonFull(root, value);
}

template <typename T, int ORDER>
void BTree<T, ORDER>::remove(T key)
{
    if (!root)
    {
        cout << "Tree is empty\n";
        return;
    }

    remove(root, key);
}

template <typename T, int ORDER>
void BTree<T, ORDER>::remove(BTreeNode<T, ORDER> *root, T key)
{
    int idx = 0;

    // find the key index
    while (idx < root->n && key > root->keys[idx])
        idx++;

    // if key is found
    if (idx < root->n && key == root->keys[idx])
    {
        // call the method to erase the value
        if (root->leaf)
            removeFromLeaf(root, idx);
        else
            removeFromNonLeaf(root, idx);
    }
    else
    {
        // control will enter in this block when the key should present in child node
        // if current node is a leaf node then there is not child. Hence the key is not presnt in the tree
        if (root->leaf)
        {
            cout << key << " is not present in the tree\n";
            return;
        }

        // invoke this function for the child
        remove(root->children[idx], key);

        // balance the tree
        if (root->children[idx]->n < ceil(ORDER / 2) - 1)
            fill(root, idx);

        // pending / updation/ wrong
    }
}

template <typename T, int ORDER>
void BTree<T, ORDER>::removeFromLeaf(BTreeNode<T, ORDER> *root, int idx)
{
    idx++;

    // left shift all the values
    while (idx < root->n)
    {
        root->keys[idx - 1] = root->keys[idx];
        idx++;
    }
    // decrease the size by one
    root->n--;
}

template <typename T, int ORDER>
void BTree<T, ORDER>::removeFromNonLeaf(BTreeNode<T, ORDER> *root, int idx)
{
    // pending
}

template <typename T, int ORDER>
T BTree<T, ORDER>::getSuccessor(BTreeNode<T, ORDER> *root, int idx)
{
    root = root->children[idx + 1];

    while (!root->leaf)
        root = root->children[0];

    return root->keys[0];
}

template <typename T, int ORDER>
T BTree<T, ORDER>::getPredecessor(BTreeNode<T, ORDER> *root, int idx)
{
    root = root->children[idx];

    while (!root->leaf)
        root = root->children[root->n];

    return root->keys[root->n - 1];
}

template <typename T, int ORDER>
void BTree<T, ORDER>::merge(BTreeNode<T, ORDER> *root, int idx)
{
    // // BTreeNode<T, ORDER> *child
}
int main()
{
    BTree<int, 3> t;

    for (int i = 10; i >= 0; i--)
    {

        t.insert(i);
        t.traverse();
        // cout << "Value is :" << t.search(5).leaf;
        cout << endl;
    }
    return 0;
}