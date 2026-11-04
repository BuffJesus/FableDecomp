#include <stdlib.h>
#include "fable_file_installer_containers.h"
void InstallerCountedFileList::Clear()
{
    Node* current=head->next;
    while(current!=head) {
        Node* node=current;
        current=current->next;
        node->value.~Value();
        free(node);
    }
    head->next=head;
    head->previous=head;
}
