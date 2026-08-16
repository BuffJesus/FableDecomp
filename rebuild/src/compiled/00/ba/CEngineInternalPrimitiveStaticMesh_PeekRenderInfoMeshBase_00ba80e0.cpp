struct Sub_00ba80e0;
struct C_00ba80e0 { char pad[0xa0]; Sub_00ba80e0* sub; Sub_00ba80e0* M(); };
Sub_00ba80e0* C_00ba80e0::M() { return (Sub_00ba80e0*)&this->sub; }