// NUISystem::CFrontEndList::ScrollDown
//
// The dummy virtual declarations preserve the retail slot numbers while the
// named methods carry the recovered signatures used by ScrollDown.
#include <cstdlib>

typedef unsigned char FrontEndByte;
typedef unsigned int FrontEndUInt;

struct FrontEndColour
{
    FrontEndByte red;
    FrontEndByte green;
    FrontEndByte blue;
    FrontEndByte alpha;
};

struct FrontEndVector2
{
    float x;
    float y;
};

#define FE_SLOT_BLOCK_0(X) \
    X(Slot00) X(Slot01) X(Slot02) X(Slot03) \
    X(Slot04) X(Slot05) X(Slot06) X(Slot07) \
    X(Slot08) X(Slot09) X(Slot0A) X(Slot0B) \
    X(Slot0C) X(Slot0D) X(Slot0E) X(Slot0F)
#define FE_SLOT_BLOCK_1(X) \
    X(Slot10) X(Slot11) X(Slot12) X(Slot13) \
    X(Slot14) X(Slot15) X(Slot16) X(Slot17) \
    X(Slot18) X(Slot19) X(Slot1A) X(Slot1B) \
    X(Slot1C) X(Slot1D) X(Slot1E) X(Slot1F)
#define FE_SLOT_BLOCK_2(X) \
    X(Slot20) X(Slot21) X(Slot22) X(Slot23) \
    X(Slot24) X(Slot25) X(Slot26) X(Slot27) \
    X(Slot28) X(Slot29) X(Slot2A) X(Slot2B) \
    X(Slot2C) X(Slot2D) X(Slot2E) X(Slot2F)
#define FE_SLOT_BLOCK_3(X) \
    X(Slot30) X(Slot31) X(Slot32) X(Slot33) \
    X(Slot34) X(Slot35) X(Slot36) X(Slot37) \
    X(Slot38) X(Slot39) X(Slot3A) X(Slot3B) \
    X(Slot3C) X(Slot3D) X(Slot3E) X(Slot3F)
#define FE_SLOT_BLOCK_4(X) \
    X(Slot40) X(Slot41) X(Slot42) X(Slot43) \
    X(Slot44) X(Slot45) X(Slot46) X(Slot47) \
    X(Slot48) X(Slot49) X(Slot4A) X(Slot4B) \
    X(Slot4C) X(Slot4D) X(Slot4E) X(Slot4F)
#define FE_SLOT_BLOCK_5(X) \
    X(Slot50) X(Slot51) X(Slot52) X(Slot53) \
    X(Slot54) X(Slot55) X(Slot56) X(Slot57) \
    X(Slot58) X(Slot59) X(Slot5A) X(Slot5B) \
    X(Slot5C) X(Slot5D) X(Slot5E) X(Slot5F)
#define FE_SLOT_BLOCK_6(X) \
    X(Slot60) X(Slot61) X(Slot62) X(Slot63) \
    X(Slot64) X(Slot65) X(Slot66) X(Slot67) \
    X(Slot68) X(Slot69) X(Slot6A) X(Slot6B) \
    X(Slot6C) X(Slot6D) X(Slot6E) X(Slot6F)
#define FE_SLOT_BLOCK_7(X) \
    X(Slot70) X(Slot71) X(Slot72) X(Slot73) \
    X(Slot74) X(Slot75) X(Slot76) X(Slot77) \
    X(Slot78) X(Slot79) X(Slot7A) X(Slot7B) \
    X(Slot7C) X(Slot7D) X(Slot7E) X(Slot7F)
#define FE_SLOT_BLOCK_8(X) \
    X(Slot80) X(Slot81) X(Slot82) X(Slot83) \
    X(Slot84) X(Slot85) X(Slot86) X(Slot87) \
    X(Slot88) X(Slot89) X(Slot8A) X(Slot8B) \
    X(Slot8C) X(Slot8D) X(Slot8E) X(Slot8F)
#define FE_SLOT_BLOCK_9(X) \
    X(Slot90) X(Slot91) X(Slot92) X(Slot93) \
    X(Slot94) X(Slot95) X(Slot96) X(Slot97) \
    X(Slot98) X(Slot99) X(Slot9A) X(Slot9B) \
    X(Slot9C) X(Slot9D) X(Slot9E) X(Slot9F)
