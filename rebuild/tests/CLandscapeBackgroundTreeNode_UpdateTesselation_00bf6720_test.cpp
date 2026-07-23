#include <stdio.h>
#include <string.h>
#include "candidates/background_tree_node_compiled.h"

class MockLoadTask : public CLandscapeBackgroundLoadTask
{
public:
    int abortCalls;
    MockLoadTask() : abortCalls(0) {}
    virtual void Slot00() {}
    virtual void Slot04() {}
    virtual void Slot08() {}
    virtual void Slot0C() {}
    virtual void Slot10() {}
    virtual void Abort() { ++abortCalls; }
};

int main()
{
    CLandscapeBackgroundTreeNode node;
    MockLoadTask task;
    memset(&node, 0, sizeof(node));
    node.AsyncLoadTask = &task;
    node.UpdateTesselation();
    if (!node.TesselationValid || task.abortCalls != 1) return 1;
    node.UpdateTesselation();
    if (task.abortCalls != 1) return 2;

    memset(&node, 0, sizeof(node));
    node.UpdateTesselation();
    if (!node.TesselationValid) return 3;

    printf("BACKGROUND_TREE_TESSELATION_TEST PASS\n");
    return 0;
}
