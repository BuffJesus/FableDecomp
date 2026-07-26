#include "rebuild_abi.h"
#include <stdio.h>
#include <stdlib.h>

struct list_node
{
    list_node* _Next; // +0x00
    list_node* _Prev; // +0x04
};

struct list_hdr
{
    list_node* _Myhead; // +0x00
};

static int g_freed = 0;

extern "C" void FABLE_CDECL list_node_free(list_node* _Node)
{
    ++g_freed;
    free(_Node);
}

void FABLE_FASTCALL sub_4367c0(list_hdr* thisptr);

int main(void)
{
    list_node* sentinel = (list_node*)malloc(sizeof(list_node));
    list_node* a = (list_node*)malloc(sizeof(list_node));
    list_node* b = (list_node*)malloc(sizeof(list_node));
    list_node* c = (list_node*)malloc(sizeof(list_node));

    // sentinel <-> a <-> b <-> c <-> sentinel
    sentinel->_Next = a; a->_Prev = sentinel;
    a->_Next = b;        b->_Prev = a;
    b->_Next = c;        c->_Prev = b;
    c->_Next = sentinel; sentinel->_Prev = c;

    list_hdr hdr;
    hdr._Myhead = sentinel;

    sub_4367c0(&hdr);

    int ok = (g_freed == 3)
          && (sentinel->_Next == sentinel)
          && (sentinel->_Prev == sentinel);

    if (ok)
        printf("PASS_004367c0_OK\n");
    else
        printf("FAIL_004367c0 freed=%d\n", g_freed);

    free(sentinel);
    return 0;
}