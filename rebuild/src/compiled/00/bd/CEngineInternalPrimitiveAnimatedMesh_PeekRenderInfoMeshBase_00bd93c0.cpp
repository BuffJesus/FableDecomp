struct S_00bd93c0;
struct C_00bd93c0 { char pad[0xe0]; S_00bd93c0* sub; S_00bd93c0* M(); };
S_00bd93c0* C_00bd93c0::M() { return (S_00bd93c0*)&this->sub; }