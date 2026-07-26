#include <cstdio>
struct AutoTinyTreeNode
{
    char reserved[8];
    AutoTinyTreeNode* next;
    AutoTinyTreeNode* child;
};
struct AutoTinyTreeWalker { char unused; };
struct AutoTinyTreeNodeArgument { AutoTinyTreeNode* node; };
static int g_AutoTinyFreedTreeNodes = 0;
extern "C" void __cdecl AutoTinyFreeTreeNode(AutoTinyTreeNode*)
{
    ++g_AutoTinyFreedTreeNodes;
}
void __fastcall BuildTreeArray(AutoTinyTreeWalker* self, AutoTinyTreeNodeArgument argument)
{
    AutoTinyTreeNode* node = argument.node;
    while (node != 0)
    {
        AutoTinyTreeNodeArgument child = {node->child};
        BuildTreeArray(self, child);
        AutoTinyTreeNode* next = node->next;
        AutoTinyFreeTreeNode(node);
        node = next;
    }
}
int main()
{
    AutoTinyTreeNode child = {{0}, 0, 0};
    AutoTinyTreeNode sibling = {{0}, 0, 0};
    AutoTinyTreeNode root = {{0}, &sibling, &child};
    AutoTinyTreeWalker walker = {0};
    AutoTinyTreeNodeArgument argument = {&root};
    BuildTreeArray(&walker, argument);
    if (g_AutoTinyFreedTreeNodes != 3)
        return 1;
    std::printf("AUTO_TINY_004da40e_TEST PASS\n");
    return 0;
}