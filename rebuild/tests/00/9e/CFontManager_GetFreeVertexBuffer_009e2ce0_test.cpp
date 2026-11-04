#include <stdio.h>
#include <string.h>
#include "fable_system_manager.h"
struct CFontManagerTestAccess {
    static void set(CFontManager& manager, CVertexBufferWin32** slots, unsigned count, unsigned current) {
        void** vectorWords = reinterpret_cast<void**>(&manager.VertexBuffers);
        vectorWords[0] = slots; vectorWords[1] = slots + count; vectorWords[2] = slots + count;
        manager.CurrentVB = current;
    }
    static unsigned current(const CFontManager& manager) { return manager.CurrentVB; }
};
int main() {
    unsigned long storage[32]; memset(storage, 0xA5, sizeof(storage));
    CFontManager& manager = *reinterpret_cast<CFontManager*>(storage);
    CVertexBufferWin32* slots[3] = {
        reinterpret_cast<CVertexBufferWin32*>(0x10100),
        reinterpret_cast<CVertexBufferWin32*>(0x20200),
        reinterpret_cast<CVertexBufferWin32*>(0x30300)
    };
    CFontManagerTestAccess::set(manager, slots, 3, 0);
    for (unsigned i = 0; i < 9; ++i) {
        unsigned expected = (i + 1) % 3;
        if (&manager.GetFreeVertexBuffer() != slots[expected]) return 1;
        if (CFontManagerTestAccess::current(manager) != expected) return 2;
    }
    CFontManagerTestAccess::set(manager, slots, 3, 0xffffffffu);
    if (&manager.GetFreeVertexBuffer() != slots[0] || CFontManagerTestAccess::current(manager) != 0) return 3;
    puts("FONT_VB_009e2ce0_PASS"); return 0;
}