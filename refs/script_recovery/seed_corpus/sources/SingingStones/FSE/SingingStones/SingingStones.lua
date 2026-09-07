-- SingingStones.lua
-- Main quest script for Singing Stones (V_SingingStones)
-- Functional parity port of NScript::CV_SingingStonesScript

local QUEST_NAME = "SingingStones"
local REGION_NAME = "Witchwood2"

local WINNING_PLAYLIST = { 3, 1, 0, 2 } -- D, B, A, C (spells "HITS")
local RUDE_PLAYLIST    = { 2, 3, 1, 0 } -- C, D, B, A

local LETTER_MAP = {
    [0] = "A",
    [1] = "B",
    [2] = "C",
    [3] = "D",
}

function Init(quest)
    quest:Log("SingingStones: Init() started.")

    quest:SetStateInt("CurrentPlayListIndex", 0)
    quest:SetStateBool("DoorManIntroComplete", false)
    quest:SetStateBool("DoorManAttackedByHero", false)
    quest:SetStateBool("DoorManHasBribe", false)
    quest:SetStateBool("DoorManComplete", false)
    quest:SetStateBool("ClearEffects", false)
    quest:SetStateBool("PuzzleWon", false)

    quest:Log("SingingStones: Init() completed.")
end

function OnPersist(quest, context)
    local introComplete = quest:GetStateBool("DoorManIntroComplete") or false
    introComplete = quest:PersistTransferBool(context, "DoorManIntroComplete", introComplete)
    quest:SetStateBool("DoorManIntroComplete", introComplete)

    local attacked = quest:GetStateBool("DoorManAttackedByHero") or false
    attacked = quest:PersistTransferBool(context, "DoorManAttackedByHero", attacked)
    quest:SetStateBool("DoorManAttackedByHero", attacked)

    local hasBribe = quest:GetStateBool("DoorManHasBribe") or false
    hasBribe = quest:PersistTransferBool(context, "DoorManHasBribe", hasBribe)
    quest:SetStateBool("DoorManHasBribe", hasBribe)

    local complete = quest:GetStateBool("DoorManComplete") or false
    complete = quest:PersistTransferBool(context, "DoorManComplete", complete)
    quest:SetStateBool("DoorManComplete", complete)
end

