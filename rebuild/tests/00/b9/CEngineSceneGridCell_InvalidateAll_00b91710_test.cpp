#include <stdio.h>
struct Cell { Cell* parent; char pad[0x40]; unsigned char flags; void InvalidateAll(); };
void Cell::InvalidateAll() { Cell* cell=this; do {
 unsigned char flags=cell->flags; if((flags&7)==0) return;
 cell->flags=flags&0xF8; cell=cell->parent; } while(cell!=0); }
int main() { Cell root,child; root.parent=0; root.flags=0x0f; child.parent=&root; child.flags=0x07;
 child.InvalidateAll(); if(child.flags!=0 || root.flags!=8) return 1;
 root.flags=8; child.flags=7; child.InvalidateAll(); if(root.flags!=8) return 2;
 printf("SCENE_GRID_INVALIDATE PASS\n"); return 0; }