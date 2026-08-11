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

int __fastcall CTCQuestCard_IsRouteToAppearOnMinimap(CTCQuestCard* self)
{
    RouteInner* p = self->inner;
    if (!p->b5b)
    {
        if (p->b59)
            return 0;
    }
    return 1;
}