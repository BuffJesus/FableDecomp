struct CEngine { unsigned char pad[0x10]; unsigned int flags; };

bool __stdcall Is3DSceneRenderingEnabled(CEngine* self)
{
    return (self->flags & 0x217) != 0;
}