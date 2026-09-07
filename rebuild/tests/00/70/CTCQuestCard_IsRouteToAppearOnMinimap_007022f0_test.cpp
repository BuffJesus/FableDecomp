#include "engine/CTCQuestCard.h"
#include <stdio.h>

struct CDefPointeeBase {
    char pad[0x59];
    char b59;
    char b5a;
    char b5b;
};


int __fastcall CTCQuestCard_IsRouteToAppearOnMinimap(CTCQuestCard* self);

int main()
{
    CDefPointeeBase ri;
    CTCQuestCard card;
    card.PDef_Object = &ri;

    ri.b59 = 0; ri.b5b = 1;
    if (CTCQuestCard_IsRouteToAppearOnMinimap(&card) != 1) { printf("FAIL1\n"); return 1; }

    ri.b59 = 0; ri.b5b = 0;
    if (CTCQuestCard_IsRouteToAppearOnMinimap(&card) != 1) { printf("FAIL2\n"); return 1; }

    ri.b59 = 1; ri.b5b = 0;
    if (CTCQuestCard_IsRouteToAppearOnMinimap(&card) != 0) { printf("FAIL3\n"); return 1; }

    printf("ROUTE_MINIMAP_OK\n");
    return 0;
}