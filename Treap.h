#include <iostream>
#include <fstream>
#include <time.h>
#include <bits/stdc++.h>

using namespace std;

// class for Treap node
class Treap_Node
{
private:
	int key, priority;
	Treap_Node *LChild, *RChild;
	friend class Treap;

public:
	// parameterized constructor
	Treap_Node(int k, int p)
	{
		key = k;
		priority = p;
		LChild = RChild = NULL;
	}
};

//========================================================================================================================

class Treap
{
private:
	Treap_Node *root;
	unordered_set<int> all_used_priorities;
	int max_priority = 1000000;
	void RotateR(Treap_Node *&ptr);
	void RotateL(Treap_Node *&ptr);
	void Treap_PrintHelper(const Treap_Node *node, ofstream &fout);
	void destructorHelper(Treap_Node *treap_node);

public:
	Treap();
	Treap(int k);
	Treap(const Treap &T);
	void Treap_Insert(int k);
	void Treap_Delete(int k);
	bool Treap_Search(int k);
	void Treap_Print(const char *filename);
	~Treap();
};

//========================================================================================================================

Treap::Treap()
{
	all_used_priorities.clear();
	root = NULL;
}

//========================================================================================================================

Treap::Treap(int k)
{
	all_used_priorities.clear();
	int p = ((rand() % max_priority) + max_priority) % max_priority;
	root = new Treap_Node(k, p);
	all_used_priorities.insert(p);
}

//========================================================================================================================

void Treap::RotateR(Treap_Node *&ptr)
{
	Treap_Node *subR = ptr;
	ptr = ptr->LChild;
	subR->LChild = ptr->RChild;
	ptr->RChild = subR;
}

//========================================================================================================================

void Treap::RotateL(Treap_Node *&ptr)
{
	Treap_Node *subL = ptr;
	ptr = ptr->RChild;
	subL->RChild = ptr->LChild;
	ptr->LChild = subL;
}

//========================================================================================================================

void Treap::Treap_Insert(int k)
{
	if (!root)
	{
		int p = ((rand() % max_priority) + max_priority) % max_priority;
		root = new Treap_Node(k, p);
		all_used_priorities.insert(p);
	}

	stack<Treap_Node *> st;

	Treap_Node *node = root;
	Treap_Node *parent = NULL;

	while (node)
	{
		if (k == node->key)
			return;

		st.push(node); // check using valgrind for memory leaks, if found then clear stack
		parent = node;
		if (k < node->key)
		{
			node = node->LChild;
		}
		else
		{
			node = node->RChild;
		}
	}

	int p = ((rand() % max_priority) + max_priority) % max_priority;
	while (all_used_priorities.find(p) != all_used_priorities.end())
	{
		p = ((rand() % max_priority) + max_priority) % max_priority;
	}

	Treap_Node *new_node = new Treap_Node(k, p);
	all_used_priorities.insert(p);

	if (k < parent->key)
		parent->LChild = new_node;
	else
		parent->RChild = new_node;

	while (!st.empty())
	{
		node = st.top();
		st.pop();
		if (node->LChild && node->priority > node->LChild->priority)
		{
			RotateR(node);
			if (st.empty())
			{
				root = node;
			}
			else
			{
				if (k < st.top()->key)
					st.top()->LChild = node;
				else
					st.top()->RChild = node;
			}
		}
		else if (node->RChild && node->priority > node->RChild->priority)
		{
			RotateL(node);
			if (st.empty())
			{
				root = node;
			}
			else
			{
				if (k < st.top()->key)
					st.top()->LChild = node;
				else
					st.top()->RChild = node;
			}
		}
		else
			break;
	}
}

//========================================================================================================================

