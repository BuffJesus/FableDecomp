#include <stdio.h>
struct N{N* Next;int State;};struct L{N* Head;bool Has(int s)const{for(N*n=Head->Next;n!=Head;n=n->Next)if(n->State==s)return true;return false;}};
int main(){N h,n;h.Next=&n;n.Next=&h;n.State=3;L l={&h};if(!l.Has(3)||l.Has(2))return 1;printf("MARRIAGE_STATE_LOOKUP PASS\n");return 0;}