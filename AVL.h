#include <iostream>
#include <fstream>
#include <bits/stdc++.h>

using namespace std;

// class for AVL node
class AVL_Node
{
private:
  int key;
  int bf; // balance factor bf = height(left subtree) – height(right subtree)
  AVL_Node *LChild, *RChild;
  friend class AVL_Tree;

public:
  // parameterized constructor
  AVL_Node(int k)
  {
    key = k;
    bf = 0;
    LChild = RChild = NULL;
  }

  // non-parameterized constructor
  AVL_Node()
  {
    bf = 0;
    LChild = RChild = NULL;
  }
};

class AVL_Tree
{
private:
  AVL_Node *root;
  int number_of_rotations_during_insertion;
  int number_of_rotations_during_deletion;
  int number_of_comparisons_during_insertion;
  int number_of_comparisons_during_deletion;

  AVL_Node *link(int a, AVL_Node *node);
  void singleRotation(AVL_Node *&p, AVL_Node *&r, AVL_Node *&s, int a);
  void doubleRotation(AVL_Node *&p, AVL_Node *&r, AVL_Node *&s, int a);
  void AVL_PrintHelper(const AVL_Node *node, ofstream &fout);
  void destructorHelper(AVL_Node *avl_node);
  void copyConstructorHelper(const AVL_Node *node);
  void RotateR(AVL_Node *&ptr);
  void RotateL(AVL_Node *&ptr);
  void RotateRL(AVL_Node *&ptr);
  void RotateLR(AVL_Node *&ptr);
  long int AVL_Average_Height_Of_Nodes_Helper(AVL_Node *node, long int &total_height, double &number_of_nodes);
  int AVL_Height_Helper(AVL_Node *node);

public:
  AVL_Tree();
  AVL_Tree(const AVL_Tree &T);
  void AVL_Insert(int k);
  void AVL_Delete(int k);
  bool AVL_Search(int k);
  void AVL_Print(const char *filename);
  double AVL_Average_Height_Of_Nodes();
  int AVL_Height();
  ~AVL_Tree();
};

//===============================================================================================

// function to handle the LL case in rotation
void AVL_Tree::RotateR(AVL_Node *&ptr) //Right Rotation
{
  AVL_Node *subR = ptr;
  ptr = subR->LChild;
  subR->LChild = ptr->RChild;
  ptr->RChild = subR;
  ptr->bf = subR->bf = 0;
}

//===============================================================================================

// function to handle the RR case in rotation
void AVL_Tree::RotateL(AVL_Node *&ptr) //Left Rotation
{
  AVL_Node *subL = ptr;
  ptr = subL->RChild;
  subL->RChild = ptr->LChild;
  ptr->LChild = subL;
  ptr->bf = subL->bf = 0;
}

//===============================================================================================

// function to handle the RL case in rotation
void AVL_Tree::RotateRL(AVL_Node *&ptr) //Rotate right then left
{
  AVL_Node *subL = ptr;
  AVL_Node *subR = ptr->RChild;
  ptr = subR->LChild;

  subR->LChild = ptr->RChild;
  ptr->RChild = subR;

  if (ptr->bf >= 0)
    subR->bf = 0;
  else
    subR->bf = 1;

  subL->RChild = ptr->LChild;
  ptr->LChild = subL;

  if (ptr->bf <= 0)
    subL->bf = 0;
  else
    subL->bf = -1;

  ptr->bf = 0;
}

//===============================================================================================

// function to handle the LR case in rotation
void AVL_Tree::RotateLR(AVL_Node *&ptr) //Rotate left then right
{
  AVL_Node *subL = ptr->LChild;
  AVL_Node *subR = ptr;
  ptr = subL->RChild;

  subL->RChild = ptr->LChild;
  ptr->LChild = subL;

  if (ptr->bf <= 0)
    subL->bf = 0;
  else
    subL->bf = -1;

  subR->LChild = ptr->RChild;
  ptr->RChild = subR;

  if (ptr->bf >= 0)
    subR->bf = 0;
  else
    subR->bf = 1;

  ptr->bf = 0;
}

//===============================================================================================

