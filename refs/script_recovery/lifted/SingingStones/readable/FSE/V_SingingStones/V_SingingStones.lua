-- Readable native conversion: V_SingingStones. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_SingingStones.Main (retail 0x00ed10a0)
function Main(quest)
    local flags, scratchValue
    local isRegionLoaded = quest:IsRegionLoaded("Witchwood2")
    flags = 0
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:AddEntityBinding("SingingStone", "V_SingingStones/Entities/SingingStone")
            quest:AddEntityBinding("ManWithDoorName", "V_SingingStones/Entities/ManWithDoorName")
            quest:FinalizeEntityBindings()
            quest:CreateThread("WatchForCompleteTune")  -- native thread body 0x00ED2BB0: lift it as function WatchForCompleteTune(quest)
            if (flags & 4) ~= 0 then
                flags = flags & 0xfffffffb
            end
            quest:CreateThread("WatchForSpokenToDemonDoors")  -- native thread body 0x00ED13B0: lift it as function WatchForSpokenToDemonDoors(quest)
            if (flags & 8) ~= 0 then
                scratchValue = flags & 0xfffffff7
            end
            quest:CreateThread("WatchForRegionLeaving")  -- native thread body CQ_BanditCampScript::WatchForEndOfScript: lift it as function WatchForRegionLeaving(quest)
            return
        end
        if not quest:NewScriptFrame() then break end
        isRegionLoaded = quest:IsRegionLoaded("Witchwood2")
        flags = 0
    end
end

-- V_SingingStones.Init (retail 0x00ed0ec0)
function Init(quest)
    quest:SetStateInt("PlayList_1", 1)
    quest:SetStateInt("RudePlayList_2", 1)
    quest:SetStateInt("CurrentPlayListIndex", 0)
    quest:SetStateInt("PlayList_0", 3)
    quest:SetStateInt("PlayList_2", 0)
    quest:SetStateInt("PlayList_3", 2)
    quest:SetStateInt("RudePlayList_0", 2)
    quest:SetStateInt("RudePlayList_1", 3)
    quest:SetStateInt("RudePlayList_3", 0)
    quest:SetStateString("ListText_" .. 0, "A")
    quest:SetStateString("ListText_" .. 1, "B")
    quest:SetStateString("ListText_" .. 2, "C")
    quest:SetStateString("ListText_" .. 3, "D")
    quest:SetStateBool("DoorManIntroComplete", false)
    quest:SetStateBool("DoorManAttackedByHero", false)
    quest:SetStateBool("DoorManHasBribe", false)
    quest:SetStateBool("DoorManComplete", false)
end

-- V_SingingStones.OnPersist (retail 0x00ed1010)
function OnPersist(quest, context)
    local doorManIntroComplete = quest:GetStateBool("DoorManIntroComplete") or false
    doorManIntroComplete = quest:PersistTransferBool(context, "DoorManIntroComplete", doorManIntroComplete)
    quest:SetStateBool("DoorManIntroComplete", doorManIntroComplete)
    local doorManAttackedByHero = quest:GetStateBool("DoorManAttackedByHero") or false
    doorManAttackedByHero = quest:PersistTransferBool(context, "DoorManAttackedByHero", doorManAttackedByHero)
    quest:SetStateBool("DoorManAttackedByHero", doorManAttackedByHero)
    local doorManHasBribe = quest:GetStateBool("DoorManHasBribe") or false
    doorManHasBribe = quest:PersistTransferBool(context, "DoorManHasBribe", doorManHasBribe)
    quest:SetStateBool("DoorManHasBribe", doorManHasBribe)
    local doorManComplete = quest:GetStateBool("DoorManComplete") or false
    doorManComplete = quest:PersistTransferBool(context, "DoorManComplete", doorManComplete)
    quest:SetStateBool("DoorManComplete", doorManComplete)
end

local function RemoveLiveEffects(quest)
    local effects = quest:GetStateListCopy("EffectList")
    for index = 1, #effects do
        local effect = effects[index]
        if effect:IsAlive() then
            quest:RemoveThing(effect, false, true)
        end
    end
end

