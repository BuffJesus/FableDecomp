#include "fable_game_text_bank.h"
inline void CResourceList::FrameEnd(){++CurrentFrame;UnloadedThisFrame=0;}
void CResourceBank::FrameEnd(){ResourceList.FrameEnd();LoadingQueue.FrameEnd();}
