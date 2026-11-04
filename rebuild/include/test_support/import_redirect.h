#pragma once
// Test-only, process-local import redirection. Never included by reconstructed sources.
#include "rebuild_abi.h"
#include <string.h>
extern "C" __declspec(dllimport) int FABLE_STDCALL VirtualProtect(void*, unsigned long, unsigned long, unsigned long*);
extern "C" char __ImageBase;
namespace FableTest {
class ImportRedirect {
public:
    ImportRedirect(const char* name, void* replacement) : slot_(0), previous_(0) {
        char* base = &__ImageBase;
        char* nt = base + *reinterpret_cast<unsigned long*>(base + 0x3C);
        unsigned long rva = *reinterpret_cast<unsigned long*>(nt + 0x80);
        if (!rva) return;
        for (unsigned long* entry = reinterpret_cast<unsigned long*>(base + rva); entry[3]; entry += 5) {
            if (!entry[0]) continue; // A resolved IAT cannot be used as an import-name table.
            unsigned long* names = reinterpret_cast<unsigned long*>(base + entry[0]);
            void** slots = reinterpret_cast<void**>(base + entry[4]);
            for (int i = 0; names[i]; ++i) {
                if (!(names[i] & 0x80000000UL) && strcmp(base + names[i] + 2, name) == 0) {
                    unsigned long oldProtect;
                    if (!VirtualProtect(&slots[i], sizeof(void*), 4, &oldProtect)) return;
                    previous_ = slots[i];
                    slot_ = &slots[i];
                    *slot_ = replacement;
                    VirtualProtect(slot_, sizeof(void*), oldProtect, &oldProtect);
                    return;
                }
            }
        }
    }
    ~ImportRedirect() {
        if (slot_) {
            unsigned long oldProtect;
            if (VirtualProtect(slot_, sizeof(void*), 4, &oldProtect)) {
                *slot_ = previous_;
                VirtualProtect(slot_, sizeof(void*), oldProtect, &oldProtect);
            }
        }
    }
    bool Installed() const { return slot_ != 0; }
private:
    ImportRedirect(const ImportRedirect&);
    ImportRedirect& operator=(const ImportRedirect&);
    void** slot_;
    void* previous_;
};
}
