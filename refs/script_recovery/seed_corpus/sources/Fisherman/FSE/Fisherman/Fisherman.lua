Quest = nil

local QUEST_NAME = "Fisherman"

function Init(questObject)
    Quest = questObject
    Quest:Log("Fisherman: Init phase started.")

    Quest:SetStateBool("IntroPlayed", false)
    Quest:SetStateBool("HeroHasSavedFishermanFromWasps", false)
    Quest:SetStateInt("WaspsAlive", 0)
    Quest:SetStateBool("QuestCardGiven", false)
    Quest:SetStateInt("FishermanTasks", 0)
    Quest:SetStateBool("FishermanMustRunAway", false)
    Quest:SetStateInt("Attitude", 0)
end

function OnPersist(questObject, context)
    Quest = questObject

    local tasks = Quest:GetStateInt("FishermanTasks") or 0
    tasks = Quest:PersistTransferInt(context, "FishermanTasks", tasks)
    Quest:SetStateInt("FishermanTasks", tasks)

    local saved = Quest:GetStateBool("HeroHasSavedFishermanFromWasps") or false
    saved = Quest:PersistTransferBool(context, "HeroHasSavedFishermanFromWasps", saved)
    Quest:SetStateBool("HeroHasSavedFishermanFromWasps", saved)
end

function Main(questObject)
    Quest = questObject
    Quest:Log("Fisherman: Main() started.")

    -- Entity bindings
    Quest:AddEntityBinding("Fisherman", "Fisherman/Entities/Fisherman")
    Quest:AddEntityBinding("M_FishermanWaspSpawn", "Fisherman/Entities/M_FishermanWaspSpawn")
    Quest:AddEntityBinding("FishermanBulletinBoard", "Fisherman/Entities/FishermanBulletinBoard")
    Quest:AddEntityBinding("FishermanWasp", "Fisherman/Entities/FishermanWasp")
    Quest:FinalizeEntityBindings()

    -- Wait until the hero saves the fisherman from the wasps
    while not Quest:GetStateBool("HeroHasSavedFishermanFromWasps") do
        if not Quest:NewScriptFrame() then
            return
        end
    end

    Quest:Log("Fisherman: Hero saved fisherman. Waiting for player to leave 'Fisherman' region...")

    -- If Fisherman region is loaded, wait until it is unloaded (player leaves)
    if Quest:IsRegionLoaded("Fisherman") then
        while Quest:IsRegionLoaded("Fisherman") do
            if not Quest:NewScriptFrame() then
                return
            end
        end
    end

    Quest:Log("Fisherman: Left Fisherman region. Transitioning to V_FishingCompetition.")

    -- Activate the follow-up fishing competition quest
    Quest:ActivateQuest("V_FishingCompetition")

    -- Deactivate this quest
    local activeQuestName = Quest:GetActiveQuestName()
    Quest:DeactivateQuestLater(activeQuestName, 0)
    Quest:Log("Fisherman: Main() terminated.")
end
