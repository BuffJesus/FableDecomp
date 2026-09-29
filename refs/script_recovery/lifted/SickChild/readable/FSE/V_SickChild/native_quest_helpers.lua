-- Generated from the same native helper bodies as the quest draft.
local helper_ECE460
function helper_ECE460(quest, me, macroName, argument)
    local resources = quest:RetailResources()
    local actorMap = resources:NewStringMap()
    local resource = resources:NewResource()
    local pScriptObject = resource
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local scratchValue = "NULL" or ""
    local scratchValue2 = argument or ""
    local scratchValue3 = string.lower(scratchValue2) == string.lower(scratchValue) and 0 or 1
    if scratchValue3 ~= 0 then
        local pOther = argument or ""
        resources:SetString(actorMap, "$ARG1", pOther)
    end
    local actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "WITCH", resources:MemberResource("seh_Witch"))
    resources:SetActor(actorMap2, "MUM", resources:MemberResource("seh_Mother"))
    resources:SetActor(actorMap2, "HERO", resource)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(macroName, actorMap2, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(resource)
    resources:DestroyStringMap(actorMap)
end

return {helper_ECE460 = helper_ECE460}
