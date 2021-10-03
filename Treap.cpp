#include <bits/stdc++.h>
#include "Treap.h"

using namespace std;

int main()
{
  srand(time(0));
  Treap *treap = new Treap();

  for (int i = 1; i <= 10; i++)
  {
    treap->Treap_Insert(i);
  }

  treap->Treap_Print("test");

  delete (treap);
}