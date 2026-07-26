#include "candidates/background_tree_node_compiled.h"

static void ReadBackgroundHeaderValue(
    CDataInputStream& input,
    void* destination,
    unsigned long size)
{
    if (static_cast<unsigned long>(input.ReadPosition) + size >= 0x80000000UL)
        return;

    if (input.Available < static_cast<fable_i32>(size))
    {
        input.ReadWithSrcChunkOverflow(static_cast<fable_u8*>(destination), size);
        return;
    }

    if (size == 1)
        *static_cast<fable_u8*>(destination) = *input.Source;
    else if (size == 2)
        *static_cast<fable_u16*>(destination) = *reinterpret_cast<fable_u16*>(input.Source);
    else
        *static_cast<fable_u32*>(destination) = *reinterpret_cast<fable_u32*>(input.Source);

    input.Source += size;
    input.Available -= static_cast<fable_i32>(size);
    input.ReadPosition += static_cast<fable_i32>(size);
}

void CLandscapeBackgroundTreeNode::LoadHeader(CDataInputStream& input)
{
    ReadBackgroundHeaderValue(input, &HeaderWord0, 2);
    ReadBackgroundHeaderValue(input, &HeaderWord1, 2);
    ReadBackgroundHeaderValue(input, &HeaderWord2, 2);
    ReadBackgroundHeaderValue(input, &HeaderWord3, 2);
    ReadBackgroundHeaderValue(input, &UnknownByte0, 1);
    ReadBackgroundHeaderValue(input, &LayerStartIndex, 1);
    ReadBackgroundHeaderValue(input, &LayerEndIndex, 1);

    ReadBackgroundHeaderValue(input, &HeaderVector0X, 4);
    ReadBackgroundHeaderValue(input, &HeaderVector0Y, 4);
    ReadBackgroundHeaderValue(input, &HeaderVector0Z, 4);
    ReadBackgroundHeaderValue(input, &HeaderVector1X, 4);
    ReadBackgroundHeaderValue(input, &HeaderVector1Y, 4);
    ReadBackgroundHeaderValue(input, &HeaderVector1Z, 4);
    ReadBackgroundHeaderValue(input, &HeaderVector2X, 4);
    ReadBackgroundHeaderValue(input, &HeaderVector2Y, 4);
    ReadBackgroundHeaderValue(input, &HeaderVector2Z, 4);

    if (LayerStartIndex <= 7 && LayerEndIndex >= LayerStartIndex)
    {
        unsigned int index = LayerStartIndex;
        do
        {
            CLandscapeBackgroundLayerEntry& entry = LayerEntries[index];
            ReadBackgroundHeaderValue(input, &entry.Selector, 1);
            ReadBackgroundHeaderValue(input, &entry.X, 4);
            ReadBackgroundHeaderValue(input, &entry.Y, 4);
            ReadBackgroundHeaderValue(input, &entry.Z, 4);
            ++index;
        } while (index <= LayerEndIndex);
    }

    HeaderLoaded = 1;
}
