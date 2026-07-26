#include <stdio.h>
#include <string.h>
#include "candidates/background_tree_node_compiled.h"

static fable_u8* g_overflowSource;
static int g_overflowCalls;

void CDataInputStream::ReadWithSrcChunkOverflow(fable_u8* destination, unsigned long size)
{
    ++g_overflowCalls;
    for (unsigned long index = 0; index < size; ++index)
        destination[index] = *g_overflowSource++;
    ReadPosition += static_cast<fable_i32>(size);
}

static void Put8(fable_u8*& output, fable_u8 value)
{
    *output++ = value;
}

static void Put16(fable_u8*& output, fable_u16 value)
{
    *output++ = static_cast<fable_u8>(value);
    *output++ = static_cast<fable_u8>(value >> 8);
}

static void Put32(fable_u8*& output, fable_u32 value)
{
    Put16(output, static_cast<fable_u16>(value));
    Put16(output, static_cast<fable_u16>(value >> 16));
}

static int CheckFixedHeader(const CLandscapeBackgroundTreeNode& node)
{
    if (node.HeaderWord0 != 0x1001 || node.HeaderWord1 != 0x1002 ||
        node.HeaderWord2 != 0x1003 || node.HeaderWord3 != 0x1004) return 0;
    if (node.UnknownByte0 != 0x55) return 0;
    if (node.HeaderVector0X != 0x20000001 || node.HeaderVector0Y != 0x20000002 ||
        node.HeaderVector0Z != 0x20000003) return 0;
    if (node.HeaderVector1X != 0x30000001 || node.HeaderVector1Y != 0x30000002 ||
        node.HeaderVector1Z != 0x30000003) return 0;
    if (node.HeaderVector2X != 0x40000001 || node.HeaderVector2Y != 0x40000002 ||
        node.HeaderVector2Z != 0x40000003) return 0;
    return node.HeaderLoaded == 1;
}

static int BuildPayload(fable_u8* bytes, fable_u8 start, fable_u8 end)
{
    fable_u8* output = bytes;
    Put16(output, 0x1001); Put16(output, 0x1002);
    Put16(output, 0x1003); Put16(output, 0x1004);
    Put8(output, 0x55); Put8(output, start); Put8(output, end);
    Put32(output, 0x20000001); Put32(output, 0x20000002); Put32(output, 0x20000003);
    Put32(output, 0x30000001); Put32(output, 0x30000002); Put32(output, 0x30000003);
    Put32(output, 0x40000001); Put32(output, 0x40000002); Put32(output, 0x40000003);
    if (start <= 7 && end >= start)
    {
        for (unsigned int index = start; index <= end; ++index)
        {
            Put8(output, static_cast<fable_u8>(0x80 + index));
            Put32(output, 0x50000000 + index);
            Put32(output, 0x60000000 + index);
            Put32(output, 0x70000000 + index);
        }
    }
    return static_cast<int>(output - bytes);
}

int main()
{
    fable_u8 bytes[128];
    CLandscapeBackgroundTreeNode node;
    CDataInputStream input;

    memset(&node, 0, sizeof(node));
    memset(&input, 0, sizeof(input));
    int size = BuildPayload(bytes, 1, 2);
    input.Source = bytes;
    input.Available = size;
    node.LoadHeader(input);
    if (!CheckFixedHeader(node) || input.ReadPosition != 73 || input.Available != 0) return 1;
    if (node.LayerStartIndex != 1 || node.LayerEndIndex != 2) return 2;
    if (node.LayerEntries[1].Selector != 0x81 || node.LayerEntries[1].X != 0x50000001 ||
        node.LayerEntries[1].Y != 0x60000001 || node.LayerEntries[1].Z != 0x70000001) return 3;
    if (node.LayerEntries[2].Selector != 0x82 || node.LayerEntries[2].X != 0x50000002 ||
        node.LayerEntries[2].Y != 0x60000002 || node.LayerEntries[2].Z != 0x70000002) return 4;

    memset(&node, 0, sizeof(node));
    memset(&input, 0, sizeof(input));
    size = BuildPayload(bytes, 8, 9);
    input.Source = bytes;
    input.Available = size;
    node.LoadHeader(input);
    if (!CheckFixedHeader(node) || input.ReadPosition != 47 || node.LayerStartIndex != 8) return 5;

    memset(&node, 0, sizeof(node));
    memset(&input, 0, sizeof(input));
    size = BuildPayload(bytes, 0, 0);
    g_overflowSource = bytes;
    g_overflowCalls = 0;
    node.LoadHeader(input);
    if (!CheckFixedHeader(node) || input.ReadPosition != 60 || g_overflowCalls != 20) return 6;
    if (node.LayerEntries[0].Selector != 0x80 || node.LayerEntries[0].Z != 0x70000000) return 7;

    printf("BACKGROUND_TREE_LOAD_HEADER_TEST PASS\n");
    return 0;
}
