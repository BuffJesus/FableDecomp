#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fable_retail_banks.h"

typedef NBankFileManager::CContainedBankMap::CNode Node;
typedef NBankFileManager::CContainedBankMap::iterator Iterator;

static Node* makeNode()
{
    Node* node = static_cast<Node*>(malloc(sizeof(Node)));
    memset(node, 0, sizeof(Node));
    return node;
}

int main()
{
    Node* head = makeNode();
    Node* root = makeNode();
    Node* left = makeNode();
    Node* right = makeNode();
    Node* rightLeft = makeNode();
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
    it.node_ = root;
    if (&++it != &it || it.node_ != rightLeft) return 2;
    it.node_ = rightLeft;
    if (&++it != &it || it.node_ != right) return 3;
    it.node_ = right;
    if (&++it != &it || it.node_ != head) return 4;

    free(rightLeft);
    free(right);
    free(left);
    free(root);
    free(head);
    puts("CONTAINED_BANK_ITERATOR_ADVANCE_PASS");
    return 0;
}
