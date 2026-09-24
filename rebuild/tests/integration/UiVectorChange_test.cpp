#include "fable_ui_transform.h"
#include <stdio.h>
#include <string.h>

unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateSourceExtent, FableUiCoordinateDestinationExtent;
void* FableUiScaleContext;
static unsigned relative, mutation;
static bool __fastcall Relative(FableUiComponentDrawView* component, void*)
{
    printf(" R");
    if (mutation) { component->Position.x = 17; component->Position.y = -23; }
    return relative != 0;
}
void* __cdecl FableUiGetManager() { printf(" M"); return 0; }
static float Float(unsigned value) { float result; memcpy(&result, &value, 4); return result; }
static void Print(float value) { unsigned result; memcpy(&result, &value, 4); printf(" %u", result); }
int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    unsigned v[20];
    while (fscanf(input, "%u", v) == 1)
    {
        for (unsigned i = 1; i < 20; ++i) if (fscanf(input, "%u", v+i) != 1) return 4;
        FableUiComponentDrawView c; memset(&c, 0, sizeof(c));
        FableUiComponentDrawVtable table = {};
        table.ChangePosition = FableUiChangePosition; table.ChangeZoom = FableUiChangeZoom;
        table.UseRelativePosition = Relative; c.Vtable = &table;
        relative = v[1]; mutation = v[2]; FableUiCoordinateConversionEnabled = static_cast<unsigned char>(v[3]);
        FableUiScaleContext = v[4] ? &c : 0;
        c.Position.x = c.Zoom.x = Float(v[8]); c.Position.y = c.Zoom.y = Float(v[9]);
        c.TargetPosition.x = c.TargetZoom.x = 33; c.TargetPosition.y = c.TargetZoom.y = 44;
        c.InitialPosition.x = c.InitialZoom.x = -55; c.InitialPosition.y = c.InitialZoom.y = -66;
        c.PositionTimeElapsed = c.ZoomTimeElapsed = Float(v[12]);
        FableUiStateVector2 value = {Float(v[10]), Float(v[11])};
        FableUiCoordinateSourceExtent.x = Float(v[13]); FableUiCoordinateSourceExtent.y = Float(v[14]);
        FableUiCoordinateDestinationExtent.x = Float(v[15]); FableUiCoordinateDestinationExtent.y = Float(v[16]);
        FableUiStateVector2* argument = &value;
        if (v[17]) argument = reinterpret_cast<FableUiStateVector2*>(reinterpret_cast<unsigned char*>(&c)+v[17]);
        printf("TRACE");
        if (v[0] == 4) FableUiConvertCoordinatesInverse(&value, 0, value);
        else
        {
            FableUiComponentVectorDelta methods[] = {FableUiChangePosition, FableUiChangeZoom, FableUiChangePositionDelta, FableUiChangeZoomDelta};
            methods[v[0]](&c, 0, argument, Float(v[6]), v[7] != 0);
        }
        printf(" END");
        if (v[0] == 4) { Print(value.x); Print(value.y); }
        else
        {
            const float* fields = reinterpret_cast<const float*>(&c.Position);
            for (unsigned j = 0; j < 16; ++j) Print(fields[j]);
            Print(c.PositionTimeElapsed); Print(c.PositionTime); Print(c.ZoomTimeElapsed); Print(c.ZoomTime);
        }
        printf("\n");
    }
    fclose(input); return 0;
}