function WatchForCompleteTune(quest)
    quest:Log("SingingStones: WatchForCompleteTune thread started.")

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: WatchForCompleteTune - could not get Hero. Aborting.")
        return
    end

    local rudePlayed = false

    while true do
        local currentIndex = quest:GetStateInt("CurrentPlayListIndex") or 0

        if currentIndex == 4 then
            local winner = true
            local rude = true

            for i = 0, 3 do
                local played = quest:GetStateInt("PersonalPlayList_" .. i)
                if played ~= WINNING_PLAYLIST[i + 1] then
                    winner = false
                end
                if played ~= RUDE_PLAYLIST[i + 1] then
                    rude = false
                end
            end

            if winner then
                quest:Log("SingingStones: Winning tune entered (DBAC)!")

                local speakMarker = quest:GetNearestWithScriptName(hero, "SpeakMarker")
                if speakMarker then
                    local convoID = quest:StartAmbientConversation(speakMarker, hero, true, true)
                    if convoID and convoID ~= -1 then
                        quest:AddLineToConversation(convoID, "TEXT_QST_060_NAME_DBAC", speakMarker, hero, false)
                    end
                end

                quest:SetMasterGameState("SingingStonesInSync", true)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                quest:SetStateBool("PuzzleWon", true)

                while quest:GetMasterGameState("SingingStonesInSync") do
                    if not quest:NewScriptFrame() then return end
                end

                -- Demon door opened, puzzle resolved: cleanup effects and deactivate quest
                quest:SetStateBool("ClearEffects", true)
                local activeQuestName = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(activeQuestName, 0)
                quest:DeactivateQuestLater("V_SingingStones", 0)
                quest:DeactivateQuestLater("V_SingingStones_Activate", 0)
                quest:Log("SingingStones: Quest completed and deactivated.")
                return

            elseif not rude or rudePlayed then
                quest:Log("SingingStones: Incorrect tune entered!")

                local speechKey = "TEXT_QST_060_NAME_"
                for i = 0, 3 do
                    local played = quest:GetStateInt("PersonalPlayList_" .. i) or 0
                    speechKey = speechKey .. (LETTER_MAP[played] or "A")
                end

                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:Pause(1.0)
                quest:CameraUseCameraPoint("CAM_Stones", {}, {}, -1.0, 0, -1)
                quest:SetStateInt("CurrentPlayListIndex", 0)

                local speakMarker = quest:GetNearestWithScriptName(hero, "SpeakMarker")
                if speakMarker then
                    local convoID = quest:StartAmbientConversation(speakMarker, hero, true, true)
                    if convoID and convoID ~= -1 then
                        quest:AddLineToConversation(convoID, speechKey, speakMarker, hero, false)
                    end
                end

                quest:Pause(2.0)
                quest:CameraDefault()
                quest:SetStateBool("ClearEffects", true)
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()
                quest:SetStateBool("ClearEffects", false)

            else
                quest:Log("SingingStones: Rude tune entered for the first time (CDBA)!")
                rudePlayed = true

                quest:StartMovieSequence()
                quest:PauseAllNonScriptedEntities(true)
                quest:Pause(1.0)
                quest:CameraUseCameraPoint("CAM_Stones", {}, {}, -1.0, 0, -1)

                local speakMarker = quest:GetNearestWithScriptName(hero, "SpeakMarker")
                if speakMarker then
                    local convoID = quest:StartAmbientConversation(speakMarker, hero, true, true)
                    if convoID and convoID ~= -1 then
                        quest:AddLineToConversation(convoID, "TEXT_QST_060_NAME_CDBA", speakMarker, hero, false)
                    end
                end

                quest:SetStateInt("CurrentPlayListIndex", 0)

                local spawnPoints = quest:GetAllThingsWithScriptName("M_StonesSpawnEnemy")
                if spawnPoints then
                    for _, sp in ipairs(spawnPoints) do
                        local balverine = quest:CreateCreature("CREATURE_BALVERINE_01", sp:GetPos(), "")
                        if balverine then
                            quest:GiveThingBestEnemyTarget(balverine, hero)
                        end
                    end
                end

                quest:Pause(2.0)
                quest:CameraDefault()
                quest:SetStateBool("ClearEffects", true)
                quest:PauseAllNonScriptedEntities(false)
                quest:EndMovieSequence()
                quest:SetStateBool("ClearEffects", false)
            end
        end

        if not quest:NewScriptFrame() then
            return
        end
    end
end

function WatchForSpokenToDemonDoors(quest)
    quest:Log("SingingStones: WatchForSpokenToDemonDoors thread started.")

    while not quest:GetMasterGameState("TrophyDealerHeroSpokenToDemonDoors") do
        if not quest:NewScriptFrame() then
            return
        end
    end

    quest:Log("SingingStones: Hero has spoken to Demon Doors. Activating quest card...")
    quest:ActivateQuest("V_SingingStones_Activate")
end

function WatchForRegionLeaving(quest)
    quest:Log("SingingStones: WatchForRegionLeaving thread started.")

    while true do
        while not quest:IsRegionLoaded(REGION_NAME) do
            if not quest:NewScriptFrame() then return end
        end

        while quest:IsRegionLoaded(REGION_NAME) do
            if not quest:NewScriptFrame() then return end
        end

        -- Player left Witchwood2, signal active stone effects to clear
        quest:Log("SingingStones: Left Witchwood2 - clearing active stone effects.")
        quest:SetStateBool("ClearEffects", true)
        if not quest:NewScriptFrame() then return end
        quest:SetStateBool("ClearEffects", false)
    end
end

function Main(quest)
    quest:Log("SingingStones: Main() started.")

    while not quest:IsRegionLoaded(REGION_NAME) do
        if not quest:NewScriptFrame() then
            return
        end
    end

    quest:Log("SingingStones: Witchwood2 loaded. Binding entities...")
    quest:AddEntityBinding("SingingStone", "SingingStones/Entities/SingingStone")
    quest:AddEntityBinding("ManWithDoorName", "SingingStones/Entities/ManWithDoorName")
    quest:FinalizeEntityBindings()

    quest:CreateThread("WatchForCompleteTune")
    quest:CreateThread("WatchForSpokenToDemonDoors")
    quest:CreateThread("WatchForRegionLeaving")

    quest:Log("SingingStones: Background threads started.")
end
