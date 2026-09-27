-- Readable native conversion: V_ChickenKicking. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_ChickenKicking.Main (retail 0x00e62a00)
function Main(quest)
    local isRegionLoaded = quest:IsRegionLoaded("OakBay")
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            quest:AddEntityBinding("ChickenMaster", "V_ChickenKicking/Entities/ChickenMaster", 1)
            quest:AddEntityBinding("ChickenSign", "V_ChickenKicking/Entities/ChickenSign", 1)
            quest:AddEntityBinding("KickedChicken", "V_ChickenKicking/Entities/KickedChicken", 1)
            quest:AddEntityBinding("Spectator", "V_ChickenKicking/Entities/Spectator", 1)
            quest:FinalizeEntityBindings()
            quest:CreateThread("CreateSpectators")  -- native thread body CreateSpectators: lift it as function CreateSpectators(quest)
            quest:CreateThread("LookAfterOrganiser")  -- native thread body CQ_GuildTrainingScript::KeepTabsOnWhisper: lift it as function LookAfterOrganiser(quest)
            return
        end
        if not quest:NewScriptFrame() then break end
        isRegionLoaded = quest:IsRegionLoaded("OakBay")
    end
end

-- V_ChickenKicking.Init (retail 0x00e628b0)
function Init(quest)
    quest:SetStateBool("ChickenLanded", false)
    quest:SetStateInt("FinalSector", 0)
    quest:SetStateInt("DistanceBand", 0)
    quest:SetStateBool("KnowGhostHasGone", false)
    quest:SetStateBool("SpectatorsCreated", false)
    quest:SetStateBool("RanOff", false)
    quest:SetStateBool("TalkedTo", false)
    quest:SetStateBool("SpectatorsUnderAttack", false)
    quest:SetStateInt("PrizesWon", 0)
    quest:SetStateBool("InfoDisplayed", false)
end

-- V_ChickenKicking.OnPersist (retail 0x00e629a0)
function OnPersist(quest, context)
    quest:SetStateInt("PrizesWon", quest:PersistTransferUInt(context, "PrizesWon", quest:GetStateInt("PrizesWon") or 0))
    quest:SetStateBool("InfoDisplayed", quest:PersistTransferBool(context, "InfoDisplayed", quest:GetStateBool("InfoDisplayed")))
    quest:SetStateBool("KnowGhostHasGone", quest:PersistTransferBool(context, "KnowGhostHasGone", quest:GetStateBool("KnowGhostHasGone")))
end

-- V_ChickenKicking.CreateSpectators (retail 0x00e62d60)
function CreateSpectators(quest)
    local isRegionLoaded
    local spectatorsCreated = quest:GetStateBool("SpectatorsCreated")
    while true do
        if spectatorsCreated then
            return
        end
        if not quest:NewScriptFrame() then return end
        while true do
            isRegionLoaded = quest:IsRegionLoaded("OakBay")
            if not isRegionLoaded or quest:GetStateBool("SpectatorsCreated") then
                isRegionLoaded = false
            end
            if not isRegionLoaded then break end
            if not quest:NewScriptFrame() then return end
            if quest:GetStateBool("KnowGhostHasGone") then
                local spectator1 = quest:GetThingWithScriptName("Spectator1")
                quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", spectator1:GetPos(), "Spectator")
                local spectator = quest:GetThingWithScriptName("Spectator2")
                quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", spectator:GetPos(), "Spectator")
                local spectator3 = quest:GetThingWithScriptName("Spectator3")
                quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", spectator3:GetPos(), "Spectator")
                quest:SetStateBool("SpectatorsCreated", true)
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        while quest:IsRegionLoaded("OakBay") do
            if not quest:NewScriptFrame() then return end
        end
        spectatorsCreated = quest:GetStateBool("SpectatorsCreated")
    end
end

-- V_ChickenKicking.LookAfterOrganiser (retail 0x00e631f0)
function LookAfterOrganiser(quest)
    local predicateResult = quest:IsActiveThreadTerminating()
    while true do
        if predicateResult then
            return
        end
        while quest:IsRegionLoaded("OakBay") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then return end
        while not quest:IsRegionLoaded("OakBay") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
        if not quest:GetStateBool("RanOff") then quest:NewScriptFrame(); predicateResult = quest:IsActiveThreadTerminating(); goto continue_1 end
        if quest:IsActiveThreadTerminating() then return end
        if not quest:GetStateBool("KnowGhostHasGone") then
            if quest:IsActiveThreadTerminating() then return end
            local scaredMasterMarker = quest:GetThingWithScriptName("ScaredMasterMarker")
            quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", scaredMasterMarker:GetPos(), "ChickenMaster")
        else
            if quest:IsActiveThreadTerminating() then return end
            local mkCkOrg = quest:GetThingWithScriptName("MK_CK_ORG")
            quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", mkCkOrg:GetPos(), "ChickenMaster")
        end
        quest:SetStateBool("RanOff", false)
        quest:NewScriptFrame()
        predicateResult = quest:IsActiveThreadTerminating()
        ::continue_1::
    end
end

-- V_ChickenKicking.helper_E68B20 (retail 0x00e68b20)
function helper_E68B20(quest, strParam1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "CHICKEN", resources:MemberResource("seh_Chicken"))
    resources:SetActor(actorMap, "ORGANISER", resources:MemberResource("seh_ChickenMaster"))
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(strParam1, actorMap, resources:MemberStringMap("csargs"), false, true)
    quest:FixMovieSequenceCamera(false)
    resources:ClearStringMap(resources:MemberStringMap("csargs"))
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

