#include <bits/stdc++.h>
#include "Treap.h"

using namespace std;

int main()
{
	srand(time(0));
	Treap *treap = new Treap();

	int c = 0;

	while (c != -1)
	{
		int num;
		string filename;
		cout << "MENU:\n";
		cout << "1. Insert(x)\n2. Search(x)\n3. Delete(x)\n4. Height of Treap\n5. Average Height of Nodes\n6. Print Treap\n7. Exit\n";
		cout << "Enter your choice: ";
		cin >> c;
		switch (c)
		{
		case 1:
			cout << "Enter a number: ";
			cin >> num;
			if (treap->Treap_Search(num))
			{
				cout << "Key is already present in the Treap.\n";
			}
			else
			{
				treap->Treap_Insert(num);
				cout << "Key inserted successfully!\n";
			}
			break;

		case 2:
			cout << "Enter a number: ";
			cin >> num;
			if (treap->Treap_Search(num))
				cout << "Key is present in Treap.\n";
			else
				cout << "Key is not present in Treap.\n";
			break;

		case 3:
			cout << "Enter a number: ";
			cin >> num;
			if (!treap->Treap_Search(num))
			{
				cout << "Key is not present in the Treap.\n";
			}
			else
			{
				treap->Treap_Delete(num);
				cout << "Key deleted successfully!\n";
			}
			break;

		case 4:
			cout << "Height of Treap: " << treap->Treap_Height() << "\n";
			break;

		case 5:
			cout << "Average Height of Nodes: " << treap->Treap_Average_Height_Of_Nodes() << "\n";
			break;

		case 6:
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

		case 7:
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