#include <cstdio>

struct HomeObj {
    int id;
    int GetHomeId();
};
int HomeObj::GetHomeId() { return id; }

struct CThingAICreature {
    char pad[0x168];
    HomeObj home;
};

struct CThingFilter_HasBuildingAsHome {
    int field0;
    bool operator()(CThingAICreature* creature);
};

bool CThingFilter_HasBuildingAsHome::operator()(CThingAICreature* creature)
{
    return creature->home.GetHomeId() == this->field0;
}

int main()
{
    CThingAICreature c;
    c.home.id = 42;

    CThingFilter_HasBuildingAsHome fMatch;
    fMatch.field0 = 42;
    CThingFilter_HasBuildingAsHome fMiss;
    fMiss.field0 = 7;

    bool a = fMatch(&c);   // true
    bool b = fMiss(&c);    // false

    if (a && !b) {
        printf("HASBUILDINGHOME_OK\n");
        return 0;
    }
    printf("HASBUILDINGHOME_FAIL a=%d b=%d\n", (int)a, (int)b);
    return 1;
}