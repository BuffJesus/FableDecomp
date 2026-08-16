struct Sub_00bc3af0;
struct C_00bc3af0 { char pad[0x7c]; Sub_00bc3af0* sub; Sub_00bc3af0* M(); };
Sub_00bc3af0* C_00bc3af0::M() { return (Sub_00bc3af0*)&this->sub; }