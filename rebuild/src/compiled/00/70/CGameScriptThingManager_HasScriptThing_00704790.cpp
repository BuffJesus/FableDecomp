struct CThingRetail { unsigned char Reserved[0x18]; unsigned __int64 UniqueID; };
struct CScriptThingInfoRetail { unsigned __int64 ThingUID; void* PScriptThingData; void* PScriptThingInfo; };
struct CScriptThingInfoNodeRetail { CScriptThingInfoNodeRetail* Next; CScriptThingInfoNodeRetail* Previous; CScriptThingInfoRetail Value; };
struct CScriptThingInfoListRetail { CScriptThingInfoNodeRetail* Head; unsigned long Size; };
struct CGameScriptThingManagerRetail { CScriptThingInfoListRetail ScriptThings; bool HasScriptThing(const CThingRetail& thing) const; };

bool CGameScriptThingManagerRetail::HasScriptThing(const CThingRetail& thing) const
{
    CScriptThingInfoNodeRetail* const head = ScriptThings.Head;
    for (CScriptThingInfoNodeRetail* node = head->Next; node != head; node = node->Next) {
        if (node->Value.ThingUID == thing.UniqueID)
            return true;
    }
    return false;
}