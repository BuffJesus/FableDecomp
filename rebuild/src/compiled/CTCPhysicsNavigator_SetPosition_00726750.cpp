struct Vec3 { float x, y, z; };
extern int __fastcall CTCPhysicsNavigator_Notify(void* self, int edxpass, const Vec3* v);
void __fastcall CTCPhysicsNavigator_SetPosition(void* self, int edxpass, const Vec3* v)
{
    CTCPhysicsNavigator_Notify(self, edxpass, v);
    *(Vec3*)((char*)self + 0x89) = *v;
}