// constructor for AVL_Tree class
AVL_Tree::AVL_Tree()
{
  number_of_comparisons_during_deletion = 0;
  number_of_comparisons_during_insertion = 0;
  number_of_rotations_during_deletion = 0;
  number_of_rotations_during_insertion = 0;
  root = new AVL_Node();
}

//===============================================================================================

// copy constructor for AVL_Tree class
AVL_Tree::AVL_Tree(const AVL_Tree &T)
{
  number_of_comparisons_during_deletion = 0;
  number_of_comparisons_during_insertion = 0;
  number_of_rotations_during_deletion = 0;
  number_of_rotations_during_insertion = 0;
  root = new AVL_Node();
  copyConstructorHelper(T.root->RChild);
}

//===============================================================================================

// helper function to do the traversal of AVL tree
// and copy it in a new object
void AVL_Tree::copyConstructorHelper(const AVL_Node *node)
{
  if (!node)
    return;
  AVL_Insert(node->key);
  copyConstructorHelper(node->LChild);
  copyConstructorHelper(node->RChild);
}

//===============================================================================================

// function to return the pointer of child whose subtree height is more
AVL_Node *AVL_Tree::link(int a, AVL_Node *node)
{
  if (a == -1)
    return node->LChild;
  return node->RChild;
};

//===============================================================================================

// function to insert a value (k) in the AVL tree
void AVL_Tree::AVL_Insert(int k)
{
  AVL_Node *t = root;         // pointer to parent of balance point
  AVL_Node *s = root->RChild; // pointer to balance point (where imbalance might have occurred)
  AVL_Node *p = root->RChild; // pointer to traverse te AVL tree
  AVL_Node *q, *r;

  AVL_Node *avl_node = new AVL_Node(k); // create new node

  if (!p) // if tree is empty
  {
    root->RChild = avl_node;
    return;
  }

  // traverse the tree to find and insert k
  // at its correct position
  while (true)
  {
    number_of_comparisons_during_insertion++;
    if (k < p->key) // k should be inserted in left subtree
    {
      q = p->LChild;
      if (!q)
      {
        q = avl_node;
        p->LChild = q;
        break;
      }
    }
    else if (k > p->key) // k should be inserted in right subtree
    {
      q = p->RChild;
      if (!q)
      {
        q = avl_node;
        p->RChild = q;
        break;
      }
    }
    else // k is already present in the tree
    {
      throw "Element already exists.";
    }

    if (q->bf != 0) // if q can be the balance point, update s and t
    {
      t = p;
      s = q;
    }

    p = q; // go down with p
  }

  int a = 1;
  if (k < s->key)
    a = -1;

  r = p = link(a, s);

  // update bf for all nodes between p and q
  while (p != q)
  {
    if (k < p->key)
    {
      p->bf = -1;
      p = p->LChild;
    }
    else if (k > p->key)
    {
      p->bf = 1;
      p = p->RChild;
    }
  }

  if (s->bf == 0) // tree has grown higher
  {
    s->bf = a;
    return;
  }

  if (s->bf == -a) // tree has become more balanced
  {
    s->bf = 0;
    return;
  }

  // tree has become imbalanced
  if (s->bf == a)
  {
    if (r->bf == a)
    {
      number_of_rotations_during_insertion++;
      singleRotation(p, r, s, a); // Either LL or RR case for a = -1 and a = 1 respectively
    }
    else if (r->bf == -a)
    {
      number_of_rotations_during_insertion += 2;
      doubleRotation(p, r, s, a); // Either LR or RL case for a = -1 and a = 1 respectively
    }
  }

  // update root if tree is rotated through it
  if (s == t->RChild)
    t->RChild = p;
  else
    t->LChild = p;
}

//===============================================================================================

void AVL_Tree::singleRotation(AVL_Node *&p, AVL_Node *&r, AVL_Node *&s, int a)
{
  p = r;
  if (a == 1) // for RR case
  {
    s->RChild = r->LChild;
    r->LChild = s;
  }
  else // for LL case
  {
    s->LChild = r->RChild;
    r->RChild = s;
  }
  s->bf = r->bf = 0;
}

//===============================================================================================

