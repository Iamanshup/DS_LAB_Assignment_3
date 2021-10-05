#include <bits/stdc++.h>
#include "Treap.h"

using namespace std;

int main()
{
	srand(time(0));
	Treap *treap = new Treap();

	// for (int i = 1; i <= 10; i++)
	// {
	//   treap->Treap_Insert(i);
	// }

	// treap->Treap_Print("test");

	int c = 0;

	while (c != -1)
	{
		int num;
		string filename;
		cout << "MENU:\n";
		cout << "1. Insert(x)\n2. Search(x)\n3. Delete(x)\n4. Print Treap\n5. Exit\n";
		cout << "Enter your choice: ";
		cin >> c;
		switch (c)
		{
		case 1:
			cout << "Enter a number: ";
			cin >> num;
			treap->Treap_Insert(num);
			cout << "Key inserted successfully!\n";
			break;

		case 2:
			cout << "Enter a number: ";
			cin >> num;
			if (treap->Treap_Search(num))
				cout << "Number is present in Treap.\n";
			else
				cout << "Number is not present in Treap.\n";
			break;

		case 3:
			cout << "Enter a number: ";
			cin >> num;
			treap->Treap_Delete(num);
			cout << "Key deleted successfully!\n";
			break;

		case 4:
			try
			{
				cout << "Enter filename(without extension): ";
				cin >> filename;
				treap->Treap_Print(filename.c_str());
			}
			catch (...)
			{
				cout << "Some error occurred!\n";
			}
			break;

		case 5:
			c = -1;
			break;

		default:
			cout << "Invalid Choice!\n Try again.\n";
			break;
		}
		cout << "\n\n";
	}

	delete (treap);
}