#include <stdio.h>
struct Box { float minX,minY,maxX,maxY; };
struct Edge { float x1,y1,x2,y2; };
static int calls;
static int hit;
static bool Probe(const Edge* e,int index,const Box* b){calls++;Edge expected[4]={{b->minX,b->minY,b->maxX,b->minY},{b->maxX,b->minY,b->maxX,b->maxY},{b->minX,b->maxY,b->maxX,b->maxY},{b->minX,b->minY,b->minX,b->maxY}};if(e->x1!=expected[index].x1||e->y1!=expected[index].y1||e->x2!=expected[index].x2||e->y2!=expected[index].y2)return false;return index==hit;}
static bool Run(const Box* b){Edge edges[4]={{b->minX,b->minY,b->maxX,b->minY},{b->maxX,b->minY,b->maxX,b->maxY},{b->minX,b->maxY,b->maxX,b->maxY},{b->minX,b->minY,b->minX,b->maxY}};for(int i=0;i<4;i++)if(Probe(&edges[i],i,b))return true;return false;}
int main(){Box b={-2,3,7,11};for(hit=0;hit<4;hit++){calls=0;if(!Run(&b)||calls!=hit+1)return 1;}hit=-1;calls=0;if(Run(&b)||calls!=4)return 2;puts("C2DLINE_BOX_INTERSECTS_PASS");return 0;}