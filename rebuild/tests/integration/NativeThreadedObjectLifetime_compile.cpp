#include "fable_threaded_file_object.h"
typedef CThreadedFileManager::CFileObject Request;
FABLE_STATIC_ASSERT(sizeof(Request)==0x40);
#if defined(FABLE_NEGATIVE_REQUEST_COPY)
void RejectRequestCopy(const Request& source) { Request copy(source); }
#elif defined(FABLE_NEGATIVE_REQUEST_ASSIGN)
void RejectRequestAssignment(Request& target,const Request& source) { target=source; }
#else
void CheckRequestLifetime() { Request request; }
void CheckOwnedDelete(Request* request) { delete request; }
#endif
