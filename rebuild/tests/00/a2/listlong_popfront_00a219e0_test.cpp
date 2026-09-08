#include <stdio.h>
struct Node { Node* Next; Node* Previous; long Value; };
static Node* destroyed=0; void __cdecl DestroyListLongNode(Node* node){destroyed=node;}
struct List { Node* Sentinel; void pop_front(); };
void List::pop_front(){Node* node=Sentinel->Next;Node* next=node->Next;Node* previous=node->Previous;
 previous->Next=next;next->Previous=previous;DestroyListLongNode(node);}
int main(){Node sentinel,first,second;sentinel.Next=&first;sentinel.Previous=&second;
 first.Next=&second;first.Previous=&sentinel;second.Next=&sentinel;second.Previous=&first;
 List values;values.Sentinel=&sentinel;values.pop_front();
 if(sentinel.Next!=&second || second.Previous!=&sentinel || destroyed!=&first)return 1;
 printf("LIST_LONG_POP_FRONT PASS\n");return 0;}