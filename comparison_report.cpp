#include <bits/stdc++.h>
#include "Treap.h"
#include "BST.h"
#include "AVL.h"
using namespace std;

int main()
{
  srand(time(0));

  int number_of_test_files = 5;

  for (int i = 0; i < number_of_test_files; i++)
  {
    string file_name = "test_case_";
    file_name += ('0' + i);
    file_name += ".txt";

    freopen(file_name.c_str(), "r", stdin);

    Treap *treap = new Treap();
    BST *bst = new BST();
    AVL_Tree *avl = new AVL_Tree();

    int number_of_operations;
    cin >> number_of_operations;

    int number_of_insertions = 0, number_of_deletions = 0;

    for (int i = 0; i < number_of_operations; i++)
    {
      string s;
      int num;
      cin >> s >> num;
      if (s == "Insert")
      {
        ++number_of_insertions;
        treap->Treap_Insert(num);
        bst->insert(num);
        avl->AVL_Insert(num);
      }
      else if (s == "Delete")
      {
        ++number_of_deletions;
        treap->Treap_Delete(num);
        bst->deleteKey(num);
        avl->AVL_Delete(num);
      }
    }

    cout << "File: " << file_name << "\n\n";
    cout << "Number of Insertions: " << number_of_insertions << "\n";
    cout << "Number of Deletions: " << number_of_deletions << "\n\n";

    cout << "Type\tComparison(Insert)\tComparison(Delete)\tRotation(Insert)\tRotation(Delete)\tHeight\tAverage Node Height\n";
    cout << "=============================================================================================================================================\n";

    cout << "Treap\t";
    cout << treap->get_number_of_comparisons_during_insertion() << "\t\t\t";
    cout << treap->get_number_of_comparisons_during_deletion() << "\t\t\t";
    cout << treap->get_number_of_rotations_during_insertion() << "\t\t\t";
    cout << treap->get_number_of_rotations_during_deletion() << "\t\t\t";
    cout << treap->Treap_Height() << "\t" << treap->Treap_Average_Height_Of_Nodes() << "\n\n";

    cout << "BST\t";
    cout << bst->get_number_of_comparisons_during_insertion() << "\t\t\t";
    cout << bst->get_number_of_comparisons_during_deletion() << "\t\t\t";
    cout << bst->get_number_of_rotations_during_insertion() << "\t\t\t";
    cout << bst->get_number_of_rotations_during_deletion() << "\t\t\t";
    cout << bst->BST_Height() << "\t" << bst->BST_Average_Height_Of_Nodes() << "\n\n";

    cout << "AVL\t";
    cout << avl->get_number_of_comparisons_during_insertion() << "\t\t\t";
    cout << avl->get_number_of_comparisons_during_deletion() << "\t\t\t";
    cout << avl->get_number_of_rotations_during_insertion() << "\t\t\t";
    cout << avl->get_number_of_rotations_during_deletion() << "\t\t\t";
    cout << avl->AVL_Height() << "\t" << avl->AVL_Average_Height_Of_Nodes() << "\n\n";

    cout << "=============================================================================================================================================\n\n";
  }

  return 0;
}