-- Readable native conversion: V_TourGuide. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local function __native_all_dead(list)
    local down = 0
    for _, thing in ipairs(list) do
        if not thing:IsAlive() or thing:IsUnconscious() then down = down + 1 end
    end
    return down == #list
end

-- V_TourGuide.Main (retail 0x00ee47b0)
function Main(quest)
    quest:AddEntityBinding("TourGuideGuide", "V_TourGuide/Entities/TourGuideGuide")
    quest:AddEntityBinding("TourGuideFollower", "V_TourGuide/Entities/TourGuideFollower")
    quest:FinalizeEntityBindings()
    quest:CreateThread("WatchForGuideKilled")  -- native thread body NScript::CV_TourGuideScript::WatchForGuideKilled: lift it as function WatchForGuideKilled(quest)
    quest:CreateThread("WatchForClosingTime")  -- native thread body NScript::CV_TourGuideScript::WatchForClosingTime: lift it as function WatchForClosingTime(quest)
    local questFinished = quest:GetStateBool("QuestFinished")
    while true do
        if questFinished then
            if quest:IsActiveThreadTerminating() then return end
            quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
            return
        end
        if not quest:NewScriptFrame() then break end
        questFinished = quest:GetStateBool("QuestFinished")
    end
end

-- V_TourGuide.Init (retail 0x00ee42a0)
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
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_9_locTextOverheard"),"TEXT_QST_066_LOCATION_OVERHEARD_BALCONY");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_9_locTextRequested"),"TEXT_QST_066_LOCATION_REQUESTED_BALCONY");
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 4),"M_TG_LocationBanquet");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_4_locTextOverheard"),"TEXT_QST_066_LOCATION_OVERHEARD_BANQUET");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_4_locTextRequested"),"TEXT_QST_066_LOCATION_REQUESTED_BANQUET");
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 7),"M_TG_LocationBar");
    quest:SetStateString("WaypointInfo_7_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_BAR")
    quest:SetStateString("WaypointInfo_7_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_BAR")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 17),"M_TG_LocationBridge");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_17_locTextOverheard"),"TEXT_QST_066_LOCATION_OVERHEARD_BRIDGE");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_17_locTextRequested"),"TEXT_QST_066_LOCATION_REQUESTED_BRIDGE");
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 14),"M_TG_LocationCloisters");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_14_locTextOverheard"),"TEXT_QST_066_LOCATION_OVERHEARD_CLOISTERS");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_14_locTextRequested"),"TEXT_QST_066_LOCATION_REQUESTED_CLOISTERS");
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 10),"M_TG_LocationDorm1");
    quest:SetStateString("WaypointInfo_10_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_DORM1")
    quest:SetStateString("WaypointInfo_10_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_DORM1")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 11),"M_TG_LocationDorm2");
    quest:SetStateString("WaypointInfo_11_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_DORM2")
    quest:SetStateString("WaypointInfo_11_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_DORM2")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 6),"M_TG_LocationLibrary");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_6_locTextOverheard"),"TEXT_QST_066_LOCATION_OVERHEARD_LIBRARY");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_6_locTextRequested"),"TEXT_QST_066_LOCATION_REQUESTED_LIBRARY");
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
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_1_locTextOverheard"),"TEXT_QST_066_LOCATION_OVERHEARD_TRAINING");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_1_locTextRequested"),"TEXT_QST_066_LOCATION_REQUESTED_TRAINING");
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 2),"M_TG_LocationServant");
    quest:SetStateString("WaypointInfo_2_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_SERVANT")
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_2_locTextRequested"),"TEXT_QST_066_LOCATION_REQUESTED_SERVANT");
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 12),"M_TG_LocationStairsInside");
    quest:SetStateString("WaypointInfo_12_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_STAIR")
    quest:SetStateString("WaypointInfo_12_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_STAIR")
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 8),"M_TG_LocationStairsOutside");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_8_locTextOverheard"),"TEXT_QST_066_LOCATION_OVERHEARD_STAIRCASE");
    -- TODO(native): CCharString::operator= (quest:GetStateString("WaypointInfo_8_locTextRequested"),"TEXT_QST_066_LOCATION_REQUESTED_STAIRCASE");
    -- TODO(native): CCharString::operator=((CCharString *)__element("WaypointInfo", 3),"M_TG_LocationStatue");
    quest:SetStateString("WaypointInfo_3_locTextOverheard", "TEXT_QST_066_LOCATION_OVERHEARD_STATUE")
    quest:SetStateString("WaypointInfo_3_locTextRequested", "TEXT_QST_066_LOCATION_REQUESTED_STATUE")
    -- TODO(native): CCharString::operator=((CCharString *)__element("RandomGuideResponse", 0),"TEXT_QST_066_RANDOM_GUIDE_RESPONSE_0");
    -- TODO(native): CCharString::operator=((CCharString *)__element("RandomGuideResponse", 1),"TEXT_QST_066_RANDOM_GUIDE_RESPONSE_1");
    -- TODO(native): CCharString::operator=((CCharString *)__element("RandomGuideResponse", 2),"TEXT_QST_066_RANDOM_GUIDE_RESPONSE_2");
    -- TODO(native): CCharString::operator=((CCharString *)__element("RandomGuideResponse", 3),"TEXT_QST_066_RANDOM_GUIDE_RESPONSE_3");
    -- TODO(native): CCharString::operator=((CCharString *)__element("RandomGuideResponse", 4),"TEXT_QST_066_RANDOM_GUIDE_RESPONSE_4");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseM", 0),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_10");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseM", 1),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_20");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseM", 2),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_30");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseM", 3),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_40");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseM", 4),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_MALE_50");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseF", 0),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_10");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseF", 1),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_20");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseF", 2),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_30");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseF", 3),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_40");
    -- TODO(native): CCharString::operator= ((CCharString *)__element("RandomFollowerResponseF", 4),"TEXT_QST_066_RANDOM_FOLLOWER_RESPONSE_FEMALE_50");
