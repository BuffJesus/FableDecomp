#include <stdio.h>
#include <stdlib.h>
#include "rebuild_abi.h"

struct TreeNode
{
    TreeNode* next;
    TreeNode* prev;
};

struct TreeList
{
    TreeNode* head;
};

static int g_deletes = 0;

extern "C" void FABLE_CDECL fable_op_delete_7c8114(void* p)
{
    ++g_deletes;
    free(p);
}

void FABLE_FASTCALL SortTreeRecursively_00436900(TreeList* thisptr);

int main(void)
{
    // Build a circular list: head <-> a <-> b <-> head
    TreeNode* head = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode* a = (TreeNode*)malloc(sizeof(TreeNode));
    TreeNode* b = (TreeNode*)malloc(sizeof(TreeNode));

    head->next = a;  head->prev = b;
    a->next = b;     a->prev = head;
    b->next = head;  b->prev = a;

    TreeList list;
    list.head = head;

    SortTreeRecursively_00436900(&list);

    // Expect a, b, and head all freed => 3 deletes.
    if (g_deletes == 3)
        printf("OK_00436900_3\n");
    else
        printf("FAIL_%d\n", g_deletes);

    return 0;
}