void AVL_Tree::doubleRotation(AVL_Node *&p, AVL_Node *&r, AVL_Node *&s, int a)
{
  if (a == -1) // for LR case
  {
    p = r->RChild;
    r->RChild = p->LChild;
    p->LChild = r;
    s->LChild = p->RChild;
    p->RChild = s;
  }
  else // for RL case
  {
    p = r->LChild;
    r->LChild = p->RChild;
    p->RChild = r;
    s->RChild = p->LChild;
    p->LChild = s;
  }

  if (p->bf == a)
  {
    s->bf = -a;
    r->bf = 0;
  }
  else if (p->bf == 0)
  {
    s->bf = 0;
    r->bf = 0;
  }
  else if (p->bf == -a)
  {
    s->bf = 0;
    r->bf = a;
  }
  p->bf = 0;
}

//===============================================================================================

// function to print the AVL tree in a png file
void AVL_Tree::AVL_Print(const char *filename)
{
  ofstream fout;

  string dot_file = "";
  dot_file = dot_file + filename + ".dot"; // name of graphviz file

  string png_file = "";
  png_file = png_file + filename + ".png"; // name of png file

  fout.open(dot_file.c_str()); // open dot file for writing

  fout << "digraph g {\n";
  fout << "node [shape=record, height=0.1];\n";

  AVL_PrintHelper(root->RChild, fout);

  fout << "}";
  fout.close(); // close dot file

  string str = "dot -Tpng ";
  str = str + dot_file + " -o " + png_file;

  const char *command = str.c_str();

  system(command); // system call to run the dot file using graphviz

  cout << "Tree Printed Successfully! Please check the " << png_file << " file.\n";
}

//===============================================================================================

// helper function to traverse the AVL tree in preorder fashion
// and print the edges in the required format in the dot file
void AVL_Tree::AVL_PrintHelper(const AVL_Node *node, ofstream &fout)
{
  if (!node) // if node is NULL
    return;

  if (node == root->RChild) // add the label and root in the dot file
  {
    fout << "label = \" rooted at " << node->key << " \";\n";
    fout << node->key << " [root = true]\n";
  }

  fout << node->key << " [label=\"<f0>|<f1>" << node->key << "|<f2> " << -node->bf << " |<f3> \"];\n";

  if (node->LChild) // if left child exists
  {
    fout << node->key << ":f0 -> " << node->LChild->key << ":f1\n"; // write edge in dot file
    AVL_PrintHelper(node->LChild, fout);                            // recurse for left subtree
  }

  if (node->RChild) // if right child exists
  {
    fout << node->key << ":f3 -> " << node->RChild->key << ":f1\n"; // write edge in dot file
    AVL_PrintHelper(node->RChild, fout);                            // recurse for right subtree
  }
}

//===============================================================================================

// function to search for an element (k) in the
// AVL tree and return a boolean value
bool AVL_Tree::AVL_Search(int k)
{
  AVL_Node *avl_node = root->RChild;

  while (avl_node) // traverse while cur node is not NULL
  {
    if (k == avl_node->key) // k is found
      return true;
    if (k < avl_node->key) // k is in left subtree
      avl_node = avl_node->LChild;
    else // k is in right subtree
      avl_node = avl_node->RChild;
  }

  // k was not found in the tree
  return false;
}

//===============================================================================================

