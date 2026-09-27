-- Generated native draft: V_TourGuide. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local function __native_all_dead(list)
    local down = 0
    for _, thing in ipairs(list) do
        if (not thing:IsAlive()) or thing:IsUnconscious() then down = down + 1 end
    end
    return down == #list
end

function Main(quest)
    local bVar5, pQuestName
    local alive = true
    quest:AddEntityBinding("TourGuideGuide", "V_TourGuide/Entities/TourGuideGuide")
    quest:AddEntityBinding("TourGuideFollower", "V_TourGuide/Entities/TourGuideFollower")
    quest:FinalizeEntityBindings()
    quest:CreateThread("WatchForGuideKilled")  -- native thread body NScript::CV_TourGuideScript::WatchForGuideKilled: lift it as function WatchForGuideKilled(quest)
    if not bVar5 then
    end
    quest:CreateThread("WatchForClosingTime")  -- native thread body NScript::CV_TourGuideScript::WatchForClosingTime: lift it as function WatchForClosingTime(quest)
    if not bVar5 then
    end
    local cVar1 = quest:GetStateBool("QuestFinished")
    while true do
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if not bVar5 then
                pQuestName = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(pQuestName, 0)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then break end
        cVar1 = quest:GetStateBool("QuestFinished")
    end
end

