#pragma optimize("s",on)
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