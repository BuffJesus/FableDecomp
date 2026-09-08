#include <stdio.h>
struct Manager{long Released;void ReleaseRequest(long p){Released=p;}};
struct Task{void* Tree;Manager* FileBlockManager;long FileBlockPos;unsigned char LOD;bool Patch,BlockLoading;
 void ReleaseFileBlockLoad();};
void Task::ReleaseFileBlockLoad(){if(BlockLoading){FileBlockManager->ReleaseRequest(FileBlockPos);BlockLoading=false;}}
int main(){Manager m;m.Released=-1;Task t;t.FileBlockManager=&m;t.FileBlockPos=77;t.BlockLoading=true;
 t.ReleaseFileBlockLoad();if(t.BlockLoading||m.Released!=77)return 1;m.Released=9;t.ReleaseFileBlockLoad();
 if(m.Released!=9)return 2;printf("BACKGROUND_LOAD_RELEASE PASS\n");return 0;}