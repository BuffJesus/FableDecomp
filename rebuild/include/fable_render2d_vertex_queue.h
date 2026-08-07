#pragma once

#include <stddef.h>

#include "rebuild_abi.h"
#include "fable_display_viewport.h"
#include "fable_string.h"
#include "fable_texture_lifecycle.h"

struct Render2DVertexView
{
    fable_u8 bytes[0x20];
};

FABLE_STATIC_ASSERT(sizeof(Render2DVertexView) == 0x20);

// Retail CQuickDrawTriInfo is a tagged 0x3c-byte record.  The first byte
// selects the normal geometry versus immediate-text interpretation; the
// payload after +0x20 is therefore deliberately represented as a union.
// These offsets come from the recovered 0x009DA9F0 Render2DDrawList body.
struct Render2DQuickDrawInfoView
{
    fable_u8 flags00;
    fable_u8 padding01[3];
    C2DBoxF window04;
    // The queue owns this slot as a CWideString, but the view is deliberately
    // non-owning: constructing a C++ CWideString here would add lifetime
    // calls that Render2DDrawList never makes while consuming the vector.
    fable_u8 text14[4];
    CTextureAssignmentView texture18;
    union
    {
        fable_u32 renderState20;
        struct
        {
            float textPosition20[3];
            float textScale2c;
        } text;
    } payload20;
    fable_u8 unused30[4];
    fable_u32 textColour34;
    void* fontBank38;
};

FABLE_STATIC_ASSERT(sizeof(Render2DQuickDrawInfoView) == 0x3C);
FABLE_STATIC_ASSERT(offsetof(Render2DQuickDrawInfoView, window04) == 0x04);
FABLE_STATIC_ASSERT(offsetof(Render2DQuickDrawInfoView, text14) == 0x14);
FABLE_STATIC_ASSERT(offsetof(Render2DQuickDrawInfoView, texture18) == 0x18);
FABLE_STATIC_ASSERT(offsetof(Render2DQuickDrawInfoView, payload20) == 0x20);
FABLE_STATIC_ASSERT(offsetof(Render2DQuickDrawInfoView, unused30) == 0x30);
FABLE_STATIC_ASSERT(offsetof(Render2DQuickDrawInfoView, textColour34) == 0x34);
FABLE_STATIC_ASSERT(offsetof(Render2DQuickDrawInfoView, fontBank38) == 0x38);

struct Render2DDrawListBlockView
{
    Render2DDrawListBlockView* begin00;
    Render2DDrawListBlockView* end04;
    fable_u8 unknown08[0x18];

    Render2DDrawListBlockView* CopyBlock(
        Render2DDrawListBlockView* destination,
        const Render2DDrawListBlockView* source);
};

FABLE_STATIC_ASSERT(sizeof(Render2DDrawListBlockView) == 0x20);

// The recovered CRenderManager2D owns two adjacent vectors.  These offsets
// are the native queue boundary consumed by Render2DDrawList; keeping the
// queue as an offset view prevents the presenter from silently substituting a
// different record format.
struct CRenderManager2DQueueView
{
    fable_u8 unknown0000[0x3E88];
    Render2DVertexView* vertexBegin3e88;
    Render2DVertexView* vertexEnd3e8c;
    Render2DVertexView* vertexCapacity3e90;
    Render2DQuickDrawInfoView* infoBegin3e94;
    Render2DQuickDrawInfoView* infoEnd3e98;
    Render2DQuickDrawInfoView* infoCapacity3e9c;
};

FABLE_STATIC_ASSERT(
    offsetof(CRenderManager2DQueueView, vertexBegin3e88) == 0x3E88);
FABLE_STATIC_ASSERT(
    offsetof(CRenderManager2DQueueView, vertexEnd3e8c) == 0x3E8C);
FABLE_STATIC_ASSERT(
    offsetof(CRenderManager2DQueueView, vertexCapacity3e90) == 0x3E90);
FABLE_STATIC_ASSERT(
    offsetof(CRenderManager2DQueueView, infoBegin3e94) == 0x3E94);
FABLE_STATIC_ASSERT(
    offsetof(CRenderManager2DQueueView, infoEnd3e98) == 0x3E98);
FABLE_STATIC_ASSERT(
    offsetof(CRenderManager2DQueueView, infoCapacity3e9c) == 0x3E9C);
