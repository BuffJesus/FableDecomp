#include <cstdio>

struct Hero
{
    bool blocked;
    float health;
    float seenAmount;
    bool seenRawMode;
    int healthCalls;
    int actionConstructs;
    int actionSets;
    int actionDestroys;
};
struct Gui { float amount; bool isDamage; int calls; };

static void Change(
    Hero* resolved,
    float amount,
    bool rawHealthMode,
    bool applyHealthChange,
    float deathThreshold,
    Gui& gui)
{
    if (resolved == 0 || resolved->blocked)
        return;
    resolved->seenAmount = amount;
    resolved->seenRawMode = rawHealthMode;
    ++resolved->healthCalls;
    if (rawHealthMode && resolved->health < deathThreshold)
    {
        ++resolved->actionConstructs;
        ++resolved->actionSets;
        ++resolved->actionDestroys;
    }
    if (applyHealthChange && amount < 0.0f)
    {
        gui.amount = -amount;
        gui.isDamage = true;
        ++gui.calls;
    }
}

int main()
{
    Gui gui = {0.0f, false, 0};
    Hero hero = {false, 0.0f, 0.0f, false, 0, 0, 0, 0};

    Change(&hero, -12.5f, false, true, 0.0001f, gui);
    if (hero.healthCalls != 1 || hero.seenAmount != -12.5f || hero.seenRawMode)
        return 1;
    if (hero.actionConstructs != 0 || gui.calls != 1 || gui.amount != 12.5f || !gui.isDamage)
        return 2;

    Change(&hero, 3.0f, true, false, 0.0001f, gui);
    if (hero.healthCalls != 2 || !hero.seenRawMode)
        return 3;
    if (hero.actionConstructs != 1 || hero.actionSets != 1 || hero.actionDestroys != 1)
        return 4;
    if (gui.calls != 1)
        return 5;

    hero.health = 0.0001f;
    Change(&hero, -1.0f, true, false, 0.0001f, gui);
    if (hero.actionConstructs != 1)
        return 6;

    hero.blocked = true;
    Change(&hero, -2.0f, true, true, 0.0001f, gui);
    Change(0, -2.0f, true, true, 0.0001f, gui);
    if (hero.healthCalls != 3 || gui.calls != 1)
        return 7;

    std::puts("CHANGE_HERO_HEALTH_BY_PASS");
    return 0;
}