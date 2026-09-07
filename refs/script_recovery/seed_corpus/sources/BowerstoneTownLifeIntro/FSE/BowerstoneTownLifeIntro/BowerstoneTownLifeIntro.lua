Quest = nil

local QUEST_NAME = "BowerstoneTownLifeIntro"

function Init(questObject)
    Quest = questObject
    Quest:Log("BowerstoneTownLifeIntro: Init phase started.")
    Quest:SetStateBool("ArenaFinished", false)
    Quest:SetStateBool("DoneGuardSpeech", false)
    Quest:SetStateBool("PoshGuardComplete", false)
end

function OnPersist(questObject, context)
    Quest = questObject
    local arenaFinished = Quest:GetStateBool("ArenaFinished") or false
    local doneGuardSpeech = Quest:GetStateBool("DoneGuardSpeech") or false
    local poshGuardComplete = Quest:GetStateBool("PoshGuardComplete") or false

    arenaFinished = Quest:PersistTransferBool(context, "ArenaFinished", arenaFinished)
    doneGuardSpeech = Quest:PersistTransferBool(context, "DoneGuardSpeech", doneGuardSpeech)
    poshGuardComplete = Quest:PersistTransferBool(context, "PoshGuardComplete", poshGuardComplete)

    Quest:SetStateBool("ArenaFinished", arenaFinished)
    Quest:SetStateBool("DoneGuardSpeech", doneGuardSpeech)
    Quest:SetStateBool("PoshGuardComplete", poshGuardComplete)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("BowerstoneTownLifeIntro: Main() started. Setting up entity bindings...")

    Quest:AddEntityBinding("BowerstoneIntroGuard", "BowerstoneTownLifeIntro/Entities/BowerstoneIntroGuard")
    Quest:AddEntityBinding("BowerstoneIntroGuardToPosh", "BowerstoneTownLifeIntro/Entities/BowerstoneIntroGuardToPosh")
    Quest:FinalizeEntityBindings()

    local regionExit = Quest:GetThingWithScriptName("BowerstoneIntroRegionExit")
    if regionExit and regionExit:IsAlive() then
        Quest:SetRegionExitAsActive(regionExit, false)
    end

    -- Phase 1: Wait until guard intro speech is completed
    while not Quest:GetStateBool("DoneGuardSpeech") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    -- Remove BowerstoneIntroGuard, wait a frame, and fade screen in
    local guard = Quest:GetThingWithScriptName("BowerstoneIntroGuard")
    if guard and guard:IsAlive() then
        Quest:RemoveThing(guard)
        if not Quest:NewScriptFrame() then
            return
        end
    end
    Quest:FadeScreenIn()

    -- Phase 2: Wait until ArenaFinished and PoshGuardComplete
    while not Quest:GetStateBool("ArenaFinished") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    while not Quest:GetStateBool("PoshGuardComplete") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    -- Remove BowerstoneIntroGuardToPosh, wait a frame, fade screen in, and reactivate region exit
    local poshGuard = Quest:GetThingWithScriptName("BowerstoneIntroGuardToPosh")
    if poshGuard and poshGuard:IsAlive() then
        Quest:RemoveThing(poshGuard)
        if not Quest:NewScriptFrame() then
            return
        end
    end
    Quest:FadeScreenIn()

    if not regionExit or not regionExit:IsAlive() then
        regionExit = Quest:GetThingWithScriptName("BowerstoneIntroRegionExit")
    end
    if regionExit and regionExit:IsAlive() then
        Quest:SetRegionExitAsActive(regionExit, true)
    end

    -- Phase 3: Wait until BowerstoneSlums region is unloaded before deactivating
    while true do
        if Quest:GetStateBool("PoshGuardComplete") and Quest:GetStateBool("DoneGuardSpeech") then
            if not Quest:IsRegionLoaded("BowerstoneSlums") then
                break
            end
        end
        if not Quest:NewScriptFrame() then
            return
        end
    end

    local activeQuestName = Quest:GetActiveQuestName()
    Quest:DeactivateQuestLater(activeQuestName, 0)
    Quest:Log("BowerstoneTownLifeIntro: Deactivated.")
end
