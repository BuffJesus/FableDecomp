#include <stdlib.h>

void __cdecl operator delete(void* memory);

struct GuiSortStateOverlay
{
    void SortTreeRecursively();
};

struct GuiTreeStorageOverlay5
{
    void* memory;
    void SortTreeRecursively();
};

struct GuiControlTreePaneOverlay5
{
    unsigned char unused[0x64];
    GuiTreeStorageOverlay5 tree;
    GuiSortStateOverlay state;

    void BaseDestructor();
};

void __fastcall CGuiControlTreePane_Destructor_5(GuiControlTreePaneOverlay5* self)
{
    if (self != 0) {
        self->state.SortTreeRecursively();
        self->tree.SortTreeRecursively();
        void* memory = self->tree.memory;
        if (memory != 0) {
            free(memory);
        }
        self->BaseDestructor();
        operator delete(self);
    }
}