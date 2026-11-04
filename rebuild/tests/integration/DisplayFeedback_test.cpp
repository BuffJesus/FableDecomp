#include "fable_game_event_dispatch.h"
#include <stdio.h>
#include <string.h>

static const CGameEvent* expectedEvent[40];
static void* expectedThis[40];
static unsigned calls, errors;
static unsigned order[40];
struct Prefix { virtual void Padding() {} long Guard; };
struct Boundary : Prefix, CDisplayEngine {
    void Feedback(const CGameEvent* event) {
        if (this!=expectedThis[calls] || event!=expectedEvent[calls]) ++errors;
        order[calls++]=event->Type;
    }
};
struct DirectBoundary : CDisplayEngine {
    void Feedback(const CGameEvent* event) {
        if (this!=expectedThis[calls] || event!=expectedEvent[calls]) ++errors;
        order[calls++]=event->Type;
    }
};
typedef CGameEventDispatch::Feedback Handler;
#define DIRECT static_cast<Handler>(&DirectBoundary::Feedback)
#define ADJUSTED static_cast<Handler>(&Boundary::Feedback)
const CGameEventDispatch CMainGameComponent::EventDispatch[87] = {
    {0,0,0,0}, {0,0,DIRECT,0}, {0,0,0,DIRECT}, {0,0,DIRECT,DIRECT},
    {0,0,ADJUSTED,0}, {0,0,0,ADJUSTED}, {0,0,ADJUSTED,ADJUSTED}, {0,0,0,0}
};

struct CGameEventDispatchTestAccess {
    static long Adjustment(unsigned type, bool confirm) {
        const CGameEventDispatch& d=CMainGameComponent::EventDispatch[type];
        Handler h=confirm?d.ConfirmFunction:d.FeedbackFunction;
        return reinterpret_cast<const long*>(&h)[1];
    }
};

int main() {
    unsigned scenarios=0;
    for (unsigned pattern=0; pattern<32; ++pattern) {
        unsigned long storage[70]; memset(storage,pattern,sizeof(storage));
        CDisplayEngine* display=reinterpret_cast<CDisplayEngine*>(&storage[3]);
        // Table lookup is static in retail and must not read this pointer.
        display->Component=0;
        unsigned long before[70]; memcpy(before,storage,sizeof(before));
        for (unsigned type=0; type<87; ++type) {
            CGameEvent event(type); expectedEvent[0]=&event;
            expectedThis[0]=reinterpret_cast<char*>(display)+CGameEventDispatchTestAccess::Adjustment(type,false);
            calls=errors=0; display->ReceiveFeedbackGameEvent(&event);
            unsigned expected=(type==1 || type==3 || type==4 || type==6)?1:0;
            if (calls!=expected || errors || (calls && order[0]!=type) || memcmp(before,storage,sizeof(before))) return 1;
            ++scenarios;
        }
        // Single-event packages verify all confirm rows and pointer adjustments.
        for (unsigned type=0; type<87; ++type) {
            CGameEventPackage package; package.NoEvents=1; package.Events[0].Type=type;
            expectedEvent[0]=&package.Events[0];
            expectedThis[0]=reinterpret_cast<char*>(display)+CGameEventDispatchTestAccess::Adjustment(type,true);
            calls=errors=0; display->ConfirmFeedbackGameEventPackage(&package);
            unsigned expected=(type==2 || type==3 || type==5 || type==6)?1:0;
            if (calls!=expected || errors || (calls && order[0]!=type) || memcmp(before,storage,sizeof(before))) return 2;
            ++scenarios;
        }
        for (unsigned n=0; n<=40; ++n) for (unsigned rotation=0; rotation<8; ++rotation) {
            CGameEventPackage package; package.NoEvents=n;
            unsigned expected=0;
            for (unsigned i=0; i<n; ++i) {
                unsigned type=(i+rotation)%8; package.Events[i].Type=type;
                if (type==2 || type==3 || type==5 || type==6) {
                    expectedEvent[expected]=&package.Events[i];
                    expectedThis[expected]=reinterpret_cast<char*>(display)+CGameEventDispatchTestAccess::Adjustment(type,true);
                    ++expected;
                }
            }
            calls=errors=0; display->ConfirmFeedbackGameEventPackage(&package);
            if (calls!=expected || errors || memcmp(before,storage,sizeof(before))) return 3;
            for (unsigned i=0; i<expected; ++i) if (order[i]!=unsigned(expectedEvent[i]->Type)) return 4;
            ++scenarios;
        }
    }
    printf("DISPLAY_FEEDBACK_BEHAVIOR_PASS scenarios=%u\n",scenarios);
    return 0;
}
