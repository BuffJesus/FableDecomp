#include <cstdio>
struct TreeNode { TreeNode* dummy0; TreeNode* parent; TreeNode* left; TreeNode* right; int key; };
struct Tree { TreeNode* header; TreeNode* RBTree_LowerBound(int* keyp); };
TreeNode* Tree::RBTree_LowerBound(int* keyp) {
    TreeNode* bound = header;
    TreeNode* n = header->parent;
    if (n) {
        int k = *keyp;
        do {
            if (n->key >= k) { bound = n; n = n->left; }
            else { n = n->right; }
        } while (n);
    }
    return bound;
}
static TreeNode* mk(int key){ TreeNode* t=new TreeNode(); t->dummy0=0;t->parent=0;t->left=0;t->right=0;t->key=key; return t; }
int main(){
    TreeNode* n10=mk(10); TreeNode* n30=mk(30); TreeNode* n50=mk(50);
    n30->left=n10; n30->right=n50;
    TreeNode header; header.dummy0=0; header.parent=n30; header.left=0; header.right=0; header.key=0;
    Tree tree; tree.header=&header;
    int k=30;  TreeNode* r  = tree.RBTree_LowerBound(&k);
    int k2=10; TreeNode* r2 = tree.RBTree_LowerBound(&k2);
    int k3=40; TreeNode* r3 = tree.RBTree_LowerBound(&k3);
    int k4=100;TreeNode* r4 = tree.RBTree_LowerBound(&k4);
    int k5=5;  TreeNode* r5 = tree.RBTree_LowerBound(&k5);
    if (r==n30 && r2==n10 && r3==n50 && r4==&header && r5==n10) {
        printf("OK_0x005bac73\n");
        return 0;
    }
    printf("BAD r=%p r2=%p r3=%p r4=%p r5=%p\n", (void*)r,(void*)r2,(void*)r3,(void*)r4,(void*)r5);
    return 1;
}