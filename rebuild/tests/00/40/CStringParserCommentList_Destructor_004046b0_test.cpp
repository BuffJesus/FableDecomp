#include <stdio.h>
#include <string.h>

#include "rebuild_abi.h"

struct FableStringParserCommentNode_004046B0
{
    FableStringParserCommentNode_004046B0* next;
    FableStringParserCommentNode_004046B0* previous;
};

struct FableStringParserCommentList_004046B0
{
    FableStringParserCommentNode_004046B0* head;
};

extern "C" void FABLE_FASTCALL
FableStringParserCommentListDestroy_004046B0(
    FableStringParserCommentList_004046B0* list);

namespace
{
    void* g_deleted[4];
    unsigned int g_deleteCount;
    FableStringParserCommentNode_004046B0* g_expectedSentinel;
    bool g_sentinelWasRelinked;

    void Reset()
    {
        memset(g_deleted, 0, sizeof(g_deleted));
        g_deleteCount = 0;
        g_expectedSentinel = 0;
        g_sentinelWasRelinked = false;
    }

    bool CheckPopulatedList()
    {
        FableStringParserCommentNode_004046B0 sentinel;
        FableStringParserCommentNode_004046B0 first;
        FableStringParserCommentNode_004046B0 second;
        sentinel.next = &first;
        sentinel.previous = &second;
        first.next = &second;
        first.previous = &sentinel;
        second.next = &sentinel;
        second.previous = &first;

        FableStringParserCommentList_004046B0 list = {&sentinel};
        Reset();
        g_expectedSentinel = &sentinel;
        FableStringParserCommentListDestroy_004046B0(&list);

        return
            g_deleteCount == 3 &&
            g_deleted[0] == &first &&
            g_deleted[1] == &second &&
            g_deleted[2] == &sentinel &&
            g_sentinelWasRelinked;
    }

    bool CheckEmptyList()
    {
        FableStringParserCommentNode_004046B0 sentinel;
        sentinel.next = &sentinel;
        sentinel.previous = &sentinel;
        FableStringParserCommentList_004046B0 list = {&sentinel};

        Reset();
        g_expectedSentinel = &sentinel;
        FableStringParserCommentListDestroy_004046B0(&list);

        return
            g_deleteCount == 1 &&
            g_deleted[0] == &sentinel &&
            g_sentinelWasRelinked;
    }

    bool CheckNullSentinel()
    {
        FableStringParserCommentList_004046B0 list = {0};
        Reset();
        FableStringParserCommentListDestroy_004046B0(&list);
        return g_deleteCount == 0;
    }
}

extern "C" void FABLE_CDECL
FableStringParserCommentNodeDelete_004046B0(void* node)
{
    if (g_deleteCount < 4)
    {
        g_deleted[g_deleteCount] = node;
    }
    ++g_deleteCount;
    if (node == g_expectedSentinel)
    {
        g_sentinelWasRelinked =
            g_expectedSentinel->next == g_expectedSentinel &&
            g_expectedSentinel->previous == g_expectedSentinel;
    }
}

int main()
{
    // The retail routine assumes a non-null sentinel while entering its walk.
    // The final null check only protects the operator-delete call, so exercise
    // populated and empty valid lists here.
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
