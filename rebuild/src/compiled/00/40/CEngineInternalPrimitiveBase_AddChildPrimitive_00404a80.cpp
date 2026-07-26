// CEngineInternalPrimitiveBase::AddChildPrimitive @ 00404a80
// Retail: mov eax, 0x13b7cd8 ; ret  -- returns address of a fixed global.
extern int g_primitiveVTableOrData;

unsigned long CEngineInternalPrimitiveBase_AddChildPrimitive()
{
    return (unsigned long)(&g_primitiveVTableOrData);
}