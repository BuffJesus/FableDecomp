struct CActionInputControl { int x; };
struct Vec { CActionInputControl* begin; CActionInputControl* end; };
struct CUserProfileManager {
    char pad[0x54];
    Vec inputs;
};
void __fastcall Populate(CUserProfileManager* self);

Vec* __fastcall CUserProfileManager_GetAssignedInputsPassive(CUserProfileManager* self)
{
    Vec* v = &self->inputs;
    if (v->begin == v->end)
        Populate(self);
    return v;
}