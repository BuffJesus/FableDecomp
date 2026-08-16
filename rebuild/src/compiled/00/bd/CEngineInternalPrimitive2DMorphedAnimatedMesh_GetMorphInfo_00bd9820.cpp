struct S_00bd9820;
struct C_00bd9820 { char pad[0x150]; S_00bd9820* sub; S_00bd9820* M(); };
S_00bd9820* C_00bd9820::M() { return (S_00bd9820*)&this->sub; }