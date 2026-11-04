#include "fable_persist_context.h"
#include "fable_string.h"

void CPersistContext::TransferObjectSaveText(const char* name, CPersistContext_TransferableComponentGenericType& object)
{
    *PStringToSaveTo += name;
    *PStringToSaveTo += " ";
    object.TransferOut(*this, *PStringToSaveTo);
    *PStringToSaveTo += ";\r\n";
}
