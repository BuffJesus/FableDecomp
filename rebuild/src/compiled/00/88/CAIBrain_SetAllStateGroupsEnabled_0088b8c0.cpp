struct CAIStateGroupBase {
    unsigned char Reserved[0x12];
    bool Enabled;
};
struct CAIStateGroupEntry {
    long Key;
    CAIStateGroupBase* StateGroup;
};
struct CAIBrain {
    unsigned char Reserved[0x28];
    CAIStateGroupEntry* StateGroupListBegin;
    CAIStateGroupEntry* StateGroupListEnd;
    void SetAllStateGroupsEnabled(bool enabled);
};
void CAIBrain::SetAllStateGroupsEnabled(bool enabled)
{
    for (CAIStateGroupEntry* entry = StateGroupListBegin; entry != StateGroupListEnd; ++entry)
        entry->StateGroup->Enabled = enabled;
}