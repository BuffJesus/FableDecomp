#include "fable_retail_banks.h"

void CBankFileManager::CRetailBankList::clear()
{
    CNode* head = node_;
    CNode* current = head->next_;
    while (current != head)
    {
        CNode* next = current->next_;
        current->value_.~CCountedPointer();
        free(current);
        current = next;
    }
    head->next_ = head;
    head->prev_ = head;
}
