#include <stdio.h>
struct B{float TLX,TLY,BRX,BRY;};B Convert(const B& b){return b;}
int main(){B a={1,2,3,4},b=Convert(a);if(b.TLX!=1||b.TLY!=2||b.BRX!=3||b.BRY!=4)return 1;printf("GUI_TO_SCREEN PASS\n");return 0;}