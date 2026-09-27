-- Generated native draft: V_ChickenKicking. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local alive = true
    local bVar1 = quest:IsRegionLoaded("OakBay")
    while true do
        if bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if not bVar1 then
                quest:AddEntityBinding("ChickenMaster", "V_ChickenKicking/Entities/ChickenMaster", 1)
                quest:AddEntityBinding("ChickenSign", "V_ChickenKicking/Entities/ChickenSign", 1)
                quest:AddEntityBinding("KickedChicken", "V_ChickenKicking/Entities/KickedChicken", 1)
                quest:AddEntityBinding("Spectator", "V_ChickenKicking/Entities/Spectator", 1)
                quest:FinalizeEntityBindings()
                quest:CreateThread("CreateSpectators")  -- native thread body CreateSpectators: lift it as function CreateSpectators(quest)
                quest:CreateThread("LookAfterOrganiser")  -- native thread body CQ_GuildTrainingScript::KeepTabsOnWhisper: lift it as function LookAfterOrganiser(quest)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then break end
        bVar1 = quest:IsRegionLoaded("OakBay")
    end
end

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

function CreateSpectators(quest)
    local bVar3, pCVar4, pCVar5, r1, r2, r3
    local alive = true
    local CVar1 = quest:GetStateBool("SpectatorsCreated")
    while true do
        if CVar1 then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        while true do
            bVar3 = quest:IsRegionLoaded("OakBay")
            if (not bVar3) or (quest:GetStateBool("SpectatorsCreated")) then
                bVar3 = false
            end
            if not bVar3 then break end
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            if quest:GetStateBool("KnowGhostHasGone") then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                pCVar4 = quest:GetThingWithScriptName("Spectator1")
                bVar3 = false
                pCVar5 = pCVar4:GetPos()
                r1 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar5, "Spectator")
                r1 = nil
                pCVar4 = nil
                pCVar4 = quest:GetThingWithScriptName("Spectator2")
                bVar3 = false
                pCVar5 = pCVar4:GetPos()
                r2 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_FEMALE_UNEMPLOYED", pCVar5, "Spectator")
                r2 = nil
                pCVar4 = nil
                pCVar4 = quest:GetThingWithScriptName("Spectator3")
                bVar3 = false
                pCVar5 = pCVar4:GetPos()
                r3 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar5, "Spectator")
                r3 = nil
                pCVar4 = nil
                quest:SetStateBool("SpectatorsCreated", true)
            end
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsRegionLoaded("OakBay")
        if bVar3 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then
                    return
                end
                bVar3 = quest:IsRegionLoaded("OakBay")
            until not (bVar3)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then break end
        CVar1 = quest:GetStateBool("SpectatorsCreated")
    end
end

function LookAfterOrganiser(quest)
    local bVar2, pCVar3, pCVar4, r1, r2
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    bVar2 = not alive
    while true do
        if bVar2 then
            return
        end
        bVar2 = quest:IsRegionLoaded("OakBay")
        if bVar2 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                bVar2 = quest:IsRegionLoaded("OakBay")
            until not (bVar2)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            return
        end
        bVar2 = quest:IsRegionLoaded("OakBay")
        while not bVar2 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            bVar2 = quest:IsRegionLoaded("OakBay")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        if quest:GetStateBool("RanOff") then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                return
            end
            if not quest:GetStateBool("KnowGhostHasGone") then
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                pCVar3 = quest:GetThingWithScriptName("ScaredMasterMarker")
                bVar2 = false
                pCVar4 = pCVar3:GetPos()
                r1 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar4, "ChickenMaster")
            else
                alive = not quest:IsActiveThreadTerminating()
                bVar2 = not alive
                if bVar2 then
                    return
                end
                pCVar3 = quest:GetThingWithScriptName("MK_CK_ORG")
                bVar2 = false
                pCVar4 = pCVar3:GetPos()
                r2 = quest:CreateCreature("CREATURE_OAKVALE_VILLAGER_MALE_UNEMPLOYED", pCVar4, "ChickenMaster")
            end
            quest:SetStateBool("RanOff", false)
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
    end
end

function helper_E68B20(quest, native_arg_strParam_1)
    local resources = quest:RetailResources()
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_1c = resources:NewActorMap()
    resources:SetActor(xStack_1c, "HERO", xStack_10)
    resources:SetActor(xStack_1c, "CHICKEN", resources:MemberResource("seh_Chicken"))
    resources:SetActor(xStack_1c, "ORGANISER", resources:MemberResource("seh_ChickenMaster"))
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(native_arg_strParam_1, xStack_1c, resources:MemberStringMap("csargs"), false, true)
    quest:FixMovieSequenceCamera(false)
    resources:ClearStringMap(resources:MemberStringMap("csargs"))
    resources:DestroyActorMap(xStack_1c)
    resources:ReleaseResource(xStack_10)
end

