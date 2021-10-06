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
  bool lthread, rthread;
  int val;

public:
  // constructor
  Node(int val)
  {
    this->val = val;
    left = right = NULL;
    lthread = rthread = false;
  }
};

// class needed for PrintTree() function
class DoubleTrees
{
public:
  Node *left, *right;
  DoubleTrees()
  {
    left = NULL;
    right = NULL;
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
  Node *search(int x);
  void deleteKey(int x);
  Node *successor(Node *node);
  Node *predecessor(Node *node);
  void printTree();
  void printTreeHelper(const Node *node, ofstream &fout);
  void split(int k);
  DoubleTrees *splitHelper(Node *node, int k);
  int BST_Height();
  double BST_Average_Height_Of_Nodes();
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

  if (!node->lthread)
    copyConstructorHelper(node->left);

  if (!node->rthread)
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
      // go to left child if exists
      if (!cur->lthread)
        cur = cur->left;
      else
        break;
    }
    else
    {
      // go to right child if exists
      if (!cur->rthread)
        cur = cur->right;
      else
        break;
    }
  }

  Node *node = new Node(x);

  if (!par) // insert as root
  {
    root = node;
  }
  else if (x < par->val) // insert as left child
  {
    node->lthread = node->rthread = true;
    node->left = par->left;
    node->right = par;
    par->left = node;
    par->lthread = false;
  }
  else // insert as right child
  {
    node->lthread = node->rthread = true;
    node->right = par->right;
    node->left = par;
    par->right = node;
    par->rthread = false;
  }
}

// search element in bst
Node *BST::search(int x)
{
  Node *cur = root;

  while (cur)
  {
    // return if found
    if (x == cur->val)
      return cur;

    else if (x < cur->val)
    {
      // go to left child if exists
      if (!cur->lthread)
        cur = cur->left;
      else
        break;
    }
    else
    {
      // go to right child if exists
      if (!cur->rthread)
        cur = cur->right;
      else
        break;
    }
  }

  // not found
  return NULL;
}

// deletekey function
void BST::deleteKey(int x)
{
  Node *cur = root, *par = NULL;

  // if element to be deleted not present in
  // the bst then throw an exception
  // if (!search(x))
  // {
  //   throw -1;
  // }

  // find the element to be deleted
  while (cur)
  {
    number_of_comparisons_during_deletion++;
    if (x == cur->val)
    {
      break;
    }
    else if (x < cur->val)
    {
      par = cur;
      cur = cur->left;
    }
    else
    {
      par = cur;
      cur = cur->right;
    }
  }
  if (!cur)
    return;

  if (cur->lthread && cur->rthread) // cur is leaf node
  {
    deleteLeaf(cur, par);
  }
  else if (!cur->lthread && !cur->rthread) // cur has two children
  {
    deleteNodeWithTwoChildren(cur, par);
  }
  else // cur has one child
  {
    deleteNodeWithOneChild(cur, par);
  }
}

void BST::deleteLeaf(Node *node, Node *par)
{
  // If Node to be deleted is root
  if (!par)
    root = NULL;

  // If Node to be deleted is left
  // of its parent
  else if (node == par->left)
  {
    par->lthread = true;
    par->left = node->left;
  }
  else
  {
    par->rthread = true;
    par->right = node->right;
  }

  // Free memory
  delete (node);
}

void BST::deleteNodeWithTwoChildren(Node *node, Node *par)
{
  // Find inorder successor and its parent.
  Node *parsucc = node;
  Node *succ = node->right;

  // Find leftmost child of successor
  while (!succ->lthread && succ->left != NULL)
  {
    parsucc = succ;
    succ = succ->left;
  }

  node->val = succ->val;

  if (succ->lthread == true && succ->rthread == true)
    deleteLeaf(succ, parsucc);
  else
    deleteNodeWithOneChild(succ, parsucc);
}

void BST::deleteNodeWithOneChild(Node *node, Node *par)
{
  Node *child;

  // Initialize child Node to be deleted has
  // left child.
  if (node->lthread == false)
    child = node->left;

  // Node to be deleted has right child.
  else
    child = node->right;

  // Node to be deleted is root Node.
  if (par == NULL)
    root = child;

  // Node is left child of its parent.
  else if (node == par->left)
    par->left = child;
  else
    par->right = child;

  // Find successor and predecessor
  Node *s = successor(node);
  Node *p = predecessor(node);

  // If node has left subtree.
  if (node->lthread == false)
    p->right = s;

  // If node has right subtree.
  else
  {
    if (node->rthread == false)
      s->left = p;
  }

  // free memory
  delete (node);
}

// find successor of a node
Node *BST::successor(Node *node)
{
  Node *cur = node;

  // if right child does not exist
  // return successor stored as right child
  if (cur->rthread)
    return cur->right;

  // go to right child if exists
  cur = cur->right;
  if (!cur)
    return cur;

  // go to left most element
  while (!cur->lthread && cur->left)
    cur = cur->left;
  return cur;
}

Node *BST::predecessor(Node *node)
{
  // if left child does not exist
  // return successor stored as left child
  Node *cur = node;
  if (cur->lthread)
    return cur->left;

  // go to left child if exists
  cur = cur->left;
  if (!cur)
    return cur;

  // go to right most element
  while (!cur->rthread && cur->right)
    cur = cur->right;
  return cur;
}

// function to print the bst
void BST::printTree()
{
  ofstream fout;

  fout.open("BST.dot");
  fout << "digraph g {\n";
  printTreeHelper(root, fout);
  fout << "}";
  fout.close();
  system("dot -Tpng BST.dot -o BST.png");
}

void BST::printTreeHelper(const Node *node, ofstream &fout)
{
  if (!node)
    return;

  if (node == root) // add the label and root in the dot file
  {
    fout << "label = \" rooted at " << node->val << " \";\n";
    fout << node->val << " [root = true]\n";
  }

  if (!node->lthread) // if left child exits
  {
    fout << node->val << " -> " << node->left->val << "\n";
    printTreeHelper(node->left, fout);
  }
  else
  {
    if (node->left)
      fout << node->val << " -> " << node->left->val << "[style = dotted];\n";
  }

  if (!node->rthread) // if right child exists
  {
    fout << node->val << " -> " << node->right->val << "\n";
    printTreeHelper(node->right, fout);
  }
  else
  {
    if (node->right)
      fout << node->val << " -> " << node->right->val << "[style = dotted];\n";
  }
}

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
  int lh = node->lthread ? 0 : BST_Height_Helper(node->left);
  int rh = node->rthread ? 0 : BST_Height_Helper(node->right);
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
  long int lh = node->lthread ? 0 : BST_Average_Height_Of_Nodes_Helper(node->left, total_height, number_of_nodes);
  long int rh = node->rthread ? 0 : BST_Average_Height_Of_Nodes_Helper(node->right, total_height, number_of_nodes);

  int h = max(lh, rh) + 1;
  total_height += h;
  return h;
}

//========================================================================================================================