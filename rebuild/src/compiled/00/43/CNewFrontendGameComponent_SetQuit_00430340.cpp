// Byte-flag setter at this+8: `mov byte ptr [ecx+8],1; ret`. __fastcall this=ecx.
struct T { char pad[8]; char quit; void SetQuit(); };
void T::SetQuit() { this->quit = 1; }