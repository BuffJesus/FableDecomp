#include <cstdio>
struct Key { int id; };
struct Array {
    void* begin; void* end; void* seenFirst; void* seenLast; int erases;
};
struct Map {
    int observedKeyId;
    Array entries;
    Array& Lookup(const Key& key) { observedKeyId = key.id; return entries; }
};
static int copies, destroys;
static void Run(Map& map, const Key& input) {
    Key local = input;
    ++copies;
    Array& entries = map.Lookup(local);
    entries.seenFirst = entries.begin;
    entries.seenLast = entries.end;
    ++entries.erases;
    ++destroys;
}
int main() {
    int first, last;
    Map map = {0, {&first, &last, 0, 0, 0}};
    Key key = {42};
    Run(map, key);
    if (copies != 1 || destroys != 1 || map.observedKeyId != 42) return 1;
    if (map.entries.seenFirst != &first || map.entries.seenLast != &last) return 2;
    if (map.entries.erases != 1) return 3;
    std::printf("CLEAR_GOSSIP_PASS\n");
    return 0;
}
