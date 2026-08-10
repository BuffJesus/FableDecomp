#include <cstdio>
struct Sub114 { int flag; };
struct Host { char pad[0x114]; Sub114 sub; };
static int g_sub=0; static int g_next=0;
static void sub_teardown(Host* h){ (void)h; g_sub++; }
static void next_teardown(Host* h){ (void)h; g_next++; }
static void DestVal_114_B_model(Host* self){ sub_teardown(self); next_teardown(self); }
int main(){
  Host h; h.sub.flag=1;
  DestVal_114_B_model(&h);
  if(g_sub!=1){ printf("FAIL sub\n"); return 1; }
  if(g_next!=1){ printf("FAIL next\n"); return 2; }
  DestVal_114_B_model(&h);
  if(g_sub!=2||g_next!=2){ printf("FAIL2\n"); return 3; }
  printf("OK_008e8cb0\n");
  return 0;
}