#include <stdio.h>
struct CActionPointUserNode { CActionPointUserNode* Next; };
struct CTCUserActionPointBase { char pad[0x108]; CActionPointUserNode* sentinel;
 bool IsActionPointFree() const; };
bool CTCUserActionPointBase::IsActionPointFree() const { return sentinel->Next != sentinel; }
int main() { CActionPointUserNode head,node; CTCUserActionPointBase point; point.sentinel=&head;
 head.Next=&head; if(point.IsActionPointFree()) return 1;
 head.Next=&node; if(!point.IsActionPointFree()) return 2;
 printf("ACTION_POINT_FREE PASS\n"); return 0; }