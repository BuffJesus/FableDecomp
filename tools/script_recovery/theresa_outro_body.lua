-- Native DBB0E4..DBB2D8. Main still owns presented text and Theresa control.
local function finishTheresaChildhood(quest, me, resources, theresaControl)
    if quest:IsActiveThreadTerminating() then return false end
    quest:DisplayQuestInfo(false)
    quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBullyHealthCounter"))
    quest:RemoveQuestInfoElement(quest:GetStateInt("GUIGoodDeedCounter"))
    quest:RemoveQuestInfoElement(quest:GetStateInt("GUIBarrelCounter"))
    local movie = resources:StartMovie("")
    local heroControl, actors
    local ok, failure = pcall(function()
        resources:Pause(true)
        heroControl = resources:NewResource()
        -- Native performs one attempt and proceeds even if acquisition fails.
        resources:TryAcquireTheresaHero(heroControl, 4)
        actors = newTheresaCutsceneActors(resources, heroControl, theresaControl, "Theresa")
        resources:RunMacro("CS_OAKVALE_INTRO_THERESA", actors, false, true)
        quest:PlayAVIMovie("Data\\Video\\1_raid_on_oak_vale_comp.xmv")
        quest:FadeScreenOut(0.5, 0.0)
        quest:OverrideMusic(25, false, false)
        quest:SetStateBool("AttackOver", true)
    end)
    local cleanupError
    local function cleanup(method, ...)
        local closed, err = pcall(method, resources, ...)
        if not closed and cleanupError == nil then cleanupError = err end
    end
    if actors then cleanup(resources.DestroyActorMap, actors) end
    if heroControl then cleanup(resources.ReleaseResource, heroControl) end
    cleanup(resources.Pause, false)
    cleanup(resources.DestroyMovie, movie)
    if not ok then error(failure, 0) end
    if cleanupError ~= nil then error(cleanupError, 0) end
    return true
end

return finishTheresaChildhood
