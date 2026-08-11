#pragma optimize("s",on)
struct CQuestCardDef {
    char data[0x74];
    unsigned long GetSizeofClass();
};

unsigned long CQuestCardDef::GetSizeofClass()
{
    return sizeof(CQuestCardDef);
}