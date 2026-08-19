// Constant-false predicate: `xor al,al; ret`. __fastcall this=ecx, no args.
struct T { bool IsEditorActive(); };
bool T::IsEditorActive() { return false; }
