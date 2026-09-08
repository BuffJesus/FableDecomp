#include <stdio.h>
struct Q{int count;void Remove(unsigned){--count;}void Clear(){while(count)Remove(0);}};
int main(){Q q={4};q.Clear();if(q.count)return 1;printf("DRAW_GUILD_SEAL_CLEAR_MESSAGES PASS\n");return 0;}