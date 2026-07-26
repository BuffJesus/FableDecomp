struct Def { char pad[8]; long classIndex; };
struct CDefinitionManager { char pad[0xa4]; Def** table; };

long __fastcall CDefinitionManager_GetDefClassIndexFromGlobalIndex(CDefinitionManager* self, int edx_unused, int globalIndex)
{
    return self->table[globalIndex]->classIndex;
}