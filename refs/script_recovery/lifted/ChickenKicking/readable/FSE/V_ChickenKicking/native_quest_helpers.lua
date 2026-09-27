-- Generated from the same native helper bodies as the quest draft.
local helper_E68B20
function helper_E68B20(quest, me, strParam1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    local pScriptObject = resource
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "CHICKEN", resources:MemberResource("seh_Chicken"))
    resources:SetActor(actorMap, "ORGANISER", resources:MemberResource("seh_ChickenMaster"))
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(strParam1, actorMap, resources:MemberStringMap("csargs"), false, true)
    quest:FixMovieSequenceCamera(false)
    resources:ClearStringMap(resources:MemberStringMap("csargs"))
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

return {helper_E68B20 = helper_E68B20}
