#include <iostream>
#include <limits.h>
#include <fstream>
#include <cstdlib>
using namespace std;

// class for BST node
class Node
{
public:
  Node *left, *right;
  int val;

public:
  // constructor
  Node(int val)
  {
    this->val = val;
    left = right = NULL;
  }
};

// class for BST
class BST
{
public:
  Node *root;
  int number_of_rotations_during_insertion;
  int number_of_rotations_during_deletion;
  int number_of_comparisons_during_insertion;
  int number_of_comparisons_during_deletion;

  void copyConstructorHelper(const Node *node);
  void deleteLeaf(Node *node, Node *par);
  void deleteNodeWithOneChild(Node *node, Node *par);
  void deleteNodeWithTwoChildren(Node *node, Node *par);
  int kthElementHelper(Node *node, int k);
  long int BST_Average_Height_Of_Nodes_Helper(Node *node, long int &total_height, double &number_of_nodes);
  int BST_Height_Helper(Node *node);

public:
  BST();
  BST(BST &bst);

  void insert(int x);
  bool search(int x);
  void deleteKey(int x);
  Node *successor(Node *node);
  Node *predecessor(Node *node);
  void printTree();
  void printTreeHelper(const Node *node, ofstream &fout);
  void split(int k);
  int BST_Height();
  double BST_Average_Height_Of_Nodes();
  int get_number_of_rotations_during_insertion();
  int get_number_of_rotations_during_deletion();
  int get_number_of_comparisons_during_insertion();
  int get_number_of_comparisons_during_deletion();
};

// default constructor
BST::BST()
{
  number_of_comparisons_during_deletion = 0;
  number_of_comparisons_during_insertion = 0;
  number_of_rotations_during_deletion = 0;
  number_of_rotations_during_insertion = 0;
  root = NULL;
}

// copy constructor
BST::BST(BST &bst)
{
  number_of_comparisons_during_deletion = 0;
  number_of_comparisons_during_insertion = 0;
  number_of_rotations_during_deletion = 0;
  number_of_rotations_during_insertion = 0;
  // Node *node = bst->root;
  if (bst.root)
    copyConstructorHelper(bst.root);
}

void BST::copyConstructorHelper(const Node *node)
{
  if (!node)
    return;
  insert(node->val);

  copyConstructorHelper(node->left);
  copyConstructorHelper(node->right);
}

// insert function
void BST::insert(int x)
{
  Node *cur = root;
  Node *par = NULL;

  // find position where element is to be inserted
  while (cur)
  {
    number_of_comparisons_during_insertion++;
    if (x == cur->val)
      return;

    par = cur;

    if (x < cur->val)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }

  Node *node = new Node(x);

  if (!par) // insert as root
  {
    root = node;
  }
  else if (x < par->val) // insert as left child
  {
    par->left = node;
  }
  else // insert as right child
  {
    par->right = node;
  }
}

// search element in bst
bool BST::search(int x)
{
  Node *cur = root;

  while (cur)
  {
    // return if found
    if (x == cur->val)
      return true;

    else if (x < cur->val)
    {
      // go to left child
      cur = cur->left;
    }
    else
    {
      // go to right child
      cur = cur->right;
    }
  }

  // not found
  return false;
}

// deletekey function
void BST::deleteKey(int x)
{
  Node *cur = root, *par = NULL;

  // find the element to be deleted
  while (cur)
  {
    number_of_comparisons_during_deletion++;
    if (x == cur->val)
    {
      break;
    }

    par = cur;
    if (x < cur->val)
    {
      cur = cur->left;
    }
    else
    {
      cur = cur->right;
    }
  }
  if (!cur)
    return;

  // Check if the node to be
  // deleted has atmost one child.
  if (cur->left == NULL || cur->right == NULL)
  {

    // newcur will replace
    // the node to be deleted.
    Node *newcur;

    // if the left child does not exist.
    if (cur->left == NULL)
      newcur = cur->right;
    else
      newcur = cur->left;

    // check if the node to
    // be deleted is the root.
    if (par == NULL)
    {
      root = NULL;
      delete (newcur);
    }

    // check if the node to be deleted
    // is par's left or right child
    // and then replace this with newcur
    if (cur == par->left)
      par->left = newcur;
    else
      par->right = newcur;

    // free memory of the
    // node to be deleted.
    delete (cur);
  }

  // node to be deleted has
  // two children.
  else
  {
    Node *p = NULL;
    Node *temp;

    // Compute the inorder successor
    temp = cur->right;
    while (temp->left != NULL)
    {
      p = temp;
      temp = temp->left;
    }

    // check if the parent of the inorder
    // successor is the cur or not(i.e. cur=
    // the node which has the same data as
    // the given data by the user to be
    // deleted). if it isn't, then make the
    // the left child of its parent equal to
    // the inorder successor'd right child.
    if (p != NULL)
      p->left = temp->right;

    // if the inorder successor was the
    // cur (i.e. cur = the node which has the
    // same data as the given data by the
    // user to be deleted), then make the
    // right child of the node to be
    // deleted equal to the right child of
    // the inorder successor.
    else
      cur->right = temp->right;

    cur->val = temp->val;
    delete (temp);
  }
}

