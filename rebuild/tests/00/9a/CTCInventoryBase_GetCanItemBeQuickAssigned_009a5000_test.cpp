#include <stdio.h>
struct CTCInventoryBase {
  char pad[0xe1];
  unsigned char canQuickAssign;
};
char __fastcall GetCanItemBeQuickAssigned(CTCInventoryBase* self) {
  return (char)self->canQuickAssign;
}
int main(){
  CTCInventoryBase a; a.canQuickAssign = 1;
  CTCInventoryBase b; b.canQuickAssign = 0;
  if (GetCanItemBeQuickAssigned(&a) == 0) { printf("FAIL\n"); return 1; }
  if (GetCanItemBeQuickAssigned(&b) != 0) { printf("FAIL\n"); return 1; }
  printf("OK_009a5000\n");
  return 0;
}