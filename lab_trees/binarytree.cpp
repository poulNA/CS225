/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file.
 */
#include "TreeTraversals/InorderTraversal.h"
#include <iostream>
#include <queue>

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function.
 * @param subRoot
 * @return The height of the subtree
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + std::max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Call recursive helper function on the root
    printLeftToRight(root);

    // Finish the line
    std::cout << std::endl;
}

/**
 * Private helper function for the public printLeftToRight function.
 * @param subRoot
 */
template <typename T>
void BinaryTree<T>::printLeftToRight(const Node* subRoot) const
{
    // Base case - null node
    if (subRoot == NULL)
        return;

    // Print left subtree
    printLeftToRight(subRoot->left);

    // Print this node
    std::cout << subRoot->elem << ' ';

    // Print right subtree
    printLeftToRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 *  (not creating a flipped copy).
 */
    template <typename T>
void BinaryTree<T>::mirror()
{
    //your code here
    mirror(root);
}

template <typename T>
void BinaryTree<T>::mirror(Node * subroot)
{
    //your code here
    if (subroot == NULL) {return;}

    Node * temp = subroot->right;
    subroot->right = subroot->left;
    subroot->left = temp;
     
    mirror(subroot->left);
    mirror(subroot->right);
}




/**
 * isOrdered() function iterative version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedIterative() const
{
    // your code here
    InorderTraversal<int> trav(root);

    int prev = (*trav.begin())->elem - 1;
    for (TreeTraversal<int>::Iterator i = trav.begin(); i != trav.end(); ++i) {
        if ((*i)->elem > prev) {
            prev = (*i)->elem;
        } else {
            return false;
        }
    }
    return true;
}

// template <typename T>
// bool BinaryTree<T>::isOrderedIterative(const Node * subroot) const
// {
//     // your code here
//     // InorderTraversal(subnode);
//     // T var = stack.peek()->elem;
//     // while (!stack.empty()) {
//     //     stack.pop();
//     //     if (var < stack.peek()->elem) {return false;}
//     //     var = stack.peek()->elem;
//     // }
//     return true;
    
// }
/**
 * isOrdered() function recursive version
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrderedRecursive() const
{
    // your code here
    std::queue<T> qu;
    isOrderedRecursive(root, qu);
    T temp = qu.front();
    while (qu.empty() == false) {
        qu.pop();
        if (qu.empty() == true) {return true;}
        if (qu.front() < temp) {return false;}
        temp = qu.front();
    }
    return true;
}

template <typename T>
void BinaryTree<T>::isOrderedRecursive(const Node * subroot, std::queue<T>& qu) const
{
    // your code here
    if (subroot == NULL) {return;}

    isOrderedRecursive(subroot->left, qu);
    qu.push(subroot->elem);
    isOrderedRecursive(subroot->right, qu);
}

