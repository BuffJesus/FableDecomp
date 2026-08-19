// Constant-true predicate: `mov al,1; ret`. __fastcall this=ecx, no args.
struct T { bool OnPostDeviceReset(); };
bool T::OnPostDeviceReset() { return true; }