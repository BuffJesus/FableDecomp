// CActionDoCreatureAction::GetActionName @ 008fd7d9
// Retail: push esi; push -1; push offset "literal"; mov esi,ecx;
//         call ??0CCharString; mov eax,esi; pop esi; ret
// A no-arg fastcall value factory: hidden CCharString return slot arrives in
// ECX, the body constructs it from a string literal with length -1.

struct CCharStringData
{
    char* text;
    unsigned int unknown04;
    unsigned int unknown08;
    unsigned char flags0C;
    int owners;
};

class CCharString
{
public:
    CCharString(const char* text, long length);
    CCharStringData* storage_;
};

CCharString __fastcall GetActionName()
{
    return CCharString("DoCreatureAction", -1);
}