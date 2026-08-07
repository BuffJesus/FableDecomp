struct C { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
          virtual void v4(); virtual void v5(); virtual void SetX(bool); };
struct B { char pad[0x2c]; C* f2c; };
struct A { char pad[0x28]; B* f28; };
struct CGameScriptInterface { char pad[8]; A* f8; void SetEnvironmentalEffectsAlwaysUpdate(bool b); };
void CGameScriptInterface::SetEnvironmentalEffectsAlwaysUpdate(bool b){ f8->f28->f2c->SetX(b); }