#define FE_DECLARE_VOID_SLOT(Name) virtual void Name();

class FrontEndRefObject
{
public:
    virtual void Slot00();
    virtual void Release();
    long references; // 0x04
};

struct FrontEndDefPointer
{
    FrontEndRefObject* object;
};

class CFrontEndManager
{
public:
    FE_SLOT_BLOCK_0(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_1(FE_DECLARE_VOID_SLOT)
    FE_DECLARE_VOID_SLOT(Slot20)
    FE_DECLARE_VOID_SLOT(Slot21)
    FE_DECLARE_VOID_SLOT(Slot22)
    FE_DECLARE_VOID_SLOT(Slot23)
    FE_DECLARE_VOID_SLOT(Slot24)
    FE_DECLARE_VOID_SLOT(Slot25)
    FE_DECLARE_VOID_SLOT(Slot26)
    FE_DECLARE_VOID_SLOT(Slot27)
    virtual void PlaySound(void* sound, int argument); // 0xa0
    FE_DECLARE_VOID_SLOT(Slot29)
    virtual FrontEndDefPointer* GetMiscDefinition(
        FrontEndDefPointer* output); // 0xa8
};

class CFrontEndComponent
{
public:
    FE_SLOT_BLOCK_0(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_1(FE_DECLARE_VOID_SLOT)
    virtual void SetPosition(
        const FrontEndVector2& position,
        float updateTime,
        bool linear); // 0x80
    FE_DECLARE_VOID_SLOT(Slot21)
    FE_DECLARE_VOID_SLOT(Slot22)
    FE_DECLARE_VOID_SLOT(Slot23)
    FE_DECLARE_VOID_SLOT(Slot24)
    FE_DECLARE_VOID_SLOT(Slot25)
    virtual void SetColour(
        const FrontEndColour& colour,
        float updateTime,
        bool linear); // 0x98
    FE_DECLARE_VOID_SLOT(Slot27)
    FE_DECLARE_VOID_SLOT(Slot28)
    FE_DECLARE_VOID_SLOT(Slot29)
    FE_DECLARE_VOID_SLOT(Slot2A)
    FE_DECLARE_VOID_SLOT(Slot2B)
    FE_DECLARE_VOID_SLOT(Slot2C)
    FE_DECLARE_VOID_SLOT(Slot2D)
    FE_DECLARE_VOID_SLOT(Slot2E)
    FE_DECLARE_VOID_SLOT(Slot2F)
    virtual void SetCurrentState(int state); // 0xc0
    FE_DECLARE_VOID_SLOT(Slot31)
    FE_DECLARE_VOID_SLOT(Slot32)
    FE_DECLARE_VOID_SLOT(Slot33)
    FE_DECLARE_VOID_SLOT(Slot34)
    FE_DECLARE_VOID_SLOT(Slot35)
    FE_DECLARE_VOID_SLOT(Slot36)
    virtual FrontEndVector2& GetPosition(
        FrontEndVector2& output); // 0xdc
    virtual FrontEndColour& GetColour(
        FrontEndColour& output); // 0xe0
};

class CFrontEndList_0054c810
{
public:
    FE_SLOT_BLOCK_0(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_1(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_2(FE_DECLARE_VOID_SLOT)
    FE_DECLARE_VOID_SLOT(Slot30)
    FE_DECLARE_VOID_SLOT(Slot31)
    FE_DECLARE_VOID_SLOT(Slot32)
    FE_DECLARE_VOID_SLOT(Slot33)
    FE_DECLARE_VOID_SLOT(Slot34)
    FE_DECLARE_VOID_SLOT(Slot35)
    virtual float GetUpdateTime(); // 0xd8
    FE_DECLARE_VOID_SLOT(Slot37)
    FE_DECLARE_VOID_SLOT(Slot38)
    FE_DECLARE_VOID_SLOT(Slot39)
    FE_DECLARE_VOID_SLOT(Slot3A)
    FE_DECLARE_VOID_SLOT(Slot3B)
    FE_DECLARE_VOID_SLOT(Slot3C)
    FE_DECLARE_VOID_SLOT(Slot3D)
    FE_DECLARE_VOID_SLOT(Slot3E)
    FE_DECLARE_VOID_SLOT(Slot3F)
    FE_SLOT_BLOCK_4(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_5(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_6(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_7(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_8(FE_DECLARE_VOID_SLOT)
    FE_SLOT_BLOCK_9(FE_DECLARE_VOID_SLOT)
    FE_DECLARE_VOID_SLOT(SlotA0)
    FE_DECLARE_VOID_SLOT(SlotA1)
    FE_DECLARE_VOID_SLOT(SlotA2)
    FE_DECLARE_VOID_SLOT(SlotA3)
    FE_DECLARE_VOID_SLOT(SlotA4)
    FE_DECLARE_VOID_SLOT(SlotA5)
    FE_DECLARE_VOID_SLOT(SlotA6)
    FE_DECLARE_VOID_SLOT(SlotA7)
    FE_DECLARE_VOID_SLOT(SlotA8)
    FE_DECLARE_VOID_SLOT(SlotA9)
    FE_DECLARE_VOID_SLOT(SlotAA)
    virtual bool IsScrolling(); // 0x2ac
    virtual void ScrollDown();

    char pad004[0x158];
    FrontEndUInt selectedIndex;         // 0x15c
    char pad160[0x04];
    CFrontEndComponent** childrenBegin; // 0x164
    CFrontEndComponent** childrenEnd;   // 0x168
    char pad16c[0x8c];
    FrontEndByte alphaOffset;           // 0x1f8
};

extern CFrontEndManager* __cdecl CFrontEndManager_GetInstance();
extern "C" void* __cdecl FableFrontEndScrollDownGetManager();
extern "C" int __cdecl FableFrontEndScrollDownFtol2();

extern "C" const float FableFrontEndScrollDownMinusOne;
extern "C" const float FableFrontEndScrollDownInv255;
extern "C" const float FableFrontEndScrollDownOne;
extern "C" const float FableFrontEndScrollDownZero;
extern "C" const float FableFrontEndScrollDown255;

static __forceinline FrontEndUInt FrontEndChildCount(
    const CFrontEndList_0054c810* self)
{
    return (FrontEndUInt)(self->childrenEnd - self->childrenBegin);
}

static __forceinline void FrontEndReleaseDefinition(
    FrontEndRefObject* object)
{
    if ((object != 0) && (--object->references == 0))
        object->Release();
}

static __forceinline void FrontEndPlayListSound(unsigned int fieldOffset)
{
    CFrontEndManager* definitionManager = CFrontEndManager_GetInstance();
    CFrontEndManager* soundManager = CFrontEndManager_GetInstance();
    FrontEndDefPointer definition;
    FrontEndDefPointer* returnedDefinition =
        definitionManager->GetMiscDefinition(&definition);

    soundManager->PlaySound(
        (char*)returnedDefinition->object + fieldOffset,
        1);
    FrontEndReleaseDefinition(definition.object);
}

__declspec(naked)
void CFrontEndList_0054c810::ScrollDown()
{
    __asm
    {
        sub esp, 20h
        push esi
        mov esi, ecx
        mov eax, dword ptr [esi + 168h]
        push edi
        sub eax, dword ptr [esi + 164h]
        sar eax, 2
        cmp eax, 1
        ja childCountReady
        call FableFrontEndScrollDownGetManager
        mov esi, eax
        call FableFrontEndScrollDownGetManager
        mov edx, dword ptr [esi]
        mov edi, eax
        lea eax, [esp + 8]
        push eax
        mov ecx, esi
        call dword ptr [edx + 0a8h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [edi]
        push 1
        add eax, 1a4h
        push eax
        mov ecx, edi
        call dword ptr [edx + 0a0h]
        mov eax, dword ptr [esp + 8]
        test eax, eax
        je commonReturn
        mov esi, dword ptr [eax + 4]
        mov ecx, eax
        add eax, 4
        dec esi
        mov edx, esi
        test edx, edx
        mov dword ptr [eax], esi
        jne commonReturn
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 4]
        pop edi
        pop esi
        add esp, 20h
        ret

childCountReady:
        mov eax, dword ptr [esi + 168h]
        mov ecx, dword ptr [esi + 15ch]
        push ebx
        sub eax, dword ptr [esi + 164h]
        sar eax, 2
        dec eax
        cmp ecx, eax
        jne scroll
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2ach]
        test al, al
        je scroll
        call FableFrontEndScrollDownGetManager
        mov esi, eax
        call FableFrontEndScrollDownGetManager
        lea ecx, [esp + 0ch]
        mov edi, eax
        mov eax, dword ptr [esi]
        push ecx
        mov ecx, esi
        call dword ptr [eax + 0a8h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [edi]
        push 1
        add eax, 1a4h
        push eax
        mov ecx, edi
        call dword ptr [edx + 0a0h]
        mov eax, dword ptr [esp + 0ch]
        test eax, eax
        je popEbxReturn
        mov esi, dword ptr [eax + 4]
        mov ecx, eax
        add eax, 4
        dec esi
        mov edx, esi
        test edx, edx
        mov dword ptr [eax], esi
        jne popEbxReturn
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 4]
        pop ebx
        pop edi
        pop esi
        add esp, 20h
        ret

scroll:
        call FableFrontEndScrollDownGetManager
        mov edi, eax
        call FableFrontEndScrollDownGetManager
        lea ecx, [esp + 0ch]
        mov ebx, eax
        mov eax, dword ptr [edi]
        push ecx
        mov ecx, edi
        call dword ptr [eax + 0a8h]
        mov eax, dword ptr [eax]
        mov edx, dword ptr [ebx]
        push 1
        add eax, 194h
        push eax
        mov ecx, ebx
        call dword ptr [edx + 0a0h]
        mov eax, dword ptr [esp + 0ch]
        test eax, eax
        je definitionReleased
        mov edi, dword ptr [eax + 4]
        mov ecx, eax
        add eax, 4
        dec edi
        mov edx, edi
        test edx, edx
        mov dword ptr [eax], edi
        jne definitionReleased
        mov edx, dword ptr [ecx]
        call dword ptr [edx + 4]

definitionReleased:
        mov eax, dword ptr [esi + 15ch]
        mov ecx, dword ptr [esi + 164h]
        mov ecx, dword ptr [ecx + eax*4]
        mov edx, dword ptr [ecx]
        push ebp
        push 4
        call dword ptr [edx + 0c0h]
        mov eax, dword ptr [esi + 15ch]
        inc eax
        mov dword ptr [esi + 15ch], eax
        mov ebp, dword ptr [esi + 164h]
        mov ecx, eax
        mov eax, dword ptr [esi + 168h]
        sub eax, ebp
        sar eax, 2
        cmp ecx, eax
        sbb eax, eax
        and eax, ecx
        mov ecx, dword ptr [esi + 164h]
        mov dword ptr [esi + 15ch], eax
        mov eax, dword ptr [ecx + eax*4]
        mov edx, dword ptr [eax]
        push 3
        mov ecx, eax
        call dword ptr [edx + 0c0h]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 0d8h]
        fld dword ptr [FableFrontEndScrollDownMinusOne]
        fucompp
        fnstsw ax
        test ah, 44h
        jnp zeroUpdateTime
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 0d8h]
        fstp dword ptr [esp + 10h]
        jmp updateTimeReady

zeroUpdateTime:
        mov dword ptr [esp + 10h], 0

updateTimeReady:
        mov eax, dword ptr [esi + 164h]
        mov ecx, dword ptr [eax]
        mov edx, dword ptr [ecx]
        lea eax, [esp + 18h]
        push eax
        call dword ptr [edx + 0e0h]
        mov ecx, dword ptr [esi + 164h]
        mov ecx, dword ptr [ecx]
        mov edx, dword ptr [ecx]
        lea eax, [esp + 28h]
        push eax
        call dword ptr [edx + 0dch]
        mov ecx, dword ptr [esi + 168h]
        mov edi, dword ptr [esi + 164h]
        mov ebp, dword ptr [esp + 10h]
        sub ecx, edi
        sar ecx, 2
        xor ebx, ebx
        dec ecx
        mov dword ptr [esp + 20h], 0
        mov dword ptr [esp + 24h], 0c1f00000h
        je wrapLast
        mov edi, edi

rotateRows:
        mov edx, dword ptr [esi + 164h]
        mov ecx, dword ptr [edx + ebx*4 + 4]
        mov edi, dword ptr [edx + ebx*4]
        lea eax, [edx + ebx*4]
        mov eax, dword ptr [edi]
        mov edx, dword ptr [ecx]
        push 0
        push ebp
        lea ebp, [esp + 24h]
        push ebp
        mov dword ptr [esp + 20h], eax
        call dword ptr [edx + 0e0h]
        push eax
        mov eax, dword ptr [esp + 20h]
        mov ecx, edi
        call dword ptr [eax + 98h]
        mov eax, dword ptr [esi]
        mov ecx, esi
        call dword ptr [eax + 2ach]
        test al, al
        mov ebp, dword ptr [esp + 10h]
        je nextRow
        mov ecx, dword ptr [esi + 164h]
        mov ecx, dword ptr [ecx + ebx*4]
        mov edx, dword ptr [ecx]
        push 0
        push ebp
        lea eax, [esp + 28h]
        push eax
        call dword ptr [edx + 80h]

nextRow:
        mov ecx, dword ptr [esi + 168h]
        sub ecx, dword ptr [esi + 164h]
        sar ecx, 2
        inc ebx
        dec ecx
        cmp ebx, ecx
        jb rotateRows

wrapLast:
        mov edi, dword ptr [esi + 164h]
        mov edx, dword ptr [esi + 168h]
        sub edx, edi
        sar edx, 2
        mov eax, edi
        mov ecx, dword ptr [eax + edx*4 - 4]
        mov edx, dword ptr [ecx]
        push 0
        push ebp
        lea eax, [esp + 20h]
        push eax
        call dword ptr [edx + 98h]
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2ach]
        test al, al
        je alphaPolicy
        mov edx, dword ptr [esi + 164h]
        mov eax, dword ptr [esi + 168h]
        sub eax, edx
        sar eax, 2
        mov ecx, edx
        mov ecx, dword ptr [ecx + eax*4 - 4]
        mov edx, dword ptr [ecx]
        push 0
        push ebp
        lea eax, [esp + 28h]
        push eax
        call dword ptr [edx + 80h]

alphaPolicy:
        mov edx, dword ptr [esi]
        mov ecx, esi
        call dword ptr [edx + 2ach]
        test al, al
        je done
        mov ebx, dword ptr [esi + 164h]
        mov al, 0ffh
        mov byte ptr [esp + 12h], al
        mov byte ptr [esp + 11h], al
        mov byte ptr [esp + 10h], al
        mov byte ptr [esp + 13h], al
        mov eax, dword ptr [esi + 168h]
        sub eax, ebx
        sar eax, 2
        xor edi, edi
        test eax, eax
        jbe done
        mov edi, edi

alphaLoop:
        mov eax, dword ptr [esi + 15ch]
        movzx ecx, byte ptr [esi + 1f8h]
        sub eax, edi
        cdq
        xor eax, edx
        sub eax, edx
        mov dword ptr [esp + 14h], eax
        fild dword ptr [esp + 14h]
        mov dword ptr [esp + 14h], ecx
        fild dword ptr [esp + 14h]
        fmul dword ptr [FableFrontEndScrollDownInv255]
        fmulp st(1), st(0)
        fsubr dword ptr [FableFrontEndScrollDownOne]
        fcom dword ptr [FableFrontEndScrollDownZero]
        fnstsw ax
        test ah, 5
        jp positiveAlpha
        fstp st(0)
        fld dword ptr [FableFrontEndScrollDownZero]
        jmp convertAlpha

positiveAlpha:
        fmul dword ptr [FableFrontEndScrollDown255]

convertAlpha:
        call FableFrontEndScrollDownFtol2
        mov edx, dword ptr [esi + 164h]
        mov byte ptr [esp + 13h], al
        mov ecx, dword ptr [edx + edi*4]
        mov eax, dword ptr [ecx]
        push 0
        push ebp
        lea edx, [esp + 18h]
        push edx
        call dword ptr [eax + 98h]
        mov eax, dword ptr [esi + 168h]
        sub eax, dword ptr [esi + 164h]
        inc edi
        sar eax, 2
        cmp edi, eax
        jb alphaLoop

done:
        pop ebp

popEbxReturn:
        pop ebx

commonReturn:
        pop edi
        pop esi
        add esp, 20h
        ret
    }
}
