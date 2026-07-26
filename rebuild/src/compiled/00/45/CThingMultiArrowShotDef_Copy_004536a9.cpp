struct CThingMultiArrowShotDef;

extern void __fastcall CThingMultiArrowShotDef_CopyBase(CThingMultiArrowShotDef* self, void* edx, const CThingMultiArrowShotDef* other);

void __fastcall CThingMultiArrowShotDef_Copy(CThingMultiArrowShotDef* self, void* edx, const CThingMultiArrowShotDef* other)
{
    CThingMultiArrowShotDef_CopyBase(self, edx, other);
}