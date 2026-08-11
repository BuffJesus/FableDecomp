#include <stdio.h>

struct RouteInner {
    char pad[0x59];
    char b59;
    char b5a;
    char b5b;
};

struct CTCQuestCard {
    char pad0[0x0c];
    RouteInner* inner;
};

int __fastcall CTCQuestCard_IsRouteToAppearOnMinimap(CTCQuestCard* self);

int main()
{
    RouteInner ri;
    CTCQuestCard card;
    card.inner = &ri;

    ri.b59 = 0; ri.b5b = 1;
    if (CTCQuestCard_IsRouteToAppearOnMinimap(&card) != 1) { printf("FAIL1\n"); return 1; }

    ri.b59 = 0; ri.b5b = 0;
    if (CTCQuestCard_IsRouteToAppearOnMinimap(&card) != 1) { printf("FAIL2\n"); return 1; }

    ri.b59 = 1; ri.b5b = 0;
    if (CTCQuestCard_IsRouteToAppearOnMinimap(&card) != 0) { printf("FAIL3\n"); return 1; }

    printf("ROUTE_MINIMAP_OK\n");
    return 0;
}