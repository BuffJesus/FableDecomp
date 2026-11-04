-- Readable native conversion: Artefact. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Artefact.Main (retail 0x00dcdc50)
function Main(quest, me)
    local predicateResult2 = false
    local banditTeamCrateDrop = quest:GetThingWithScriptName("BanditTeamCrateDrop")
    local function ReleaseEverything()
        banditTeamCrateDrop = nil
    end
    local scratchValue = quest:IsActiveThreadTerminating()
    repeat
        if scratchValue then
            banditTeamCrateDrop = nil
            return
        end
        if quest:IsDistanceBetweenThingsUnder(me, banditTeamCrateDrop, 3.0) then
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            scratchValue = me:IsBeingCarriedBy("SCRIPT_NAME_HERO")
            local predicateResult = quest:IsActiveThreadTerminating()
            if not scratchValue then
                if predicateResult then return end
                quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") - 1)
                quest:SetMasterGameState("OFBRCratesStolen", true)
                quest:RemoveThing(me, false, true)
                return
            end
            if predicateResult then
                return
            end
            if not predicateResult2 then
                if quest:IsActiveThreadTerminating() then return end
                predicateResult2 = true
            end
        else
            if quest:IsActiveThreadTerminating() then ReleaseEverything(); return end
            predicateResult2 = false
        end
        quest:NewScriptFrame(me)
        scratchValue = quest:IsActiveThreadTerminating()
    until false
end

-- Artefact.Init (retail 0x00dcfa10)
function Init(quest, me)
    quest:SetStateInt("CrateCount", quest:GetStateInt("CrateCount") + 1)
    quest:StateListPush("CrateList", me)
    quest:MiniMapAddMarker(me, "HUD_ORB_QUEST_CORE")
end

-- Artefact.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- Artefact.OnPredicateFail (retail 0x00dcf920)
function OnPredicateFail(quest, me)
    local p0 = 0
    if 0 == quest:GetStateListCount("CrateList") then return end
    while true do
        if quest:GetStateListAt("CrateList", p0):IsEqualTo(me) then break end
        p0 = p0 + 1
        if p0 == quest:GetStateListCount("CrateList") then
            return
        end
    end
    quest:StateListErase("CrateList", p0)
end

