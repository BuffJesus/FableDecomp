#include <stdio.h>
struct Node{Node* next;};static int Count(Node* p,Node* end){int n=0;while(p!=end){p=p->next;++n;}return n;}
int main(){Node end={0},b={&end},a={&b};if(Count(&a,&end)!=2||Count(&end,&end)!=0)return 1;puts("ALLOCATOR_POOL_PAIR_PASS");return 0;}