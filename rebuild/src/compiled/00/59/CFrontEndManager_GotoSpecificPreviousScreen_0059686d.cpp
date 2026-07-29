// NUISystem::CFrontEndManager::GotoSpecificPreviousScreen @ 0x0059686D
//
// Address-isolated reconstruction of the retail deque-history operation.
// The iterator and deque declarations preserve the VC7.1 call boundaries
// visible in retail: iterator subtraction, back, pop_back, and push_back.

typedef unsigned int FrontEndUInt_0059686d;

#define FE_PREVIOUS_DECLARE_SLOT(Name) virtual void Name();
#define FE_PREVIOUS_SLOT_BLOCK_0(X) \
    X(Slot00) X(Slot01) X(Slot02) X(Slot03) \
    X(Slot04) X(Slot05) X(Slot06) X(Slot07) \
    X(Slot08) X(Slot09) X(Slot0A) X(Slot0B) \
    X(Slot0C) X(Slot0D) X(Slot0E) X(Slot0F)
#define FE_PREVIOUS_SLOT_BLOCK_1(X) \
    X(Slot10) X(Slot11) X(Slot12) X(Slot13) \
    X(Slot14) X(Slot15) X(Slot16) X(Slot17) \
    X(Slot18) X(Slot19) X(Slot1A) X(Slot1B) \
    X(Slot1C) X(Slot1D) X(Slot1E) X(Slot1F)
#define FE_PREVIOUS_SLOT_BLOCK_2(X) \
    X(Slot20) X(Slot21) X(Slot22) X(Slot23) \
    X(Slot24) X(Slot25) X(Slot26) X(Slot27) \
    X(Slot28) X(Slot29) X(Slot2A) X(Slot2B) \
    X(Slot2C) X(Slot2D) X(Slot2E) X(Slot2F)

class CComponent_0059686d
{
public:
    FE_PREVIOUS_SLOT_BLOCK_0(FE_PREVIOUS_DECLARE_SLOT)
    FE_PREVIOUS_SLOT_BLOCK_1(FE_PREVIOUS_DECLARE_SLOT)
    FE_PREVIOUS_SLOT_BLOCK_2(FE_PREVIOUS_DECLARE_SLOT)
    virtual void SetCurrentState(int state); // vtable +0xc0

    void* ownership; // +0x04
};

class CManager_0059686d
{
public:
    FE_PREVIOUS_DECLARE_SLOT(Slot00)
    FE_PREVIOUS_DECLARE_SLOT(Slot01)
    FE_PREVIOUS_DECLARE_SLOT(Slot02)
    FE_PREVIOUS_DECLARE_SLOT(Slot03)
    FE_PREVIOUS_DECLARE_SLOT(Slot04)
    virtual void ReleaseComponentOwnership(void* ownership); // vtable +0x14

    static CManager_0059686d* GetInstance();
};

class CUserProfileManager_0059686d
{
public:
};

struct FrontEndDequeIterator_0059686d
{
    CComponent_0059686d** current;
    CComponent_0059686d** first;
    CComponent_0059686d** last;
    CComponent_0059686d*** node;

    long operator-(
        const FrontEndDequeIterator_0059686d& other) const;
};

class FrontEndComponentDeque_0059686d
{
public:
    FrontEndDequeIterator_0059686d begin;
    FrontEndDequeIterator_0059686d end;

    CComponent_0059686d*& back();
    void pop_back();
    void push_back(CComponent_0059686d* const& component);

    __forceinline FrontEndUInt_0059686d size() const
    {
        return (FrontEndUInt_0059686d)(end - begin);
    }

    __forceinline bool empty() const
    {
        return begin.current == end.current;
    }
};

class CFrontEndManager_0059686d
{
public:
    char pad000[0x20];
    FrontEndComponentDeque_0059686d previousScreens; // +0x20
    char pad040[0x58];
    CComponent_0059686d* transitionFrom; // +0x98
    CComponent_0059686d* transitionTo;   // +0x9c

    void GotoSpecificPreviousScreen(CComponent_0059686d* component);
};

extern "C" CUserProfileManager_0059686d* __cdecl
FrontEndUserProfileGet_0059686d();
extern "C" void __fastcall FrontEndUserProfileLoad_0059686d(
    CUserProfileManager_0059686d* profile,
    void*);
extern "C" long __fastcall FrontEndDequeDistance_0059686d(
    FrontEndDequeIterator_0059686d* finish,
    void*,
    const FrontEndDequeIterator_0059686d* start);
extern "C" CComponent_0059686d** __fastcall FrontEndDequeBack_0059686d(
    FrontEndComponentDeque_0059686d* history,
    void*);
extern "C" void __fastcall FrontEndDequePopBack_0059686d(
    FrontEndComponentDeque_0059686d* history,
    void*);
extern "C" void __fastcall FrontEndDequePushBack_0059686d(
    FrontEndComponentDeque_0059686d* history,
    void*,
    CComponent_0059686d* const* component);
extern "C" CManager_0059686d* __cdecl FrontEndManagerGet_0059686d();

__declspec(naked)
void CFrontEndManager_0059686d::GotoSpecificPreviousScreen(
    CComponent_0059686d* component)
{
    __asm
    {
        push ebp
        mov ebp, esp
        push ecx
        push esi
        mov esi, ecx
        mov dword ptr [ebp - 4], esi

        call FrontEndUserProfileGet_0059686d
        mov ecx, eax
        call FrontEndUserProfileLoad_0059686d

        add esi, 20h
        push esi
        lea ecx, [esi + 10h]
        call FrontEndDequeDistance_0059686d
        cmp eax, 1
        jbe finished

        push ebx
        push edi
        mov ecx, esi
        call FrontEndDequeBack_0059686d
        mov edi, dword ptr [eax]

        mov eax, dword ptr [edi]
        push 6
        mov ecx, edi
        call dword ptr [eax + 0c0h]

        call FrontEndManagerGet_0059686d
        mov edx, dword ptr [eax]
        lea ecx, [edi + 4]
        push ecx
        mov ecx, eax
        call dword ptr [edx + 14h]

        mov ecx, esi
        call FrontEndDequePopBack_0059686d

        push esi
        lea ecx, [esi + 10h]
        call FrontEndDequeDistance_0059686d
        mov ebx, dword ptr [ebp + 8]
        jmp testHistorySize

searchHistory:
        mov ecx, esi
        call FrontEndDequeBack_0059686d
        cmp dword ptr [eax], ebx
        je historyReady

        mov ecx, esi
        call FrontEndDequePopBack_0059686d

        push esi
        lea ecx, [esi + 10h]
        call FrontEndDequeDistance_0059686d

testHistorySize:
        test eax, eax
        ja searchHistory

historyReady:
        mov eax, dword ptr [esi + 10h]
        cmp eax, dword ptr [esi]
        jne storeTransition

        lea eax, [ebp + 8]
        push eax
        mov ecx, esi
        call FrontEndDequePushBack_0059686d

storeTransition:
        mov eax, dword ptr [ebp - 4]
        mov dword ptr [eax + 98h], edi
        pop edi
        mov dword ptr [eax + 9ch], ebx
        pop ebx

finished:
        pop esi
        leave
        ret 4
    }
}
