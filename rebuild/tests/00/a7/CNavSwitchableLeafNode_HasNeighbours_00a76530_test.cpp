#include <cstdio>

struct Node {
  unsigned char f_0f;
  unsigned char f_18;
};

static bool HasNeighbours(const Node* n)
{
  if (!n->f_0f)
    return n->f_18 != 0;
  return false;
}

int main()
{
  Node a = {0, 0};
  Node b = {0, 1};
  Node c = {1, 0};
  Node d = {1, 1};
  bool ok = (HasNeighbours(&a) == false)
         && (HasNeighbours(&b) == true)
         && (HasNeighbours(&c) == false)
         && (HasNeighbours(&d) == false);
  if (ok) { printf("OK_00a76530\n"); return 0; }
  printf("FAIL_00a76530\n");
  return 1;
}