struct Sub_00baa370;
struct C_00baa370 { char pad[0xac]; Sub_00baa370* sub; Sub_00baa370* M(); };
Sub_00baa370* C_00baa370::M() { return (Sub_00baa370*)&this->sub; }