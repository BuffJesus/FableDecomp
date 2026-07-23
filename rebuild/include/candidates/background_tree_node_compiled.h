#ifndef FABLETLC_BACKGROUND_TREE_NODE_COMPILED_H
#define FABLETLC_BACKGROUND_TREE_NODE_COMPILED_H

#include <stddef.h>
#include "rebuild_abi.h"

class CLandscapeBackgroundLoadTask
{
public:
    virtual void Slot00() = 0;
    virtual void Slot04() = 0;
    virtual void Slot08() = 0;
    virtual void Slot0C() = 0;
    virtual void Slot10() = 0;
    virtual void Abort() = 0;
};

class CDataInputStream
{
public:
    fable_u8 Unknown00[4];
    fable_i32 ReadPosition;
    fable_u8 Unknown08[4];
    fable_u8* Source;
    fable_u8 Unknown10[4];
    fable_i32 Available;

    void ReadWithSrcChunkOverflow(fable_u8* destination, unsigned long size);
};

struct CLandscapeBackgroundLayerEntry
{
    fable_u32 X;
    fable_u32 Y;
    fable_u32 Z;
    fable_u8 Selector;
    fable_u8 Padding[3];
};

class CLandscapeBackgroundTreeNode
{
public:
    fable_u8 Unknown00[0x0C];
    CLandscapeBackgroundLayerEntry LayerEntries[8];
    fable_u32 HeaderVector0X;
    fable_u32 HeaderVector0Y;
    fable_u32 HeaderVector0Z;
    fable_u8 Unknown98[8];
    CLandscapeBackgroundLoadTask* AsyncLoadTask;
    fable_u8 UnknownA4[4];
    fable_u32 HeaderVector1X;
    fable_u32 HeaderVector1Y;
    fable_u32 HeaderVector1Z;
    fable_u32 HeaderVector2X;
    fable_u32 HeaderVector2Y;
    fable_u32 HeaderVector2Z;
    fable_u8 UnknownC0[0x18];
    fable_u16 HeaderWord0;
    fable_u16 HeaderWord1;
    fable_u16 HeaderWord2;
    fable_u16 HeaderWord3;
    fable_u8 UnknownByte0;
    fable_u8 LayerStartIndex;
    fable_u8 LayerEndIndex;
    bool TesselationValid;
    fable_u8 UnknownE4[2];
    fable_u8 HeaderLoaded;

    void UpdateTesselation();
    void LoadHeader(CDataInputStream& input);
};

FABLE_STATIC_ASSERT(sizeof(CDataInputStream) == 0x18);
FABLE_STATIC_ASSERT(sizeof(CLandscapeBackgroundLayerEntry) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, LayerEntries) == 0x0C);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, HeaderVector0X) == 0x8C);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, AsyncLoadTask) == 0xA0);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, HeaderVector1X) == 0xA8);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, HeaderVector2X) == 0xB4);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, HeaderWord0) == 0xD8);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, LayerStartIndex) == 0xE1);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, TesselationValid) == 0xE3);
FABLE_STATIC_ASSERT(offsetof(CLandscapeBackgroundTreeNode, HeaderLoaded) == 0xE6);

#endif
