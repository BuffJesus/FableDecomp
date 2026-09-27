-- Generated from the same native helper bodies as the quest draft.
local helper_E68B20
function helper_E68B20(quest, me, native_arg_strParam_1)
    local resources = quest:RetailResources()
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_1c = resources:NewActorMap()
    resources:SetActor(xStack_1c, "HERO", xStack_10)
    resources:SetActor(xStack_1c, "CHICKEN", resources:MemberResource("seh_Chicken"))
    resources:SetActor(xStack_1c, "ORGANISER", resources:MemberResource("seh_ChickenMaster"))
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(native_arg_strParam_1, xStack_1c, resources:MemberStringMap("csargs"), false, true)
    quest:FixMovieSequenceCamera(false)
    resources:ClearStringMap(resources:MemberStringMap("csargs"))
    resources:DestroyActorMap(xStack_1c)
    resources:ReleaseResource(xStack_10)
end

return {helper_E68B20 = helper_E68B20}
