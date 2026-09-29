-- Generated from the same native helper bodies as the quest draft.
local helper_ECE460
function helper_ECE460(quest, me, native_arg_macroName, native_arg_argument)
    local resources = quest:RetailResources()
    local pOther
    local xStack_1c = resources:NewStringMap()
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local piVar2 = ("NULL" or "")
    local piVar5 = (native_arg_argument or "")
    local iVar3 = ((string.lower(piVar5) == string.lower(piVar2)) and 0 or 1)
    if iVar3 ~= 0 then
        pOther = (native_arg_argument or "")
        resources:SetString(xStack_1c, "$ARG1", pOther)
    end
    local xStack_28 = resources:NewActorMap()
    resources:SetActor(xStack_28, "WITCH", resources:MemberResource("seh_Witch"))
    resources:SetActor(xStack_28, "MUM", resources:MemberResource("seh_Mother"))
    resources:SetActor(xStack_28, "HERO", xStack_10)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(native_arg_macroName, xStack_28, xStack_1c, false, true)
    quest:FixMovieSequenceCamera(false)
    resources:DestroyActorMap(xStack_28)
    resources:ReleaseResource(xStack_10)
    resources:DestroyStringMap(xStack_1c)
end

return {helper_ECE460 = helper_ECE460}
