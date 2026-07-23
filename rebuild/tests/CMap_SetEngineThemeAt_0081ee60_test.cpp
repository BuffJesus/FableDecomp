#include <stdio.h>
#include <string.h>
#include "candidates/world_map_theme.h"

static unsigned long g_addedTheme;
unsigned char CMap::AddThemeDefIndexToPalette(unsigned long themeId)
{
    g_addedTheme = themeId;
    return 6;
}

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
    map->SetEngineThemeAt(1,2,0x1234,3,EThemePaintType_Default);
    if(g_addedTheme!=0x1234 || gdata[((4-2)*5+1)*12+4+3]!=6)return 1;
    map->SetEngineThemeAt(2,2,0,1,EThemePaintType_Default);
    if(gdata[((4-2)*5+2)*12+4+1]!=0)return 2;
    map->SetEngineThemeAt(8,4,0xA5,3,EThemePaintType_Replace);
    if(qdata[(((4-1)-1)*4+2)*7+3]!=0xA5)return 3;
    printf("MAP_SET_ENGINE_THEME_TEST PASS\n"); return 0;
}
