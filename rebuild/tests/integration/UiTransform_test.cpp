#include "fable_ui_transform.h"
#include <stdio.h>
#include <string.h>

unsigned char FableUiCoordinateConversionEnabled;
FableUiStateVector2 FableUiCoordinateSourceExtent, FableUiCoordinateDestinationExtent;
static unsigned independentBits, relative, mode, independentCalls, scaleCalls, varyingScale;
static FableUiStateVector2 managerScale;
static bool __fastcall Independent(FableUiComponentDrawView* component, void*)
{
    printf(" I%u", independentCalls);
    if (mode == 2 && independentCalls == 0) component->Parent = 0;
    return ((independentBits >> independentCalls++) & 1) != 0;
}
static bool __fastcall Relative(FableUiComponentDrawView* component, void*)
{
    printf(" R");
    if (mode == 1) { component->Position.x = 5; component->Zoom.x = 7; component->RelativeParentZoom.x = 2; }
    return relative != 0;
}
void* __cdecl FableUiGetManager() { printf(" M"); return reinterpret_cast<void*>(0x1234); }
FableUiStateVector2* __fastcall FableUiGetManagerScale(void* manager, void*, FableUiStateVector2* output)
{
    if (manager != reinterpret_cast<void*>(0x1234)) return 0;
    printf(" S%u", scaleCalls);
    output->x = managerScale.x + (varyingScale ? scaleCalls*0.125f : 0);
    output->y = managerScale.y + (varyingScale ? scaleCalls*0.25f : 0);
    ++scaleCalls; return output;
}
static float Float(unsigned value) { float result; memcpy(&result, &value, 4); return result; }
static FableUiStateVector2 Vector(const unsigned* value)
{ FableUiStateVector2 result = {Float(value[0]), Float(value[1])}; return result; }
static void Print(float value) { unsigned bits; memcpy(&bits, &value, 4); printf(" %u", bits); }
static void PrintVector(const FableUiStateVector2& value) { Print(value.x); Print(value.y); }
int main(int argc, char** argv)
{
    if (argc != 2) return 2;
    FILE* input = fopen(argv[1], "r"); if (!input) return 3;
    unsigned value[30];
    while (fscanf(input, "%u", &value[0]) == 1)
    {
        for (unsigned i = 1; i < 30; ++i) if (fscanf(input, "%u", &value[i]) != 1) return 4;
        FableUiComponentDrawView component; memset(&component, 0, sizeof(component));
        FableUiComponentDrawVtable table = {};
        table.IsPositionIndependent = table.IsZoomIndependent = Independent;
        table.UseRelativePosition = table.UseRelativeZoom = Relative;
        component.Vtable = &table; component.Parent = value[4] ? &component : 0;
        independentBits = value[1]; relative = value[2]; FableUiCoordinateConversionEnabled = static_cast<unsigned char>(value[3]);
        mode = value[5]; independentCalls = scaleCalls = 0;
        component.PositionTimeElapsed = component.ZoomTimeElapsed = Float(value[7]);
        component.PositionTime = component.ZoomTime = Float(value[8]);
        component.Position = component.Zoom = Vector(value+9);
        component.TargetPosition = component.TargetZoom = Vector(value+11);
        component.InitialPosition = component.InitialZoom = Vector(value+13);
        component.ParentPosition = Vector(value+15); component.ParentZoom = Vector(value+17);
        component.RelativeParentPosition = Vector(value+19); component.RelativeParentZoom = Vector(value+21);
        FableUiCoordinateSourceExtent = Vector(value+23); FableUiCoordinateDestinationExtent = Vector(value+25);
        managerScale = Vector(value+27); varyingScale = value[29];
        printf("TRACE");
        FableUiStateVector2 converted;
        if (value[0] == 0) FableUiUpdatePosition(&component, 0, Float(value[6]));
        else if (value[0] == 1) FableUiUpdateZoom(&component, 0, Float(value[6]));
        else if (FableUiConvertCoordinates(&converted, 0, component.Position) != &converted) return 5;
        printf(" END");
        if (value[0] == 0)
        {
            PrintVector(component.Position); PrintVector(component.InitialPosition); PrintVector(component.RenderPosition);
            PrintVector(component.RelativeRenderPosition); Print(component.PositionTimeElapsed);
        }
        else if (value[0] == 1)
        {
            PrintVector(component.Zoom); PrintVector(component.InitialZoom); PrintVector(component.RenderZoom);
            PrintVector(component.RelativeRenderZoom); Print(component.ZoomTimeElapsed);
        }
        else PrintVector(converted);
        printf("\n");
    }
    fclose(input); return 0;
}