function Init(quest)
    quest:SetStateBool("TourGuideKilled", false)
    quest:SetStateBool("AllFollowersDead", false)
    quest:SetStateBool("QuestFinished", false)
    quest:SetStateBool("QuestActivated", false)
    quest:SetStateBool("TourFinished", false)
    quest:SetStateBool("Initialise", true)
    quest:SetStateBool("SpawnedQuestFinishThread", false)
    quest:SetStateBool("GuideSpokenToHeroThisWaypoint", false)
    quest:SetStateBool("OverheardTourGuideThisWaypoint", false)
    quest:SetStateInt("WaypointCounter", 0)
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 9),"M_TG_LocationBalcony");
    quest:SetStateString("WaypointInfo_9_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_BALCONY")
    quest:SetStateString("WaypointInfo_9_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_BALCONY")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 4),"M_TG_LocationBanquet");
    quest:SetStateString("WaypointInfo_4_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_BANQUET")
    quest:SetStateString("WaypointInfo_4_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_BANQUET")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 7),"M_TG_LocationBar");
    quest:SetStateString("WaypointInfo_7_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_BAR")
    quest:SetStateString("WaypointInfo_7_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_BAR")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 17),"M_TG_LocationBridge");
    quest:SetStateString("WaypointInfo_17_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_BRIDGE")
    quest:SetStateString("WaypointInfo_17_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_BRIDGE")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 14),"M_TG_LocationCloisters");
    quest:SetStateString("WaypointInfo_14_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_CLOISTERS")
    quest:SetStateString("WaypointInfo_14_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_CLOISTERS")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 10),"M_TG_LocationDorm1");
    quest:SetStateString("WaypointInfo_10_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_DORM1")
    quest:SetStateString("WaypointInfo_10_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_DORM1")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 11),"M_TG_LocationDorm2");
    quest:SetStateString("WaypointInfo_11_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_DORM2")
    quest:SetStateString("WaypointInfo_11_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_DORM2")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 6),"M_TG_LocationLibrary");
    quest:SetStateString("WaypointInfo_6_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_LIBRARY")
    quest:SetStateString("WaypointInfo_6_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_LIBRARY")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 5),"M_TG_LocationMap");
    quest:SetStateString("WaypointInfo_5_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_MAP")
    quest:SetStateString("WaypointInfo_5_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_MAP")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 16),"M_TG_LocationMaze");
    quest:SetStateString("WaypointInfo_16_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_MAZE")
    quest:SetStateString("WaypointInfo_16_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_MAZE")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 13),"M_TG_LocationShop");
    quest:SetStateString("WaypointInfo_13_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_SHOP")
    quest:SetStateString("WaypointInfo_13_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_SHOP")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 0),"M_TG_LocationStart");
    quest:SetStateString("WaypointInfo_0_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_START")
    quest:SetStateString("WaypointInfo_0_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_START")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 15),"M_TG_LocationTomb");
    quest:SetStateString("WaypointInfo_15_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_TOMB")
    quest:SetStateString("WaypointInfo_15_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_TOMB")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 1),"M_TG_LocationTraining");
    quest:SetStateString("WaypointInfo_1_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_TRAINING")
    quest:SetStateString("WaypointInfo_1_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_TRAINING")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 2),"M_TG_LocationServant");
    quest:SetStateString("WaypointInfo_2_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_SERVANT")
    quest:SetStateString("WaypointInfo_2_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_SERVANT")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 12),"M_TG_LocationStairsInside");
    quest:SetStateString("WaypointInfo_12_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_STAIR")
    quest:SetStateString("WaypointInfo_12_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_STAIR")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 8),"M_TG_LocationStairsOutside");
    quest:SetStateString("WaypointInfo_8_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_STAIRCASE")
    quest:SetStateString("WaypointInfo_8_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_STAIRCASE")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 3),"M_TG_LocationStatue");
    quest:SetStateString("WaypointInfo_3_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_STATUE")
    quest:SetStateString("WaypointInfo_3_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_STATUE")
    quest:SetStateString(("RandomGuideResponse_" .. 0), "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_0")
    quest:SetStateString(("RandomGuideResponse_" .. 1), "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_1")
    quest:SetStateString(("RandomGuideResponse_" .. 2), "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_2")
    quest:SetStateString(("RandomGuideResponse_" .. 3), "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_3")
    quest:SetStateString(("RandomGuideResponse_" .. 4), "TEXT_QST_066_RANDOM_GUIDE_RESPONSE_4")
    quest:SetStateString(("RandomFollowerResponseM_" .. 0), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_10")
    quest:SetStateString(("RandomFollowerResponseM_" .. 1), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_20")
    quest:SetStateString(("RandomFollowerResponseM_" .. 2), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_30")
    quest:SetStateString(("RandomFollowerResponseM_" .. 3), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_40")
    quest:SetStateString(("RandomFollowerResponseM_" .. 4), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_50")
    quest:SetStateString(("RandomFollowerResponseF_" .. 0), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_10")
    quest:SetStateString(("RandomFollowerResponseF_" .. 1), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_20")
    quest:SetStateString(("RandomFollowerResponseF_" .. 2), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_30")
    quest:SetStateString(("RandomFollowerResponseF_" .. 3), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_40")
    quest:SetStateString(("RandomFollowerResponseF_" .. 4), "TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_50")
end

function OnPersist(quest, context)
    local tourGuideKilled = quest:GetStateBool("TourGuideKilled") or false
    tourGuideKilled = quest:PersistTransferBool(context, "TourGuideKilled", tourGuideKilled)
    quest:SetStateBool("TourGuideKilled", tourGuideKilled)
    local waypointCounter = quest:GetStateInt("WaypointCounter") or 0
    waypointCounter = quest:PersistTransferInt(context, "WaypointCounter", waypointCounter)
    quest:SetStateInt("WaypointCounter", waypointCounter)
    local tourFinished = quest:GetStateBool("TourFinished") or false
    tourFinished = quest:PersistTransferBool(context, "TourFinished", tourFinished)
    quest:SetStateBool("TourFinished", tourFinished)
    local questActivated = quest:GetStateBool("QuestActivated") or false
    questActivated = quest:PersistTransferBool(context, "QuestActivated", questActivated)
    quest:SetStateBool("QuestActivated", questActivated)
end

function WatchForGuideKilled(quest)
    local bVar2, cVar1
    local alive = true
    local r1 = quest:GetThingWithScriptName("TourGuideGuide")
    while true do
        local __native_condition_1 = not (r1 ~= nil and not r1:IsNull())
        if not __native_condition_1 then
            cVar1 = (r1 ~= nil and r1:IsAlive())
            __native_condition_1 = not cVar1
        end
        if not __native_condition_1 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            r1 = nil
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if bVar2 then
        return
    end
    while true do
        if not (r1 ~= nil and not r1:IsNull()) then
            cVar1 = false
        else
            cVar1 = (r1 ~= nil and r1:MsgIsKilledBy(""))
        end
        if cVar1 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    if not bVar2 then
        quest:SetStateBool("TourGuideKilled", true)
    end
end

function WatchForClosingTime(quest)
    local bVar1, iVar2
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    repeat
        if bVar1 then
            return
        end
        iVar2 = quest:GetTimeOfDay()
        if iVar2 < quest:ReadGlobalGameData(0x8ec) then
            goto LAB_00ee4a39
        else
            if not quest:GetStateBool("TourFinished") then goto LAB_00ee4a4b end
            if iVar2 <= quest:ReadGlobalGameData(0x8ec) then goto LAB_00ee4a39 end
        end
        goto FLOW_past_lab_00ee4a39
        ::LAB_00ee4a39::
        if (quest:ReadGlobalGameData(0x8e8) < iVar2) and (quest:GetStateBool("TourFinished")) then
            goto LAB_00ee4a4b
        end
        ::FLOW_past_lab_00ee4a39::
        goto FLOW_past_lab_00ee4a4b
        ::LAB_00ee4a4b::
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
        ::FLOW_past_lab_00ee4a4b::
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    until false
end

function null(quest)
    local bVar2, iVar3, xStack_c
    local alive = true
    xStack_c = quest:GetAllThingsWithScriptName("TourGuideFollower")
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    repeat
        if bVar2 then
            -- LAB_00ee6be5: (native jump target)
            return
        end
        -- TODO(native): MsgIsRegionUnloaded is not a ForgeFSE binding
        quest:MsgIsRegionUnloaded("HeroGuildComplexInside")
        if bVar2 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            goto LAB_00ee6b8f
        end
        goto FLOW_past_lab_00ee6b8f
        ::LAB_00ee6b8f::
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
        else
        end
        do return end
        ::FLOW_past_lab_00ee6b8f::
        iVar3 = __native_all_dead(xStack_c)
        if (iVar3) and (not quest:GetStateBool("TourFinished")) then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            quest:SetStateBool("AllFollowersDead", true)
            goto LAB_00ee6b8f
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    until false
end

