// `return this->p == other->p;` via the sub/neg/sbb/inc idiom. __fastcall this=ecx (ret 4).
struct CP { void* p; bool IsEquivalent(const CP* other) const; };
bool CP::IsEquivalent(const CP* other) const { return this->p == other->p; }
