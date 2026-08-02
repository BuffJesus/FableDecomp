#pragma optimize("s",on)

struct CMemoryArea
{
    int unused0;
    CMemoryArea* parent;
    CMemoryArea* right;
    CMemoryArea* left;
};

void RotateLeft(CMemoryArea* x, CMemoryArea** root)
{
    CMemoryArea* y = x->right;
    x->right = y->left;
    if (y->left)
    {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x == *root)
    {
        *root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}