// void BST::deleteLeaf(Node *node, Node *par)
// {
//   // If Node to be deleted is root
//   if (!par)
//     root = NULL;

//   // If Node to be deleted is left
//   // of its parent
//   else if (node == par->left)
//   {
//     par->left = NULL;
//   }
//   else
//   {
//     par->right = NULL;
//   }

//   // Free memory
//   delete (node);
// }

// void BST::deleteNodeWithTwoChildren(Node *node, Node *par)
// {
//   // Find inorder successor and its parent.
//   Node *parsucc = node;
//   Node *succ = node->right;

//   // Find leftmost child of successor
//   while (succ->left != NULL)
//   {
//     parsucc = succ;
//     succ = succ->left;
//   }

//   node->val = succ->val;

//   if (!succ->left && !succ->right)
//     deleteLeaf(succ, parsucc);
//   else
//     deleteNodeWithOneChild(succ, parsucc);
// }

// void BST::deleteNodeWithOneChild(Node *node, Node *par)
// {
//   Node *child;

//   // Initialize child Node to be deleted has
//   // left child.
//   if (node->left)
//     child = node->left;

//   // Node to be deleted has right child.
//   else
//     child = node->right;

//   // Node to be deleted is root Node.
//   if (par == NULL)
//     root = child;

//   // Node is left child of its parent.
//   else if (node == par->left)
//     par->left = child;
//   else
//     par->right = child;

//   // Find successor and predecessor
//   Node *s = successor(node);
//   Node *p = predecessor(node);

//   // If node has left subtree.
//   if (node->left)
//     p->right = s;

//   // If node has right subtree.
//   else
//   {
//     if (node->right)
//       s->left = p;
//   }

//   // free memory
//   delete (node);
// }

// find successor of a node
// Node *BST::successor(Node *node)
// {
//   Node *cur = node;

//   // if right child does not exist
//   // return successor stored as right child
//   if (cur->rthread)
//     return cur->right;

//   // go to right child if exists
//   cur = cur->right;
//   if (!cur)
//     return cur;

//   // go to left most element
//   while (cur->left)
//     cur = cur->left;
//   return cur;
// }

// Node *BST::predecessor(Node *node)
// {
//   // if left child does not exist
//   // return successor stored as left child
//   Node *cur = node;
//   if (cur->lthread)
//     return cur->left;

//   // go to left child if exists
//   cur = cur->left;
//   if (!cur)
//     return cur;

//   // go to right most element
//   while (!cur->rthread && cur->right)
//     cur = cur->right;
//   return cur;
// }

//========================================================================================================================

int BST::BST_Height()
{
  return BST_Height_Helper(root);
}

//========================================================================================================================

int BST::BST_Height_Helper(Node *node)
{
  if (!node)
    return 0;
  int lh = BST_Height_Helper(node->left);
  int rh = BST_Height_Helper(node->right);
  return 1 + max(lh, rh);
}

//========================================================================================================================

double BST::BST_Average_Height_Of_Nodes()
{
  long int h = 0;
  int c = 1;
  double number_of_nodes = 0;
  BST_Average_Height_Of_Nodes_Helper(root, h, number_of_nodes);
  return h / number_of_nodes;
}

//========================================================================================================================

long int BST::BST_Average_Height_Of_Nodes_Helper(Node *node, long int &total_height, double &number_of_nodes)
{
  if (!node)
    return 0;
  ++number_of_nodes;
  long int lh = BST_Average_Height_Of_Nodes_Helper(node->left, total_height, number_of_nodes);
  long int rh = BST_Average_Height_Of_Nodes_Helper(node->right, total_height, number_of_nodes);

  int h = max(lh, rh) + 1;
  total_height += h;
  return h;
}

//========================================================================================================================

int BST::get_number_of_rotations_during_insertion() { return number_of_rotations_during_insertion; }
int BST::get_number_of_rotations_during_deletion() { return number_of_rotations_during_deletion; }
int BST::get_number_of_comparisons_during_insertion() { return number_of_comparisons_during_insertion; }
int BST::get_number_of_comparisons_during_deletion() { return number_of_comparisons_during_deletion; }