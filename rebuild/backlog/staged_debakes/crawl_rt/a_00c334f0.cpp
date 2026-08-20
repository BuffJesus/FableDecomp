// CThingFilter_IsValid::operator() — forward to virtual slot 1 on the ARGUMENT.
// __fastcall this=ecx (unused), thing=stack (ret 4).
struct Thing { virtual void v0(); virtual bool IsValid(); };
struct T { bool Check(Thing* thing); };
bool T::Check(Thing* thing) { return thing->IsValid(); }
