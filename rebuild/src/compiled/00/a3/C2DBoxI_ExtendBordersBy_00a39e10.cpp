struct C2DBoxI { int a,b,c,d; void ExtendBordersBy(long); };
void C2DBoxI::ExtendBordersBy(long){ --a; ++c; --b; ++d; }