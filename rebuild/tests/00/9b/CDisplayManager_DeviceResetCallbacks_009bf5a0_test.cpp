#include "fable_display_viewport.h"
#include <stdio.h>
#include <string.h>
typedef _STL::_List_node<CDeviceResetCallback*> Node;
static unsigned long storage[0x2c8/4], expectedStorage[0x2c8/4], markers[8];
static CDeviceResetCallback* Pointer(unsigned id){return id?reinterpret_cast<CDeviceResetCallback*>(markers+id):0;}
static bool Check(Node& head,const unsigned* values,unsigned count) {
    if(memcmp(storage,expectedStorage,sizeof(storage)))return false;
    _STL::_List_node_base* node=head._M_next;_STL::_List_node_base* previous=&head;
    for(unsigned i=0;i<count;++i) {
        if(node==&head || node->_M_prev!=previous || static_cast<Node*>(node)->_M_data!=Pointer(values[i]))return false;
        previous=node;node=node->_M_next;
    }
    return node==&head && head._M_prev==previous;
}
static void RemoveModel(unsigned* values,unsigned& count,unsigned target) {
    for(unsigned i=0;i<count;++i)if(values[i]==target) {
        for(unsigned j=i+1;j<count;++j)values[j-1]=values[j];
        --count;break;
    }
}
int main() {
    unsigned scenarios=0;
    for(unsigned length=0;length<=8;++length)for(unsigned pattern=0;pattern<4;++pattern)for(unsigned target=0;target<6;++target) {
        memset(storage,0xa5,sizeof(storage));Node head;head._M_next=head._M_prev=&head;head._M_data=Pointer(7);
        CDisplayManager* manager=reinterpret_cast<CDisplayManager*>(storage);manager->DeviceResetCallbacks._M_node=&head;
        memcpy(expectedStorage,storage,sizeof(storage));unsigned values[10],count=length;
        for(unsigned i=0;i<length;++i) {
            values[i]=(pattern+i)%4;Node* node=static_cast<Node*>(malloc(sizeof(Node)));if(!node)return 3;
            node->_M_data=Pointer(values[i]);node->_M_next=&head;node->_M_prev=head._M_prev;head._M_prev->_M_next=node;head._M_prev=node;
        }
        manager->RemoveDeviceResetCallback(Pointer(target));RemoveModel(values,count,target);
        if(!Check(head,values,count))return 1;
        manager->AddDeviceResetCallback(Pointer(target));values[count++]=target;
        if(!Check(head,values,count))return 2;
        manager->RemoveDeviceResetCallback(Pointer(target));RemoveModel(values,count,target);
        if(!Check(head,values,count))return 4;
        while(head._M_next!=&head){Node* node=static_cast<Node*>(head._M_next);node->_M_unhook();free(node);}
        ++scenarios;
    }
    printf("DEVICE_RESET_CALLBACKS_BEHAVIOR_PASS scenarios=%u\n",scenarios);return 0;
}
