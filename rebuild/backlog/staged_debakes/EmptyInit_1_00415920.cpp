// Empty virtual hook (CGameComponent::Init and friends): __fastcall this=ecx, no args,
// no cleanup -> a bare `ret`.
struct T { void Init(); };
void T::Init() {}
