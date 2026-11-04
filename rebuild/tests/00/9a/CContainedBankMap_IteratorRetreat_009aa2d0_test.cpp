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
    node->color_ = true;
    return node;
}

int main()
{
    Node* head = makeNode();
    Node* root = makeNode();
    Node* left = makeNode();
    Node* leftRight = makeNode();
    Node* right = makeNode();
    head->color_ = false;
    head->parent_ = root;
    head->left_ = left;
    head->right_ = right;
    root->parent_ = head;
    root->left_ = left;
    root->right_ = right;
    left->parent_ = root;
    left->right_ = leftRight;
    leftRight->parent_ = left;
    right->parent_ = root;

    Iterator it(head);
    if (&--it != &it || it.node_ != right) return 1;
    it.node_ = root;
    if (&--it != &it || it.node_ != leftRight) return 2;
    it.node_ = right;
    if (&--it != &it || it.node_ != root) return 3;
    it.node_ = leftRight;
    if (&--it != &it || it.node_ != left) return 4;

    free(right);
    free(leftRight);
    free(left);
    free(root);
    free(head);
    puts("CONTAINED_BANK_ITERATOR_RETREAT_PASS");
    return 0;
}
