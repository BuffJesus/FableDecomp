// CPlayerGui::ForceOpen  @ 0x00434ad0  (__fastcall, this=ecx, no stack args)
struct CPlayerGui {
    char pad_c8[0xc8];
    unsigned char b_c8;   // +0xc8
    unsigned char b_c9;   // +0xc9
    char pad_ca[0xd8-0xc9-1];
    unsigned char b_d8;   // +0xd8
    char pad_d9[0xf0-0xd8-1];
    unsigned int  d_f0;   // +0xf0
    unsigned int  d_f4;   // +0xf4
};

void __fastcall ForceOpen(CPlayerGui *this_)
{
    this_->b_c9 = 0;
    this_->b_c8 = 1;
    this_->b_d8 = 0;
    this_->d_f0 = 0;
    this_->d_f4 = 0;
}