#pragma once
#include <stddef.h>
#include "fable_string.h"
#include "fable_file_installer.h"
#include "fable_counted_pointer.h"

// Shipping CompletedFiles container at installer +0x44. The sentinel is
// allocated as 20 bytes and its value lies at +0x10 in ordinary nodes.
class InstallerCompletedFileSet
{
public:
    struct Node
    {
        bool color;
        Node *parent, *left, *right;
        CCharString value;
    };
    void Erase(Node* node); // retail 004ABA70, does not update the owner fields
    Node* head;
    fable_u32 size;
    fable_u32 unknown08;
};
FABLE_STATIC_ASSERT(sizeof(InstallerCompletedFileSet::Node) == 20);
FABLE_STATIC_ASSERT(offsetof(InstallerCompletedFileSet::Node, value) == 16);
FABLE_STATIC_ASSERT(sizeof(InstallerCompletedFileSet) == 12);

// Shipping FilesToCopy/FilesToFinish at +0x38/+0x3C each own one sentinel.
class InstallerCountedFileList
{
public:
    typedef CCountedPointer<const CFileInstaller::CFileInfo> Value;
    struct Node { Node *next, *previous; Value value; };
    Node* head;
    void Clear(); // retail 009C6CC0
};
FABLE_STATIC_ASSERT(sizeof(InstallerCountedFileList) == 4);
FABLE_STATIC_ASSERT(sizeof(InstallerCountedFileList::Node) == 16);
FABLE_STATIC_ASSERT(offsetof(InstallerCountedFileList::Node, value) == 8);
