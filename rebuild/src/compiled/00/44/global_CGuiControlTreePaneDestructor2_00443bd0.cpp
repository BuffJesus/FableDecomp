#include <stdlib.h>

void __cdecl operator delete(void* memory);

struct GuiTreeStorageOverlay
{
    void* memory;
    void SortTreeRecursively();
};

struct GuiControlTreePaneOverlay
{
    unsigned char unused[0x64];
    GuiTreeStorageOverlay tree;

    void BaseDestructor();
    GuiControlTreePaneOverlay* CGuiControlTreePane_Destructor_2(unsigned char flags);
};

GuiControlTreePaneOverlay* GuiControlTreePaneOverlay::CGuiControlTreePane_Destructor_2(unsigned char flags)
{
    tree.SortTreeRecursively();
    void* memory = tree.memory;
    if (memory != 0) {
        free(memory);
    }
    BaseDestructor();
    if ((flags & 1) != 0) {
        operator delete(this);
    }
    return this;
}