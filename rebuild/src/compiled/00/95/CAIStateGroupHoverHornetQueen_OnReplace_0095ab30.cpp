#include "engine/CAIStateGroup_HoverHornetQueen.h"

struct CircleMarkerStorage {
    void Method(unsigned int currentCirclePoint, unsigned int firstMarker);
};
struct CAIStateGroup_HoverHornetQueen_Methods : CAIStateGroup_HoverHornetQueen {
    void OnReplaceBase();
    void OnReplace();
};
void CAIStateGroup_HoverHornetQueen_Methods::OnReplace() {
    OnReplaceBase();
    CAIStateGroup_HoverHornetQueen* state =
        reinterpret_cast<CAIStateGroup_HoverHornetQueen*>(this);
    unsigned int currentCirclePoint = (unsigned int)state->CurrentCirclePoint;
    unsigned int firstMarker = *reinterpret_cast<unsigned int*>(state->CircleMarkers);
    reinterpret_cast<CircleMarkerStorage*>(&state->CurrentCirclePoint)->Method(
        currentCirclePoint, firstMarker);
}
