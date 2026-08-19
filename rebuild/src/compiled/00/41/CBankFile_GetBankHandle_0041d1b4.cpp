#pragma optimize("s",on)
// Register the sub-object at this+0x18 with the bank passed in, then return the bank.
// __fastcall this=ecx, bank=stack (ret 4).
#pragma pack(push,1)
struct Sub { int dummy; };
struct Bank { void Register(Sub* sub); };
struct T { char pad[0x18]; Sub sub; Bank* GetBankHandle(Bank* bank); };
#pragma pack(pop)
Bank* T::GetBankHandle(Bank* bank) {
    bank->Register(&this->sub);
    return bank;
}