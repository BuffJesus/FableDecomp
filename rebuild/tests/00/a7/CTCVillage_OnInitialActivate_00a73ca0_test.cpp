#include <stdio.h>

struct Callee { int calls; void* lastEdx; bool lastFlag; };

static Callee g_obj;

void __fastcall Callee_method(Callee* obj, void* edxArg, bool flag) {
    obj->calls++;
    obj->lastEdx = edxArg;
    obj->lastFlag = flag;
}

struct CTCVillage {
    Callee*       field0;
    void*         field4;
    int           dummy8;
    bool          fieldC;
    unsigned char fieldD;
};

void __fastcall OnInitialActivate(CTCVillage* self) {
    if (self->fieldD) {
        Callee_method(self->field0, self->field4, self->fieldC);
        self->fieldD = 0;
    }
}

int main() {
    int ok = 1;

    // Case 1: guard set -> callee runs, args forwarded, guard cleared.
    g_obj.calls = 0; g_obj.lastEdx = 0; g_obj.lastFlag = false;
    CTCVillage v;
    int edxMarker = 12345;
    v.field0 = &g_obj;
    v.field4 = &edxMarker;
    v.dummy8 = 0;
    v.fieldC = true;
    v.fieldD = 1;
    OnInitialActivate(&v);
    if (g_obj.calls != 1) ok = 0;
    if (g_obj.lastEdx != &edxMarker) ok = 0;
    if (g_obj.lastFlag != true) ok = 0;
    if (v.fieldD != 0) ok = 0;

    // Case 2: guard clear -> callee does not run.
    g_obj.calls = 0;
    v.fieldD = 0;
    OnInitialActivate(&v);
    if (g_obj.calls != 0) ok = 0;

    // Case 3: fieldC false forwards false.
    g_obj.calls = 0; g_obj.lastFlag = true;
    v.fieldC = false;
    v.fieldD = 1;
    OnInitialActivate(&v);
    if (g_obj.calls != 1) ok = 0;
    if (g_obj.lastFlag != false) ok = 0;

    if (ok) printf("OIA_PARITY_OK\n");
    else printf("FAIL\n");
    return 0;
}