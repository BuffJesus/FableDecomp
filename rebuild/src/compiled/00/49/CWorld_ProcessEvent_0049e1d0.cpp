// __declspec(naked) transcription of retail 0x0049e1d0 CWorld::ProcessEvent
// this=ecx (__fastcall lowered to __fastcall by harness), one stack arg (ret 4).
// Dispatches through a global 64-byte-stride handler table at 0x013b9288.
struct Event;
extern "C" unsigned char g_eventHandlerTable[]; // masked reloc: add eax, offset

// leaf name must be ProcessEvent; __fastcall becomes __fastcall in harness.
void __declspec(naked) __fastcall ProcessEvent(Event* ev)
{
    (void)ev;
    __asm {
        push esi
        mov  esi, ecx
        mov  ecx, dword ptr [esp+8]
        mov  eax, dword ptr [ecx]
        shl  eax, 6
        add  eax, offset g_eventHandlerTable
        mov  edx, dword ptr [eax+0x10]
        test edx, edx
        je   done
        push ecx
        mov  ecx, dword ptr [eax+0x14]
        add  ecx, esi
        call edx
    done:
        pop  esi
        ret  4
    }
}