end

-- V_TourGuide.OnPersist (retail 0x00ee5720)
function OnPersist(quest, context)
    quest:SetStateBool("TourGuideKilled", quest:PersistTransferBool(context, "TourGuideKilled", quest:GetStateBool("TourGuideKilled")))
    quest:SetStateInt("WaypointCounter", quest:PersistTransferInt(context, "WaypointCounter", quest:GetStateInt("WaypointCounter") or 0))
    quest:SetStateBool("TourFinished", quest:PersistTransferBool(context, "TourFinished", quest:GetStateBool("TourFinished")))
    quest:SetStateBool("QuestActivated", quest:PersistTransferBool(context, "QuestActivated", quest:GetStateBool("QuestActivated")))
end

-- V_TourGuide.WatchForGuideKilled (retail 0x00ee4a70)
function WatchForGuideKilled(quest)
    local tourGuideGuide = quest:GetThingWithScriptName("TourGuideGuide")
    local scratchValue
    while true do
        local predicateResult = not (tourGuideGuide ~= nil and not tourGuideGuide:IsNull()) or not (tourGuideGuide ~= nil and tourGuideGuide:IsAlive())
        if not predicateResult then break end
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    while true do
        if not (tourGuideGuide ~= nil and not tourGuideGuide:IsNull()) then
            scratchValue = 0
        else
            scratchValue = tourGuideGuide ~= nil and tourGuideGuide:MsgIsKilledBy("")
        end
        if scratchValue then break end
        if not quest:NewScriptFrame() then return end
    end
    quest:SetStateBool("TourGuideKilled", true)
end

-- V_TourGuide.WatchForClosingTime (retail 0x00ee4a00)
function WatchForClosingTime(quest)
    repeat
        if quest:IsActiveThreadTerminating() then return end
        local getTimeOfDay = quest:GetTimeOfDay()
        if getTimeOfDay < quest:ReadGlobalGameData(2284) then
            goto LAB_00ee4a39
        else
            if not quest:GetStateBool("TourFinished") then goto LAB_00ee4a4b end
            if getTimeOfDay <= quest:ReadGlobalGameData(2284) then goto LAB_00ee4a39 end
        end
        goto FLOW_past_lab_00ee4a39
        ::LAB_00ee4a39::
        if quest:ReadGlobalGameData(2280) < getTimeOfDay and quest:GetStateBool("TourFinished") then
            goto LAB_00ee4a4b
        end
        ::FLOW_past_lab_00ee4a39::
        goto FLOW_past_lab_00ee4a4b
        ::LAB_00ee4a4b::
        ::FLOW_past_lab_00ee4a4b::
        quest:NewScriptFrame()
    until false
end

-- V_TourGuide.null (retail 0x00ee6a40)
function null(quest)
    local predicateResult
    local tourGuideFollower = quest:GetAllThingsWithScriptName("TourGuideFollower")
    predicateResult = quest:IsActiveThreadTerminating()
    repeat
        if predicateResult then
            return
        end
        -- TODO(native): MsgIsRegionUnloaded is not a ForgeFSE binding
        quest:MsgIsRegionUnloaded("HeroGuildComplexInside")
        if predicateResult then
            if quest:IsActiveThreadTerminating() then return end
            goto LAB_00ee6b8f
        end
        goto FLOW_past_lab_00ee6b8f
        ::LAB_00ee6b8f::
        do return end
        ::FLOW_past_lab_00ee6b8f::
        if __native_all_dead(tourGuideFollower) and not quest:GetStateBool("TourFinished") then
            if quest:IsActiveThreadTerminating() then return end
            quest:SetStateBool("AllFollowersDead", true)
            goto LAB_00ee6b8f
        end
        quest:NewScriptFrame()
        predicateResult = quest:IsActiveThreadTerminating()
    until false
end

