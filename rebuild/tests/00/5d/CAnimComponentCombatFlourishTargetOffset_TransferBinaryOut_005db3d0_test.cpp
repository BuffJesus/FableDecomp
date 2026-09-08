#include <stdio.h>
struct V{float x,y;};struct F{bool enabled;V offset;};int main(){F f={true,{2,3}};if(!f.enabled||f.offset.y!=3)return 1;printf("FLOURISH_TARGET_OFFSET_BINARY_OUT PASS\n");return 0;}