#include <stdio.h>
#include <string.h>

struct Box { long left, top, right, bottom; };
struct Trace { int potentialSearch, namedSearch, copied, destroyed, reserve, freed; Box box; };

static long Model(const char* name, long x, long y, long radius,
                  long foundCount, long existingCount, Trace* trace)
{
    trace->box.left = x - radius;
    trace->box.top = y - radius;
    trace->box.right = x + radius;
    trace->box.bottom = y + radius;
    if (name == 0 || name[0] == 0) {
        ++trace->potentialSearch;
    } else {
        ++trace->copied;
        ++trace->namedSearch;
        ++trace->destroyed;
    }
    if (foundCount == 0) return 0;
    trace->reserve = (int)foundCount;
    trace->freed = 1;
    return existingCount + foundCount;
}

int main()
{
    Trace empty = {};
    if (Model("", 10, 20, 3, 0, 0, &empty) != 0 || empty.potentialSearch != 1 ||
        empty.namedSearch != 0 || empty.box.left != 7 || empty.box.top != 17 ||
        empty.box.right != 13 || empty.box.bottom != 23)
        return 1;
    Trace named = {};
    if (Model("guard", -4, 8, 5, 3, 2, &named) != 5 || named.namedSearch != 1 ||
        named.copied != 1 || named.destroyed != 1 || named.potentialSearch != 0 ||
        named.reserve != 3 || named.freed != 1 || named.box.left != -9 ||
        named.box.right != 1)
        return 2;
    puts("GET_CREATURES_AREA_SCRIPT_NAME_PASS");
    return 0;
}