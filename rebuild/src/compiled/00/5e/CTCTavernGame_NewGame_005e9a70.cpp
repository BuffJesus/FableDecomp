struct CTCTavernGame {
    char pad0[0x34];
    int  field_34;   // 0x34
    char pad1[0x08];
    int  field_40;   // 0x40
    char pad2[0x5c];
    char field_a0;   // 0xa0
    char field_a1;   // 0xa1
    char field_a2;   // 0xa2
};

void __fastcall NewGame(CTCTavernGame *this_)
{
    this_->field_40 -= this_->field_34;
    this_->field_a0 = 0;
    this_->field_a2 = 0;
}