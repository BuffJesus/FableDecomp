#include "fable_file_installer_active_files.h"
void InstallerActiveFileList::Clear()
{
    Node* current=head->next;
    while(current!=head){
        Node* victim=current;current=current->next;
        victim->output.~InstallerTransferList();
        victim->input.~InstallerTransferList();
        victim->compressor.CCompressorZlib::~CCompressorZlib();
        victim->file.~Value();
        free(victim);
    }
    head->next=head;
    head->previous=head;
}
