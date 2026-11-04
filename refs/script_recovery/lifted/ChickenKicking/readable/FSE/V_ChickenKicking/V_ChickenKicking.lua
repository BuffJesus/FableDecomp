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
    local prizesWon = quest:GetStateInt("PrizesWon") or 0
    prizesWon = quest:PersistTransferUInt(context, "PrizesWon", prizesWon)
    quest:SetStateInt("PrizesWon", prizesWon)
    local infoDisplayed = quest:GetStateBool("InfoDisplayed") or false
    infoDisplayed = quest:PersistTransferBool(context, "InfoDisplayed", infoDisplayed)
    quest:SetStateBool("InfoDisplayed", infoDisplayed)
    local knowGhostHasGone = quest:GetStateBool("KnowGhostHasGone") or false
    knowGhostHasGone = quest:PersistTransferBool(context, "KnowGhostHasGone", knowGhostHasGone)
    quest:SetStateBool("KnowGhostHasGone", knowGhostHasGone)
end

-- V_ChickenKicking.CreateSpectators (retail 0x00e62d60)
function CreateSpectators(quest)
    local isRegionLoaded
    local spectatorsCreated = quest:GetStateBool("SpectatorsCreated")
    while true do
        if spectatorsCreated then
            quest:IsActiveThreadTerminating()
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
                if quest:IsActiveThreadTerminating() then return end
                local position = quest:GetThingWithScriptName("Spectator1"):GetPos()
                quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position, "Spectator", false)
                local position2 = quest:GetThingWithScriptName("Spectator2"):GetPos()
                quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", position2, "Spectator", false)
                local position3 = quest:GetThingWithScriptName("Spectator3"):GetPos()
                quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position3, "Spectator", false)
                quest:SetStateBool("SpectatorsCreated", true)
            end
        end
        if quest:IsActiveThreadTerminating() then return end
        while quest:IsRegionLoaded("OakBay") do
            if not quest:NewScriptFrame() then return end
        end
        if quest:IsActiveThreadTerminating() then break end
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
        if not quest:GetStateBool("RanOff") then
            quest:NewScriptFrame();
            predicateResult = quest:IsActiveThreadTerminating();
        else
            if quest:IsActiveThreadTerminating() then return end
            if not quest:GetStateBool("KnowGhostHasGone") then
                if quest:IsActiveThreadTerminating() then return end
                local scaredMasterMarker = quest:GetThingWithScriptName("ScaredMasterMarker")
                local position = scaredMasterMarker:GetPos()
                quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position, "ChickenMaster", false)
            else
                if quest:IsActiveThreadTerminating() then return end
                local mkCkOrg = quest:GetThingWithScriptName("MK_CK_ORG")
                local position2 = mkCkOrg:GetPos()
                quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", position2, "ChickenMaster", false)
            end
            quest:SetStateBool("RanOff", false)
            quest:NewScriptFrame()
            predicateResult = quest:IsActiveThreadTerminating()
        end
        ::continue_1::
    end
end

-- V_ChickenKicking.helper_E68B20 (retail 0x00e68b20)
function helper_E68B20(quest, strParam1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    local pThing = quest:GetHero()
    resources:TryAcquire(resource, pThing, 4)
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