-- V_SingingStones.WatchForCompleteTune (retail 0x00ed2bb0)
function WatchForCompleteTune(quest)
    local resources = quest:RetailResources()
    local getMasterGameState, scratchValue, scratchValue2, scratchValue3, scratchValue4
    local scratchValue5, scratchValue6, p0, speakMarker2, scratchValue8
    scratchValue3 = 0
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if quest:GetStateInt("CurrentPlayListIndex") == 4 then
            if quest:IsActiveThreadTerminating() then return end
            scratchValue2 = 1
            scratchValue = 1
            scratchValue4 = 0
            repeat
                if quest:GetStateInt("PlayList_" .. scratchValue4) ~= quest:GetStateInt("YourPersonalPlayList_" .. scratchValue4) then
                    if quest:IsActiveThreadTerminating() then return end
                    scratchValue2 = 0
                end
                if quest:GetStateInt("RudePlayList_" .. scratchValue4) == quest:GetStateInt("YourPersonalPlayList_" .. scratchValue4) then
                    scratchValue4 = scratchValue4 + 1
                else
                    if quest:IsActiveThreadTerminating() then return end
                    scratchValue = 0
                    scratchValue4 = scratchValue4 + 1
                end
            until not (scratchValue4 < 4)
            if scratchValue2 ~= 0 then
                if quest:IsActiveThreadTerminating() then return end
                local hero = quest:GetHero()
                local speakMarker = quest:GetNearestWithScriptName(hero, "SpeakMarker")
                local conversationID = quest:AddNewConversation(speakMarker, true, true)
                local hero2 = quest:GetHero()
                quest:AddPersonToConversation(conversationID, hero2)
                local hero3 = quest:GetHero()
                quest:AddLineToConversation(conversationID, "TEXT_QST_060_NAME_DBAC", speakMarker, hero3, false)
                quest:SetMasterGameState("SingingStonesInSync", true)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                getMasterGameState = quest:GetMasterGameState("SingingStonesInSync")
                break
            end
            if scratchValue == 0 or scratchValue3 ~= 0 then
                if quest:IsActiveThreadTerminating() then return end
                local tuneName = "TEXT_QST_060_NAME_"
                scratchValue5 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then return end
                    local switch = quest:GetStateInt("YourPersonalPlayList_" .. scratchValue5)
                    repeat
                        if switch == 0 then
                            p0 = "A"
                            break
                        elseif switch == 1 then
                            p0 = "B"
                            break
                        elseif switch == 2 then
                            p0 = "C"
                            break
                        elseif switch == 3 then
                            p0 = "D"
                            break
                        else
                            goto FLOW_native_label_1
                        end
                    until true
                    tuneName = tuneName .. p0
                    ::FLOW_native_label_1::
                    scratchValue5 = scratchValue5 + 1
                until not (scratchValue5 < 4)
                if quest:IsActiveThreadTerminating() then return end
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:Pause(1.0)
                quest:CameraUseCameraPoint("CAM_Stones", nil, nil, -1.0, 0, -1)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                local hero4 = quest:GetHero()
                speakMarker2 = quest:GetNearestWithScriptName(hero4, "SpeakMarker")
                local conversationId = quest:AddNewConversation(speakMarker2, true, true)
                local hero5 = quest:GetHero()
                quest:AddPersonToConversation(conversationId, hero5)
                local hero6 = quest:GetHero()
                quest:AddLineToConversation(conversationId, tuneName, speakMarker2, hero6, false)
                quest:Pause(2.0)
                quest:CameraDefault()
                RemoveLiveEffects(quest)
                quest:StateListClear("EffectList")
                speakMarker2 = nil
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            else
                if quest:IsActiveThreadTerminating() then return end
                scratchValue3 = 1
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:Pause(1.0)
                quest:CameraUseCameraPoint("CAM_Stones", nil, nil, -1.0, 0, -1)
                local hero7 = quest:GetHero()
                local speakMarker3 = quest:GetNearestWithScriptName(hero7, "SpeakMarker")
                local conversationId2 = quest:AddNewConversation(speakMarker3, true, true)
                local hero8 = quest:GetHero()
                quest:AddPersonToConversation(conversationId2, hero8)
                local hero9 = quest:GetHero()
                quest:AddLineToConversation(conversationId2, "TEXT_QST_060_NAME_CDBA", speakMarker3, hero9, false)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                local stonesSpawnEnemy = quest:GetAllThingsWithScriptName("M_StonesSpawnEnemy")
                scratchValue8 = 0
                if #stonesSpawnEnemy ~= 0 then
                    scratchValue6 = 0
                    repeat
                        local pPosition = stonesSpawnEnemy[scratchValue6 + 1]:GetPos()
                        local scratchValue7 = quest:CreateCreature("", pPosition, "CREATURE_BALVERINE_01", false)
                        local hero10 = quest:GetHero()
                        quest:GiveThingBestEnemyTarget(scratchValue7, hero10)
                        scratchValue6 = scratchValue6 + 1
                        scratchValue8 = scratchValue8 + 1
                    until not (scratchValue8 < (#stonesSpawnEnemy))
                end
                quest:Pause(2.0)
                quest:CameraDefault()
                RemoveLiveEffects(quest)
                quest:StateListClear("EffectList")
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
            end
        end
        if not quest:NewScriptFrame() then return end
    until false
    ::LAB_00ed3294::
    if not getMasterGameState then
        if quest:IsActiveThreadTerminating() then return end
        RemoveLiveEffects(quest)
        quest:StateListClear("EffectList")
        quest:DeactivateQuestLater("V_SingingStones", 0)
        quest:DeactivateQuestLater("V_SingingStones_Activate", 0)
        return
    end
    if not quest:NewScriptFrame() then return end
    getMasterGameState = quest:GetMasterGameState("SingingStonesInSync")
    goto LAB_00ed3294
end

-- V_SingingStones.WatchForSpokenToDemonDoors (retail 0x00ed13b0)
function WatchForSpokenToDemonDoors(quest)
    local getMasterGameState = quest:GetMasterGameState("TrophyDealerHeroSpokenToDemonDoors")
    while true do
        if getMasterGameState then
            if quest:IsActiveThreadTerminating() then return end
            quest:ActivateQuest("V_SingingStones_Activate")
            return
        end
        if not quest:NewScriptFrame() then break end
        getMasterGameState = quest:GetMasterGameState("TrophyDealerHeroSpokenToDemonDoors")
    end
end

-- V_SingingStones.WatchForRegionLeaving (retail 0x00ed2a50)
function WatchForRegionLeaving(quest)
    local isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    while true do
        if isActiveThreadTerminating then
            return
        end
        while not quest:IsRegionLoaded("Witchwood2") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        while quest:IsRegionLoaded("Witchwood2") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
        RemoveLiveEffects(quest)
        quest:StateListClear("EffectList")
        quest:NewScriptFrame()
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    end
end

