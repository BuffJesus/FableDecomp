#include <cstdio>

struct Activity { bool blocked; };
struct Player { int index; Activity* activity; };

static bool Active(Player** begin, Player** end, int neutral)
{
    for (int slot = 0; slot < 4; ++slot)
    {
        const int count = (int)(end - begin);
        for (int i = 0; i < count; ++i)
        {
            if (begin[i]->index == slot)
            {
                Player* player = begin[i];
                if (player->index != neutral && player->activity != 0 && !player->activity->blocked)
                    return true;
                break;
            }
        }
    }
    return false;
}

int main()
{
    Activity ready = {false}, blocked = {true};
    Player mainPlayer = {0, &ready};
    Player inactive = {2, &blocked};
    Player missing = {1, 0};
    Player active = {3, &ready};
    Player* empty[1] = {0};
    if (Active(empty, empty, 0)) return 1;
    Player* onlyMain[] = {&mainPlayer};
    if (Active(onlyMain, onlyMain + 1, 0)) return 2;
    Player* gated[] = {&inactive, &missing, &mainPlayer};
    if (Active(gated, gated + 3, 0)) return 3;
    Player* sparse[] = {&active, &mainPlayer};
    if (!Active(sparse, sparse + 2, 0)) return 4;
    Player* onlyActive[] = {&active};
    if (Active(onlyActive, onlyActive + 1, 3)) return 5;
    std::puts("IS_MULTIPLAYER_GAME_ACTIVE_PASS");
    return 0;
}