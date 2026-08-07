struct Filter; struct CThing { virtual void v0(); virtual void v1(); virtual bool V2(Filter*); };
struct Filter { bool op(CThing* t); };
bool Filter::op(CThing* t){ return t->V2(this); }