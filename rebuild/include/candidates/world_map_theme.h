#ifndef FABLETLC_WORLD_MAP_THEME_H
#define FABLETLC_WORLD_MAP_THEME_H

#include <stddef.h>
#include "rebuild_abi.h"

struct C2DCoordI
{
    long x;
    long y;
};

enum EThemePaintType
{
    EThemePaintType_Default = 0,
    EThemePaintType_Replace = 1
};

class CMap
{
public:
    unsigned char AddThemeDefIndexToPalette(unsigned long themeId);
    void SetEngineThemeAt(
        long x,
        long y,
        unsigned long themeId,
        unsigned long themeValue,
        EThemePaintType paintType);
    void SetEngineBlendAt(
        long x,
        long y,
        unsigned char blendValue,
        unsigned long themeId,
        EThemePaintType paintType);
    unsigned long GetEngineThemeAt(
        long x,
        long y,
        unsigned long themeId,
        EThemePaintType paintType) const;
    unsigned char GetEngineBlendAt(
        long x,
        long y,
        unsigned long themeId,
        EThemePaintType paintType) const;
};

class CWorldMap
{
public:
    virtual void Slot00();
    virtual void Slot04();
    virtual void Slot08();
    virtual void Slot0C();
    virtual void Slot10();
    virtual void Slot14();
    virtual void Slot18();
    virtual void Slot1C();
    virtual void Slot20();
    virtual void Slot24();
    virtual void Slot28();
    virtual void Slot2C();
    virtual void Slot30();
    virtual void Slot34();
    virtual void Slot38();
    virtual void Slot3C();
    virtual void Slot40();
    virtual long GetMapNumberAt(const C2DCoordI& worldMapCoord) const;
    virtual CMap& GetMap(long mapNumber);
    virtual const CMap& PeekMap(long mapNumber) const;
    virtual void Slot50();
    virtual void Slot54();
    virtual void Slot58();
    virtual void Slot5C();
    virtual void Slot60();
    virtual bool IsPosChangeable(const C2DCoordI& worldMapCoord) const;

    C2DCoordI ConvertWorldMapCoordsToLocalCoords(
        long mapNumber,
        const C2DCoordI& worldMapCoord) const;

    void SetThemeAtBlock(
        const C2DCoordI& worldMapCoord,
        unsigned long themeId,
        unsigned long themeValue,
        EThemePaintType paintType);
    void SetThemeBlendAtBlock(
        const C2DCoordI& worldMapCoord,
        unsigned char blendValue,
        unsigned long themeId,
        EThemePaintType paintType);
    unsigned long GetThemeAtBlock(
        const C2DCoordI& worldMapCoord,
        unsigned long themeId,
        EThemePaintType paintType) const;
    unsigned long GetThemeBlendAtBlock(
        const C2DCoordI& worldMapCoord,
        unsigned long themeId,
        EThemePaintType paintType) const;
};

FABLE_STATIC_ASSERT(sizeof(C2DCoordI) == 8);

#endif
