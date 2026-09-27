-- Readable native conversion: V_SingingStones. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_SingingStones.Main (retail 0x00ed10a0)
function Main(quest)
    local flags
    local isRegionLoaded = quest:IsRegionLoaded("Witchwood2")
    flags = 0
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:AddEntityBinding("SingingStone", "V_SingingStones/Entities/SingingStone")
            quest:AddEntityBinding("ManWithDoorName", "V_SingingStones/Entities/ManWithDoorName")
            quest:FinalizeEntityBindings()
            quest:CreateThread("WatchForCompleteTune")  -- native thread body 0x00ED2BB0: lift it as function WatchForCompleteTune(quest)
            if flags & 4 ~= 0 then
                flags = flags & 0xfffffffb
            end
            quest:CreateThread("WatchForSpokenToDemonDoors")  -- native thread body 0x00ED13B0: lift it as function WatchForSpokenToDemonDoors(quest)
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
    -- TODO(native): CCharString::operator=((CCharString *)__element("ListText", 0),"A");
    -- TODO(native): CCharString::operator=((CCharString *)__element("ListText", 1),"B");
    -- TODO(native): CCharString::operator=((CCharString *)__element("ListText", 2),"C");
    -- TODO(native): CCharString::operator=((CCharString *)__element("ListText", 3),"D");
    quest:SetStateBool("DoorManIntroComplete", false)
    quest:SetStateBool("DoorManAttackedByHero", false)
    quest:SetStateBool("DoorManHasBribe", false)
    quest:SetStateBool("DoorManComplete", false)
end

-- V_SingingStones.OnPersist (retail 0x00ed1010)
function OnPersist(quest, context)
    quest:SetStateBool("DoorManIntroComplete", quest:PersistTransferBool(context, "DoorManIntroComplete", quest:GetStateBool("DoorManIntroComplete")))
    quest:SetStateBool("DoorManAttackedByHero", quest:PersistTransferBool(context, "DoorManAttackedByHero", quest:GetStateBool("DoorManAttackedByHero")))
    quest:SetStateBool("DoorManHasBribe", quest:PersistTransferBool(context, "DoorManHasBribe", quest:GetStateBool("DoorManHasBribe")))
    quest:SetStateBool("DoorManComplete", quest:PersistTransferBool(context, "DoorManComplete", quest:GetStateBool("DoorManComplete")))
end

-- V_SingingStones.WatchForCompleteTune (retail 0x00ed2bb0)
function WatchForCompleteTune(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local getMasterGameState, scratchValue, scratchValue3, scratchValue4, scratchValue5
    local scratchValue6, p0, scratchValue10
    if quest:IsActiveThreadTerminating() then return end
    repeat
        if quest:GetStateInt("CurrentPlayListIndex") == 4 then
            scratchValue4 = 0
            repeat
                -- TODO(native): if piVar6[-8] ~= *piVar6 then
                -- TODO(native): if piVar6[-4] ~= *piVar6 then
                scratchValue4 = scratchValue4 + 1; goto continue_1
                if quest:IsActiveThreadTerminating() then return end
                scratchValue4 = scratchValue4 + 1
                ::continue_1::
            until scratchValue4 >= 4
            if quest:IsActiveThreadTerminating() then return end
            local speakMarker = quest:GetNearestWithScriptName(hero, "SpeakMarker")
            local conversationID = quest:AddNewConversation(speakMarker, true, true)
            quest:AddPersonToConversation(conversationID, hero)
            quest:AddLineToConversation(conversationID, "TEXT_QST_060_NAME_DBAC", speakMarker, hero, false)
            quest:SetMasterGameState("SingingStonesInSync", true)
            quest:SetStateInt("CurrentPlayListIndex", 0)
            getMasterGameState = quest:GetMasterGameState("SingingStonesInSync")
            break
            if scratchValue == 0 or scratchValue3 ~= 0 then
                if quest:IsActiveThreadTerminating() then return end
                scratchValue5 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then return end
                    -- TODO(native): native_arg_switch_2 = *puVar7
    --[[unresolved native value]]
                    repeat
                        if nil == 0 then
                            break
                        elseif nil == 1 then
                            break
                        elseif nil == 2 then
                            break
                        elseif nil == 3 then
                            break
                        else
                            break
                        end
                    until true
                    -- TODO(native): CCharString::operator+=(&xStack_80,(int)p0);
                    scratchValue5 = scratchValue5 + 1
                until scratchValue5 >= 4
                if quest:IsActiveThreadTerminating() then return end
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:Pause(1.0)
                quest:CameraUseCameraPoint(speakMarker, nil --[[missing]], 0, 0, -1.0)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                local speakMarker2 = quest:GetNearestWithScriptName(hero, "SpeakMarker")
                local conversationId = quest:AddNewConversation(speakMarker2, true, true)
                quest:AddPersonToConversation(conversationId, hero)
                quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_", speakMarker2, hero, false)
                quest:Pause(2.0)
                quest:CameraDefault()
                -- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);
                quest:StateListClear("EffectList")
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            else
                if quest:IsActiveThreadTerminating() then return end
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:Pause(1.0)
                quest:CameraUseCameraPoint(nil --[[missing]], nil --[[missing]], 0, 0, -1.0)
                local speakMarker3 = quest:GetNearestWithScriptName(hero, "SpeakMarker")
                local conversationId2 = quest:AddNewConversation(speakMarker3, true, true)
                quest:AddPersonToConversation(conversationId2, hero)
                quest:AddLineToConversation(conversationId2, "TEXT_QST_060_NAME_CDBA", speakMarker3, hero, false)
                quest:SetStateInt("CurrentPlayListIndex", 0)
                local stonesSpawnEnemy = quest:GetAllThingsWithScriptName("M_StonesSpawnEnemy")
                scratchValue10 = 0
                if #stonesSpawnEnemy ~= 0 then
                    scratchValue6 = 0
                    repeat
                        quest:GiveThingBestEnemyTarget(quest:CreateCreature("", stonesSpawnEnemy[scratchValue6 + 1]:GetPos(), "CREATURE_BALVERINE_01"), hero)
                        scratchValue6 = scratchValue6 + 1
                        scratchValue10 = scratchValue10 + 1
                    until scratchValue10 >= #stonesSpawnEnemy
                end
                quest:Pause(2.0)
                quest:CameraDefault()
                -- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);
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
        -- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);
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
        -- TODO(native): KillAllThingsInVector(*(void **)(this + 0x40),quest:GetStateListRef("EffectList"),0);
        -- TODO(native): Std_Vector_Erase_Range(quest:GetStateListRef("EffectList"),quest:GetStateListRef("EffectList"),(quest:GetStateListCount("EffectList") * 0xc));
        quest:NewScriptFrame()
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    end
end

