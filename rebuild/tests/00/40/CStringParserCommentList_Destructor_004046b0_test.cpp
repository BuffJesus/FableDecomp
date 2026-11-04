#include <stdio.h>
#include <string.h>

#include <new>

#include "fable_string_parser.h"

// Link the reconstructed COMDAT instead of instantiating a local copy.
#pragma warning(disable : 4231)
extern template std::list<CStringParserCommentBoundaries>::~list();

typedef std::list<CStringParserCommentBoundaries> CCommentList;
typedef std::_List_node<CStringParserCommentBoundaries> CCommentNode;

namespace
{
    void* g_freed[4];
    unsigned int g_freeCount;
    CCommentNode* g_sentinel;
    bool g_sentinelWasRelinked;

    void ResetObservations(CCommentNode* sentinel)
    {
        memset(g_freed, 0, sizeof(g_freed));
        g_freeCount = 0;
        g_sentinel = sentinel;
        g_sentinelWasRelinked = false;
    }

    // Comments owns its sentinel through a single pointer member.
    CCommentNode*& Sentinel(CCommentList& list)
    {
        return *reinterpret_cast<CCommentNode**>(&list);
    }

    bool CheckPopulatedList()
    {
        CCommentNode first;
        CCommentNode second;
        void* storage[1];
        CCommentList* list = new (storage) CCommentList;
        CCommentNode* sentinel = Sentinel(*list);
        sentinel->_M_next = &first;
        sentinel->_M_prev = &second;
        first._M_next = &second;
        first._M_prev = sentinel;
        second._M_next = sentinel;
        second._M_prev = &first;

        ResetObservations(sentinel);
        list->~CCommentList();

        return
            g_freeCount == 3 &&
            g_freed[0] == &first &&
            g_freed[1] == &second &&
            g_freed[2] == sentinel &&
            g_sentinelWasRelinked;
    }

    bool CheckEmptyList()
    {
        void* storage[1];
        CCommentList* list = new (storage) CCommentList;
        CCommentNode* sentinel = Sentinel(*list);

        ResetObservations(sentinel);
        list->~CCommentList();

        return
            g_freeCount == 1 &&
            g_freed[0] == sentinel &&
            g_sentinelWasRelinked;
    }
}

// Records instead of releasing; the fixture's few blocks are left to exit.
extern "C" void __cdecl free(void* block)
{
    if (block == 0)
    {
        return;
    }
    if (g_freeCount < 4)
    {
        g_freed[g_freeCount] = block;
    }
    ++g_freeCount;
    if (block == g_sentinel)
    {
        g_sentinelWasRelinked =
            g_sentinel->_M_next == g_sentinel &&
            g_sentinel->_M_prev == g_sentinel;
    }
}

int main()
{
    const bool passed =
        CheckPopulatedList() &&
        CheckEmptyList();

    if (!passed)
    {
        printf("FABLETLC_STRING_PARSER_COMMENT_LIST_DTOR FAIL\n");
        return 1;
    }

    printf("FABLETLC_STRING_PARSER_COMMENT_LIST_DTOR PASS\n");
    return 0;
}
