#include <stddef.h>

class CCharStringData
{
public:
    char* m_psz;
    long m_nLength;
};

class CCharString
{
public:
    CCharStringData* m_pString;
};

class CGameScriptInterface
{
public:
    bool IsQuestStartScreenActive(CCharString& value) const;
};

struct CGlobalInterfaceRootOverlay
{
    unsigned char m_Pad00[0x1C];
    void* m_Field1C;
};

struct CInterfaceMapOverlay
{
    void* m_pBegin;
    long* m_pEnd;

    long* LowerBound(const long* key);
};

struct CInterfaceRegistryOverlay
{
    unsigned char m_Pad00[0x3C];
    unsigned long m_Flags3C;
    unsigned char m_Pad40[0x04];
    CInterfaceMapOverlay m_Map44;
};

struct CTCBaseOverlay
{
    unsigned char m_Pad00[0x0C];
    unsigned char m_Flag0C;
    unsigned char m_Pad0D[0x0F];
    unsigned char m_Flag1C;
    unsigned char m_Pad1D[0x03];
    unsigned long m_Value20;
    unsigned char m_Pad24[0x2C];
    CCharStringData* m_String50;
};

extern "C" void* __fastcall Helper00449970(void* value);
extern "C" CInterfaceRegistryOverlay* __fastcall Helper00487DC0(void* value);
extern "C" long* LowerBound0040F020(void* map, const long* key);
extern "C" long __fastcall Compare00411570(const char* lhs, const char* rhs);

bool __declspec(naked) CGameScriptInterface::IsQuestStartScreenActive(
    CCharString& value) const
{
    __asm
    {
        push ecx
        __emit 0A1h
        __emit 0A0h
        __emit 086h
        __emit 03Bh
        __emit 001h
        mov ecx, dword ptr [eax+1Ch]
        push esi
        call Helper00449970
        mov ecx, eax
        call Helper00487DC0
        test dword ptr [eax+3Ch], 10000000h
        je IsQuestStartScreenActive_false
        lea ecx, [esp+4]
        lea esi, [eax+44h]
        push ecx
        mov ecx, esi
        mov dword ptr [esp+8], 0FCh
        call LowerBound0040F020
        mov esi, dword ptr [esi+4]
        cmp eax, esi
        je IsQuestStartScreenActive_end
        cmp dword ptr [eax], 0FCh
        jle IsQuestStartScreenActive_entry
IsQuestStartScreenActive_end:
        mov eax, esi
IsQuestStartScreenActive_entry:
        mov eax, dword ptr [eax+4]
        mov cl, byte ptr [eax+0Ch]
        test cl, cl
        je IsQuestStartScreenActive_false
        mov cl, byte ptr [eax+1Ch]
        test cl, cl
        jne IsQuestStartScreenActive_false
        mov ecx, dword ptr [eax+20h]
        test ecx, ecx
        jne IsQuestStartScreenActive_false
        mov edx, dword ptr [esp+0Ch]
        mov eax, dword ptr [eax+50h]
        mov ecx, dword ptr [edx]
        cmp eax, ecx
        je IsQuestStartScreenActive_true
        test eax, eax
        je IsQuestStartScreenActive_false
        test ecx, ecx
        je IsQuestStartScreenActive_false
        mov edx, dword ptr [eax+4]
        cmp edx, dword ptr [ecx+4]
        jne IsQuestStartScreenActive_false
        mov edx, dword ptr [ecx]
        mov ecx, dword ptr [eax]
        call Compare00411570
        neg eax
        sbb al, al
        inc al
        je IsQuestStartScreenActive_false
IsQuestStartScreenActive_true:
        mov al, 1
        pop esi
        pop ecx
        ret 4
IsQuestStartScreenActive_false:
        xor al, al
        pop esi
        pop ecx
        ret 4
    }
}
