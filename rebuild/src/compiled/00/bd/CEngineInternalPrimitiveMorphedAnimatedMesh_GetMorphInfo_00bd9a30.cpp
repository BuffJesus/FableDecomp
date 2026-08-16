struct S_00bd9a30;
struct C_00bd9a30 { char pad[0x128]; S_00bd9a30* sub; S_00bd9a30* M(); };
S_00bd9a30* C_00bd9a30::M() { return (S_00bd9a30*)&this->sub; }