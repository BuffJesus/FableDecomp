#include "fable_world.h"

class CIDrawWorldMap
{
public:
    virtual ~CIDrawWorldMap();
};

class CWorldMapView : public CBaseClassNonCopyable, public CIDrawWorldMap
{
};

const CIDrawWorldMap* CWorld::DrawGetWorldMap() const
{
    return static_cast<const CIDrawWorldMap*>(
        reinterpret_cast<const CWorldMapView*>(PWorldMap));
}
