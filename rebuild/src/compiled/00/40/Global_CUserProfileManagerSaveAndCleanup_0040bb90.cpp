#include <stdlib.h>

class CUserProfileManager
{
public:
    void Save();
};

struct CUserProfileManagerSaveOverlay
{
    unsigned char pad00[0x8c];
    unsigned long sourceCursor;
    unsigned long savedCursor;
    unsigned char pad94[4];
    unsigned long cursor;
    void* activeBlock;
    unsigned long blockEnd;
    unsigned long blockLinks;
    unsigned long endCursor;
};

void __fastcall CUserProfileManager_SaveAndCleanup(CUserProfileManager* manager)
{
    CUserProfileManagerSaveOverlay* self =
        reinterpret_cast<CUserProfileManagerSaveOverlay*>(manager);

    self->savedCursor = self->sourceCursor;
    manager->Save();

    while (self->cursor != self->endCursor)
    {
        if (self->cursor != self->blockEnd - 4)
        {
            self->cursor += 4;
            continue;
        }

        if (self->activeBlock != 0)
            free(self->activeBlock);

        self->blockLinks += 4;
        const unsigned long nextBlock =
            *reinterpret_cast<unsigned long*>(self->blockLinks);
        self->activeBlock = reinterpret_cast<void*>(nextBlock);
        self->blockEnd = nextBlock + 0x80;
        self->cursor = nextBlock;
    }
}
