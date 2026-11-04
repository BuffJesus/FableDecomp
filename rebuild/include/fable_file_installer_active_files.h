#pragma once
#include <stdlib.h>
#include "fable_file_installer_containers.h"
#include "engine/CCompressorZlib.h"

// Retail active-file cleanup inlines these nested transfer-list destructors.
struct InstallerTransferList
{
    struct Link { Link *next, *previous; };
    Link* head;
    __forceinline ~InstallerTransferList()
    {
        Link* current=head->next;
        while(current!=head) { Link* victim=current;current=current->next;free(victim); }
        head->next=head;
        head->previous=head;
        if(head)free(head);
    }
};
class InstallerActiveFileList
{
public:
    typedef InstallerCountedFileList::Value Value;
    struct Node
    {
        Node *next, *previous;
        unsigned char unknown08[0x1c];
        Value file;
        unsigned char unknown2c[0x14];
        CCompressorZlib compressor;
        InstallerTransferList input, output;
        unsigned char unknowna0[0x18];
        __forceinline ~Node() {}
    };
    Node* head;
    void Clear(); // retail 009C71A0
};
FABLE_STATIC_ASSERT(sizeof(InstallerActiveFileList)==4);
FABLE_STATIC_ASSERT(sizeof(InstallerActiveFileList::Node)==0xb8);
FABLE_STATIC_ASSERT(offsetof(InstallerActiveFileList::Node,file)==0x24);
FABLE_STATIC_ASSERT(offsetof(InstallerActiveFileList::Node,compressor)==0x40);
FABLE_STATIC_ASSERT(offsetof(InstallerActiveFileList::Node,input)==0x98);
FABLE_STATIC_ASSERT(offsetof(InstallerActiveFileList::Node,output)==0x9c);
