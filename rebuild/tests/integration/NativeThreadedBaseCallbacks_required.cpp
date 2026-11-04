#include "fable_game_text_bank.h"
bool RequireBaseBegin(CThreadedFileCallback* callback, void** data, EThreadedFilePriority priority)
{
    return callback->CThreadedFileCallback::OnFileReadBegin(data, priority);
}
void RequireBaseCompletion(CThreadedFileCallback* callback, bool success)
{
    callback->CThreadedFileCallback::OnFileReadCompletion(success);
}
int main() { return 0; }
