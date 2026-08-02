
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

#include <cstdio>

int main()
{
    CMemoryArea nodeX;
    CMemoryArea nodeY;
    CMemoryArea nodeYLeft;
    CMemoryArea nodeParent;

    // Case 1: x is root, y->left exists.
    nodeX.unused0 = 0;
    nodeX.parent = 0;
    nodeX.left = 0;
    nodeX.right = &nodeY;

    nodeY.unused0 = 0;
    nodeY.parent = &nodeX;
    nodeY.left = &nodeYLeft;
    nodeY.right = 0;

    nodeYLeft.unused0 = 0;
    nodeYLeft.parent = &nodeY;
    nodeYLeft.left = 0;
    nodeYLeft.right = 0;

    CMemoryArea* root = &nodeX;

    RotateLeft(&nodeX, &root);

    bool ok = true;
    if (root != &nodeY) ok = false;
    if (nodeY.parent != 0) ok = false;
    if (nodeY.left != &nodeX) ok = false;
    if (nodeX.parent != &nodeY) ok = false;
    if (nodeX.right != &nodeYLeft) ok = false;
    if (nodeYLeft.parent != &nodeX) ok = false;

    // Case 2: x is a left child of parent, y->left is null.
    CMemoryArea px, py, pparent;
    px.unused0 = 0; px.parent = &pparent; px.left = 0; px.right = &py;
    py.unused0 = 0; py.parent = &px; py.left = 0; py.right = 0;
    pparent.unused0 = 0; pparent.parent = 0; pparent.left = &px; pparent.right = 0;

    CMemoryArea* root2 = &pparent;
    RotateLeft(&px, &root2);

    if (pparent.left != &py) ok = false;
    if (py.parent != &pparent) ok = false;
    if (py.left != &px) ok = false;
    if (px.parent != &py) ok = false;
    if (px.right != 0) ok = false;

    // Case 3: x is a right child of parent.
    CMemoryArea qx, qy, qparent;
    qx.unused0 = 0; qx.parent = &qparent; qx.left = 0; qx.right = &qy;
    qy.unused0 = 0; qy.parent = &qx; qy.left = 0; qy.right = 0;
    qparent.unused0 = 0; qparent.parent = 0; qparent.left = 0; qparent.right = &qx;

    CMemoryArea* root3 = &qparent;
    RotateLeft(&qx, &root3);

    if (qparent.right != &qy) ok = false;
    if (qy.parent != &qparent) ok = false;
    if (qy.left != &qx) ok = false;
    if (qx.parent != &qy) ok = false;

    if (ok)
    {
        printf("ROTATE_LEFT_OK\n");
    }
    else
    {
        printf("ROTATE_LEFT_FAIL\n");
    }
    return 0;
}