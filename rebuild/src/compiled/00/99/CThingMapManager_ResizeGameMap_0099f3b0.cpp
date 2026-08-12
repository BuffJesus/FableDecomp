struct C2DBoxI { int a, b, c, d; };

struct CThingMap {
    void ResizeMap(long v, C2DBoxI* box);
};

struct CThingMapManager {
    CThingMap* map;      // +0x00
    void Prepare();      // call 0x99eb40
    void ResizeGameMap(long v, C2DBoxI* box);
};

void CThingMapManager::ResizeGameMap(long v, C2DBoxI* box)
{
    Prepare();
    map->ResizeMap(v, box);
}