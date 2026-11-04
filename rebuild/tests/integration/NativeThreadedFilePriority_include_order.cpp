#include "fable_threaded_file.h"
#include "fable_gfuninitialise.h"
#include "fable_game_text_bank.h"
#include "fable_bank_async_entry.h"
FABLE_STATIC_ASSERT(sizeof(EThreadedFilePriority)==4);
FABLE_STATIC_ASSERT(THREADED_FILE_PRIORITY_NORMAL==3);
