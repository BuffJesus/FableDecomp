#include <stdio.h>
struct B{virtual ~B(){}};struct D:B{long Value;D(long v):Value(v){}D(const D& d):Value(d.Value){}B* Clone()const{return new D(*this);}};
int main(){D d(23);B*b=d.Clone();D*c=(D*)b;if(c==&d||c->Value!=23)return 1;delete b;printf("CCREATUREACTION_WASPQUEENDEATH_CLONE PASS\n");return 0;}