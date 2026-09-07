#include <stdio.h>

struct Point { float x, y; };
struct Line { Point a, b; int intersects; };
struct Box { float minX, minY, maxX, maxY; };

static bool Contains(const Box& box, const Point& point)
{
    return point.x >= box.minX && point.x <= box.maxX &&
           point.y >= box.minY && point.y <= box.maxY;
}

static bool ModelBlocked(const Box& box, const Line* lines, int count, float epsilon)
{
    for (int i = 0; i < count; ++i) {
        const Line& line = lines[i];
        float minX = line.a.x < line.b.x ? line.a.x : line.b.x;
        float maxX = line.a.x > line.b.x ? line.a.x : line.b.x;
        float minY = line.a.y < line.b.y ? line.a.y : line.b.y;
        float maxY = line.a.y > line.b.y ? line.a.y : line.b.y;
        if (maxX < box.minX - epsilon || minX >= box.maxX + epsilon ||
            maxY < box.minY - epsilon || minY >= box.maxY + epsilon)
            continue;
        if (Contains(box, line.a) || Contains(box, line.b) || line.intersects)
            return true;
    }
    return false;
}

int main()
{
    Box box = {0, 0, 10, 10};
    Line rejected = {{20, 2}, {30, 3}, 1};
    Line endpoint = {{5, 5}, {20, 5}, 0};
    Line crossing = {{-2, 5}, {12, 5}, 1};
    Line miss = {{-2, -2}, {-1, -1}, 0};
    if (ModelBlocked(box, 0, 0, 0.01f) || ModelBlocked(box, &rejected, 1, 0.01f) ||
        !ModelBlocked(box, &endpoint, 1, 0.01f) ||
        !ModelBlocked(box, &crossing, 1, 0.01f) || ModelBlocked(box, &miss, 1, 0.01f))
        return 1;
    puts("NAV_AREA_BLOCKED_BY_LINES_PASS");
    return 0;
}