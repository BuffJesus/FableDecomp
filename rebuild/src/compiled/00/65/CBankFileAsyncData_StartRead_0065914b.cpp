// CBankFileAsyncData::StartRead @ 0065914b
// push esi; lea esi,[ecx+0x64]; lea eax,[esp+8]; push eax;
// mov ecx,esi; call 0x614a10; cmp eax,[esi]; pop esi; setne al; ret 4
#pragma optimize("s",on)

struct SubObj {
    int first;
    // helper @ 0x614a10 : __fastcall member (ecx=this), one stack arg int*, returns int
    int Read(int* pLocal);
};

struct CBankFileAsyncData {
    char pad[0x64];
    SubObj sub;   // at +0x64
    bool StartRead(int scratch);
};

// The incoming stack arg 'scratch' (ret 4) is reused as the scratch local whose
// address is handed to Read; the async read is considered started when the
// returned position differs from the recorded head position (sub.first).
bool CBankFileAsyncData::StartRead(int scratch)
{
    int r = this->sub.Read(&scratch);
    if (r != this->sub.first)
        return true;
    return false;
}