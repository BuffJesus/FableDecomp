// CObserver::ObserveEvent @ 0x0052DA20.
//
// This is the typed observer-base companion immediately following the
// CFrontEndManager initializer in the 00/52 shard.  Retail performs both
// collection lookups first, then dispatches event 0x19 through virtual slot 1
// only when the first lookup resolves to the collection sentinel.

class CObserver_0052da20
{
public:
    void** vtable;
    void* collection_sentinel;

    void ObserveEvent(int event);
};

extern "C" void __fastcall
ObserverLookupFirst_0052da20(
    void* collection,
    void*,
    void** result,
    const int* event);

extern "C" void __fastcall
ObserverLookupSecond_0052da20(
    void* collection,
    void*,
    void** result,
    const int* event);

__declspec(naked)
void CObserver_0052da20::ObserveEvent(int)
{
    __asm
    {
        sub esp, 0ch
        push ebx
        push esi
        push edi
        mov edi, ecx
        lea eax, dword ptr [esp + 1ch]
        push eax
        lea ecx, dword ptr [esp + 10h]
        lea esi, dword ptr [edi + 4]
        push ecx
        mov ecx, esi
        call ObserverLookupFirst_0052da20
        mov ebx, dword ptr [esi]
        lea edx, dword ptr [esp + 1ch]
        push edx
        lea eax, dword ptr [esp + 14h]
        push eax
        mov ecx, esi
        call ObserverLookupSecond_0052da20
        cmp dword ptr [esp + 1ch], 19h
        jne done
        cmp dword ptr [esp + 0ch], ebx
        jne done
        mov edx, dword ptr [edi]
        push 19h
        mov ecx, edi
        call dword ptr [edx + 4]

    done:
        pop edi
        pop esi
        pop ebx
        add esp, 0ch
        ret 4
    }
}
