#include "fable_game_text_bank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef _STL::list<CCountedPointer<CBankFileAsyncData> > List;
static unsigned callbacks,deletes,errors;
static CCPPointerInfo* owned;
static void __fastcall release(void* p) {if(p!=reinterpret_cast<void*>(0x1234))++errors;++callbacks;}
void operator delete(void* p) {if(p!=owned)++errors;++deletes;free(p);}
static List::_Node* makeNode(CCPPointerInfo* info) {
    List::_Node* n=static_cast<List::_Node*>(malloc(sizeof(List::_Node)));memset(n,0,sizeof(*n));
    n->_M_data.Data=reinterpret_cast<CBankFileAsyncData*>(0x1234);n->_M_data.Info=info;return n;
}
int main() {
    List::_Node* h=makeNode(0);h->_M_next=h->_M_prev=h;
    unsigned char raw[sizeof(List)];List* list=reinterpret_cast<List*>(raw);list->_M_node=h;
    list->clear();if(callbacks || deletes || h->_M_next!=h || h->_M_prev!=h)return 1;
    owned=new CCPPointerInfo(release,reinterpret_cast<void*>(0x1234));
    CCPPointerInfo* shared=new CCPPointerInfo(release,reinterpret_cast<void*>(0x1234));shared->RefCount=2;
    List::_Node* a=makeNode(owned);List::_Node* b=makeNode(shared);List::_Node* c=makeNode(0);
    h->_M_next=a;h->_M_prev=c;a->_M_next=b;a->_M_prev=h;b->_M_next=c;b->_M_prev=a;c->_M_next=h;c->_M_prev=b;
    list->clear();
    if(errors || callbacks!=1 || deletes!=1 || shared->RefCount!=1 || list->_M_node!=h || h->_M_next!=h || h->_M_prev!=h)return 2;
    list->clear();if(errors || callbacks!=1 || deletes!=1)return 3;
    free(shared);free(h);puts("ASYNC_LIST_CLEAR_009d7db0_PASS");return 0;
}