// function to delete an element (k)
// from the AVL tree
void AVL_Tree::AVL_Delete(int k)
{
  if (!AVL_Search(k)) // if k is not present in the tree
  {
    throw "Key does not exist in the tree.";
  }

  AVL_Node *pr = NULL;
  AVL_Node *p = root->RChild, *q;
  stack<AVL_Node *> st;

  // traverse the tree to find node with key value k
  while (p)
  {
    number_of_comparisons_during_deletion++;
    if (k == p->key) // k is found
      break;

    pr = p;
    st.push(pr);

    if (k < p->key) // k is in left subtree
      p = p->LChild;
    else // k is in right subtree
      p = p->RChild;
  }

  // if p has both children (left and right)
  if (p->LChild != NULL && p->RChild != NULL)
  {
    pr = p;
    st.push(pr);

    // find the successor of p in q
    q = p->RChild;
    while (q->LChild != NULL)
    {
      pr = q;
      st.push(pr);
      q = q->LChild;
    }

    p->key = q->key; // copy key value of q to p
    p = q;
  }

  if (p->LChild != NULL) // if p has left child
    q = p->LChild;
  else // if p has right child
    q = p->RChild;

  // p deleted node, q deleted child node
  if (pr == NULL) // p is the root
    root->RChild = q;
  else // p is an internal node or a leaf
  {
    if (p == pr->LChild) // if p is left child of its parent
      pr->LChild = q;
    else // if p is right child of its parent
      pr->RChild = q;

    // adjust Balance
    while (!st.empty())
    {
      pr = st.top();
      st.pop();

      if (p->key < pr->key) // k was deleted from left subtree
        pr->bf++;
      else
        pr->bf--; // k was deleted from right subtree

      if (pr->bf == 1 || pr->bf == -1) // whole tree is balanced
        break;

      if (pr->bf != 0)
      {
        // let q point to a higher subtree
        if (pr->bf < 0)
          q = pr->LChild;
        else
          q = pr->RChild;

        if (q->bf == 0)
        {
          number_of_rotations_during_deletion++;
          if (pr->bf < 0) // LL case
          {
            RotateR(pr);
            pr->bf = 1;
            pr->RChild->bf = -1;
          }
          else // RR case
          {
            RotateL(pr);
            pr->bf = -1;
            pr->LChild->bf = 1;
          }

          if (!st.empty())
          {
            AVL_Node *ppr = st.top();
            if (ppr->key < pr->key)
              ppr->RChild = pr;
            else
              ppr->LChild = pr;
          }
          else
            root->RChild = pr;

          break;
        }

        if (pr->bf < 0)
        {
          if (q->bf < 0) // LL case
          {
            number_of_rotations_during_deletion++;
            RotateR(pr);
          }
          else
          {
            number_of_rotations_during_deletion += 2;
            RotateLR(pr); // LR case
          }
        }
        else
        {
          number_of_rotations_during_deletion++;
          if (q->bf > 0)
          {
            RotateL(pr); // RR case
          }
          else
          {
            number_of_rotations_during_deletion += 2;
            RotateRL(pr); // RL case
          }
        }

        if (!st.empty())
        {
          AVL_Node *ppr = st.top();
          // ppr->key < pr->key ? (ppr->RChild = pr) : (ppr->LChild = pr);
          if (ppr->key < pr->key)
            ppr->RChild = pr;
          else
            ppr->LChild = pr;
        }
        else
          root->RChild = pr;
      }

      q = pr;

    } //end while
  }
  delete p;
}

//===============================================================================================

// destructor
AVL_Tree::~AVL_Tree()
{
  destructorHelper(root->RChild); // delete all children
  delete (root);                  // delete root
}

//===============================================================================================

// helper function to traverse the AVL tree in post order
// fashion and delete the nodes one by one
void AVL_Tree::destructorHelper(AVL_Node *avl_node)
{
  if (!avl_node) // if node is NULL
    return;
  if (avl_node->LChild) // node has left child
    destructorHelper(avl_node->LChild);

  if (avl_node->RChild) // node has right child
    destructorHelper(avl_node->RChild);

  delete (avl_node); // deallocate memory for node
}

//========================================================================================================================

int AVL_Tree::AVL_Height()
{
  return AVL_Height_Helper(root->RChild);
}

//========================================================================================================================

int AVL_Tree::AVL_Height_Helper(AVL_Node *node)
{
  if (!node)
    return 0;
  return 1 + max(AVL_Height_Helper(node->LChild), AVL_Height_Helper(node->RChild));
}

//========================================================================================================================

double AVL_Tree::AVL_Average_Height_Of_Nodes()
{
  long int h = 0;
  int c = 1;
  double number_of_nodes = 0;
  AVL_Average_Height_Of_Nodes_Helper(root->RChild, h, number_of_nodes);
  return h / number_of_nodes;
}

//========================================================================================================================

long int AVL_Tree::AVL_Average_Height_Of_Nodes_Helper(AVL_Node *node, long int &total_height, double &number_of_nodes)
{
  if (!node)
    return 0;
  ++number_of_nodes;
  long int lh = AVL_Average_Height_Of_Nodes_Helper(node->LChild, total_height, number_of_nodes);
  long int rh = AVL_Average_Height_Of_Nodes_Helper(node->RChild, total_height, number_of_nodes);

  int h = max(lh, rh) + 1;
  total_height += h;
  return h;
}

//========================================================================================================================