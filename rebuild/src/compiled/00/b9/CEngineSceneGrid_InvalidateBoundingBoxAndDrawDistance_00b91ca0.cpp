#include "engine/CEngineSceneGrid.h"
#include "engine/CEngineSceneGridCell.h"

void CEngineSceneGrid::InvalidateBoundingBoxAndDrawDistance(unsigned long x,unsigned long y,bool children)
{
    CEngineSceneGridCell* cell=GridCells[x+y*Size];
    if(cell) {
        cell->BoundingBoxValid=0;
        if(children && LevelDown)
            for(unsigned long dy=0;dy<2;++dy)
                for(unsigned long dx=0;dx<2;++dx)
                    LevelDown->InvalidateBoundingBoxAndDrawDistance(x*2+dx,y*2+dy,true);
    }
}
