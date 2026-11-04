#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef CBankFileManager::CBankInfoMap::CNode Node;
typedef CBankFileManager::CBankInfoMap::iterator Iterator;

static Node* makeNode()
{
    Node* node = static_cast<Node*>(malloc(sizeof(Node)));
    memset(node, 0, sizeof(Node));
    node->color_ = true;
    return node;
}

int main()
{
    Node* head = makeNode();
    Node* root = makeNode();
    Node* left = makeNode();
    Node* right = makeNode();
    Node* rightLeft = makeNode();
    head->color_ = false;
    head->parent_ = root;
    head->left_ = left;
    head->right_ = right;
    root->parent_ = head;
    root->left_ = left;
    root->right_ = right;
    left->parent_ = root;
    right->parent_ = root;
    right->left_ = rightLeft;
    rightLeft->parent_ = right;

    Iterator it(left);
    if (&++it != &it || it.node_ != root) return 1;
    if (&++it != &it || it.node_ != rightLeft) return 2;
    if (&++it != &it || it.node_ != right) return 3;
    if (&++it != &it || it.node_ != head) return 4;
    if (&--it != &it || it.node_ != right) return 5;
    if (&--it != &it || it.node_ != rightLeft) return 6;
    if (&--it != &it || it.node_ != root) return 7;
    if (&--it != &it || it.node_ != left) return 8;

    free(rightLeft);
    free(right);
    free(left);
    free(root);
    free(head);
    puts("BANK_INFO_ITERATORS_PASS");
    return 0;
}
