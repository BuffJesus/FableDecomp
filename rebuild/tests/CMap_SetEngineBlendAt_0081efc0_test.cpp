#include <stdio.h>
#include <string.h>
#include "candidates/world_map_theme.h"

struct QuarterGrid { unsigned char p0[4]; unsigned char* data; unsigned char p8[0x14]; long width; long height; };
struct GameGrid { unsigned char p0[0x10]; unsigned char* data; unsigned char p14[0x1F0]; long width; long maxY; };

int main()
{
    unsigned char object[0x1A80]; QuarterGrid quarter; GameGrid game;
    unsigned char qdata[112]; unsigned char gdata[300];
    memset(object,0,sizeof(object)); memset(&quarter,0,sizeof(quarter)); memset(&game,0,sizeof(game));
    memset(qdata,0,sizeof(qdata)); memset(gdata,0,sizeof(gdata));
    *reinterpret_cast<QuarterGrid**>(object+0x1A78)=&quarter;
    *reinterpret_cast<GameGrid**>(object+0x1A7C)=&game;
    quarter.data=qdata; quarter.width=4; quarter.height=4;
    game.data=gdata; game.width=4; game.maxY=4;
    CMap* map=reinterpret_cast<CMap*>(object);
    map->SetEngineBlendAt(1,2,0x72,2,EThemePaintType_Default);
    if(gdata[((4-2)*5+1)*12+7+2]!=0x72)return 1;
    map->SetEngineBlendAt(8,4,0x94,2,EThemePaintType_Replace);
    if(qdata[(((4-1)-1)*4+2)*7+3+2]!=0x94)return 2;
    map->SetEngineBlendAt(1,2,0xFF,2,static_cast<EThemePaintType>(2));
    if(gdata[((4-2)*5+1)*12+7+2]!=0x72)return 3;
    printf("MAP_SET_ENGINE_BLEND_TEST PASS\n"); return 0;
}
