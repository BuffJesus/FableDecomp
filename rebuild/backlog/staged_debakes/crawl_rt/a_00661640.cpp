// CThingFilter_IsNot::operator() — `return this->target != candidate;`
// __fastcall this=ecx, candidate=stack (ret 4).
struct T { void* target; bool NotEqual(void* candidate); };
bool T::NotEqual(void* candidate) { return candidate != this->target; }
