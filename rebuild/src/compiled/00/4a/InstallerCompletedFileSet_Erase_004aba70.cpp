// Retail completed-file set erasure: exact59 bytes and recorded/real string gates.
#include <stdlib.h>
#include "fable_file_installer_containers.h"
void InstallerCompletedFileSet::Erase(Node* node)
{
    while(node) {
        Erase(node->right);
        Node* left=node->left;
        node->value.~CCharString();
        free(node);
        node=left;
    }
}
