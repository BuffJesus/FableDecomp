#include <cstdio>

struct Model {
    unsigned char enabled;
    int notifyCalls;
    int lastArg;
    void notify(int a){ notifyCalls++; lastArg=a; }
    void SetMiniMapEnabled(bool e){
        if (enabled == (unsigned char)e) return;
        enabled = (unsigned char)e;
        if (e) return;
        notify(0);
    }
};

int main(){
    // start disabled(0), enable -> changes, e!=0 so no notify
    Model m; m.enabled=0; m.notifyCalls=0; m.lastArg=-1;
    m.SetMiniMapEnabled(true);
    if (m.enabled!=1 || m.notifyCalls!=0) { printf("bad1\n"); return 1; }

    // enable again -> no change
    m.SetMiniMapEnabled(true);
    if (m.notifyCalls!=0) { printf("bad2\n"); return 1; }

    // disable -> changes and e==0 -> notify(0)
    m.SetMiniMapEnabled(false);
    if (m.enabled!=0 || m.notifyCalls!=1 || m.lastArg!=0) { printf("bad3\n"); return 1; }

    // disable again -> no change
    m.SetMiniMapEnabled(false);
    if (m.notifyCalls!=1) { printf("bad4\n"); return 1; }

    printf("BEHAVIOR_OK\n");
    return 0;
}