extern int g_OnEngineSurveyLMBHeld_vtbl;
struct Payload { unsigned int a; unsigned int b; };
struct Obj { void* vtbl; void* owner; unsigned int a; unsigned int b; };
struct Obj* __fastcall OnEngineSurveyLMBHeld(struct Obj* self, void* /*edx*/, void* owner, struct Payload* src)
{
    self->owner = owner;
    self->vtbl  = &g_OnEngineSurveyLMBHeld_vtbl;
    self->a = src->a;
    self->b = src->b;
    return self;
}