void Treap::Treap_Delete(int k)
{
	if (k == root->key)
	{
		if (root->LChild && root->RChild)
		{
			if (root->LChild->priority < root->RChild->priority)
			{
				RotateR(root);
			}
			else
			{
				RotateL(root);
			}
		}
		else if (root->LChild)
		{
			RotateR(root);
		}
		else if (root->RChild)
		{
			RotateL(root);
		}
		else
		{
			delete (root);
			root = NULL;
			return;
		}
	}

	Treap_Node *node = root;
	Treap_Node *parent = NULL;

	while (node)
	{
		if (k == node->key)
			break;

		parent = node;
		if (k < node->key)
		{
			node = node->LChild;
		}
		else
		{
			node = node->RChild;
		}
	}

	if (!node)
		return;

	all_used_priorities.erase(node->priority);
	node->priority = INT_MAX;

	if (node->LChild || node->RChild)
	{
		while (node->LChild && node->RChild)
		{
			if (node->LChild->priority < node->RChild->priority)
			{
				RotateR(node);
				if (node->key < parent->key)
					parent->LChild = node;
				else
					parent->RChild = node;
				parent = node;
				node = node->RChild;
			}
			else
			{
				RotateL(node);
				if (node->key < parent->key)
					parent->LChild = node;
				else
					parent->RChild = node;
				parent = node;
				node = node->LChild;
			}
		}
		if (node->LChild)
		{
			RotateR(node);
			if (node->key < parent->key)
				parent->LChild = node;
			else
				parent->RChild = node;
			parent = node;
			node = node->RChild;
		}
		else
		{
			RotateL(node);
			if (node->key < parent->key)
				parent->LChild = node;
			else
				parent->RChild = node;
			parent = node;
			node = node->LChild;
		}
	}

	if (node->key < parent->key)
		parent->LChild = NULL;
	else
		parent->RChild = NULL;
	delete (node);
}

//========================================================================================================================

bool Treap::Treap_Search(int k)
{
	Treap_Node *node = root;
	while (node)
	{
		if (k == node->key)
			return true;
		if (k < node->key)
			node = node->LChild;
		else
			node = node->RChild;
	}
	return false;
}

//========================================================================================================================

// function to print the Treap in a png file
void Treap::Treap_Print(const char *filename)
{
	ofstream fout;

	string dot_file = "";
	dot_file = dot_file + filename + ".dot"; // name of graphviz file

	string png_file = "";
	png_file = png_file + filename + ".png"; // name of png file

	fout.open(dot_file.c_str()); // open dot file for writing

	fout << "digraph g {\n";
	fout << "node [shape=record, height=0.1];\n";

	Treap_PrintHelper(root, fout);

	fout << "}";
	fout.close(); // close dot file

	string str = "dot -Tpng ";
	str = str + dot_file + " -o " + png_file;

	const char *command = str.c_str();

	system(command); // system call to run the dot file using graphviz

	cout << "Tree Printed Successfully! Please check the " << png_file << " file.\n";
}

//========================================================================================================================

// helper function to traverse the Treap in preorder fashion
// and print the edges in the required format in the dot file
void Treap::Treap_PrintHelper(const Treap_Node *node, ofstream &fout)
{
	if (!node) // if node is NULL
		return;

	if (node == root) // add the label and root in the dot file
	{
		fout << "label = \" rooted at " << node->key << " \";\n";
		fout << node->key << " [root = true]\n";
	}

	fout << node->key << " [label=\"<f0>|<f1>" << node->key << "|<f2> " << node->priority << " |<f3> \"];\n";

	if (node->LChild) // if left child exists
	{
		fout << node->key << ":f0 -> " << node->LChild->key << ":f1\n"; // write edge in dot file
		Treap_PrintHelper(node->LChild, fout);													// recurse for left subtree
	}

	if (node->RChild) // if right child exists
	{
		fout << node->key << ":f3 -> " << node->RChild->key << ":f1\n"; // write edge in dot file
		Treap_PrintHelper(node->RChild, fout);													// recurse for right subtree
	}
}

//========================================================================================================================

Treap::~Treap()
{
	destructorHelper(root);
}

//========================================================================================================================

// helper function to traverse the Treap in post order
// fashion and delete the nodes one by one
void Treap::destructorHelper(Treap_Node *treap_node)
{
	if (!treap_node) // if node is NULL
		return;
	if (treap_node->LChild) // node has left child
		destructorHelper(treap_node->LChild);

	if (treap_node->RChild) // node has right child
		destructorHelper(treap_node->RChild);

	delete (treap_node); // deallocate memory for node
}

//========================================================================================================================