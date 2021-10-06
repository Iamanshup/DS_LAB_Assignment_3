#include <bits/stdc++.h>
using namespace std;

int main()
{
  srand(time(0));

  int number_of_test_files = 5;
  int number_of_operations = 10000;
  int max_element = 10000;

  double deletions[] = {0.2, 0.3, 0.5, 0.75, 0.9};

  for (int i = 0; i < number_of_test_files; i++)
  {
    string file_name = "test_case_";
    file_name += to_string(i);
    file_name += ".txt";

    freopen(file_name.c_str(), "w", stdout);

    cout << number_of_operations << endl;

    int number_of_deletions = 0;
    int number_of_insertions = 0;

    while (number_of_deletions < number_of_operations * deletions[i] && number_of_insertions < number_of_operations * (1 - deletions[i]))
    {
      int type = abs(rand()) % 2;
      int num = rand() % max_element;
      if (type == 0) // insert
      {
        ++number_of_insertions;
        cout << "Insert " << num << endl;
      }
      else // delete
      {
        ++number_of_deletions;
        cout << "Delete " << num << endl;
      }
    }

    while (number_of_deletions < number_of_operations * deletions[i])
    {
      int type = abs(rand()) % 2;
      int num = rand() % max_element;
      ++number_of_deletions;
      cout << "Delete " << num << endl;
    }

    while (number_of_insertions < number_of_operations * (1 - deletions[i]))
    {
      int type = abs(rand()) % 2;
      int num = rand() % max_element;
      ++number_of_insertions;
      cout << "Insert " << num << endl;
    }

    // for (int j = 0; j < number_of_operations; j++)
    // {
    //   int type = abs(rand()) % 2;
    //   int num = rand() % max_element;
    //   if (type == 0) // insert
    //   {
    //     cout << "Insert " << num << endl;
    //   }
    //   else // delete
    //   {
    //     if (number_of_deletions > deletions[i] * number_of_operations)
    //     {
    //       --j;
    //       continue;
    //     }
    //     ++number_of_deletions;
    //     cout << "Delete " << num << endl;
    //   }
    // }
  }

  return 0;
}