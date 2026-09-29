-- Readable native conversion: V_SickChild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- V_SickChild.Main (retail 0x00ec54e0)
function Main(quest)
    quest:AddEntityBinding("SickChildsMother", "V_SickChild/Entities/SickChildsMother")
    quest:AddEntityBinding("SickChild", "V_SickChild/Entities/SickChild")
    quest:AddEntityBinding("SickChildsSister", "V_SickChild/Entities/SickChildsSister")
    quest:AddEntityBinding("Witch", "V_SickChild/Entities/Witch")
    quest:AddEntityBinding("ManInLove", "V_SickChild/Entities/ManInLove")
    quest:AddEntityBinding("MansLover", "V_SickChild/Entities/MansLover")
    quest:AddEntityBinding("IngredientOwner", "V_SickChild/Entities/IngredientOwner")
    quest:AddEntityBinding("TalkingTrader1", "V_SickChild/Entities/TalkingTrader1")
    quest:AddEntityBinding("TalkingTrader2", "V_SickChild/Entities/TalkingTrader2")
    quest:AddEntityBinding("WomanToAttract", "V_SickChild/Entities/WomanToAttract")
    quest:AddEntityBinding("SickChildFishingSpot", "V_SickChild/Entities/SickChildFishingSpot")
    quest:FinalizeEntityBindings()
    while not quest:IsQuestCompleted("QS_GuardianSisterInfo") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:ActivateQuest("V_SickChild_Activate")
    quest:ActivateQuest("V_SickChildBarrowFields")
    while not quest:GetStateBool("FinishedQuest") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:SetQuestAsCompleted(quest:GetActiveQuestName(), false, false, false)
    while quest:IsRegionLoaded("BowerstoneSlums") do
        if not quest:NewScriptFrame() then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    quest:DeactivateQuestLater("V_SickChildBarrowFields", 0)
    quest:DeactivateQuestLater("V_SickChild_Activate", 0)
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- V_SickChild.Init (retail 0x00ec5420)
function Init(quest)
    quest:SetStateBool("FinishedQuest", false)
    quest:SetStateBool("GotFishingSpotMushroom", false)
    quest:SetStateBool("MotherIntroDone", false)
    quest:SetStateInt("MansLoverState", 0)
    quest:SetStateBool("OwnerAlive", true)
    quest:SetStateBool("LaughingWomanKilled", false)
end

-- V_SickChild.OnPersist (retail 0x00ecd7b0)
function OnPersist(quest, context)
    quest:SetStateBool("FinishedQuest", quest:PersistTransferBool(context, "FinishedQuest", quest:GetStateBool("FinishedQuest")))
    quest:SetStateBool("GotFishingSpotMushroom", quest:PersistTransferBool(context, "GotFishingSpotMushroom", quest:GetStateBool("GotFishingSpotMushroom")))
    quest:SetStateBool("MotherIntroDone", quest:PersistTransferBool(context, "MotherIntroDone", quest:GetStateBool("MotherIntroDone")))
    quest:SetStateInt("MansLoverState", quest:PersistTransferInt(context, "MansLoverState", quest:GetStateInt("MansLoverState") or 0))
    quest:SetStateBool("OwnerAlive", quest:PersistTransferBool(context, "OwnerAlive", quest:GetStateBool("OwnerAlive")))
    quest:SetStateBool("LaughingWomanKilled", quest:PersistTransferBool(context, "LaughingWomanKilled", quest:GetStateBool("LaughingWomanKilled")))
end

-- V_SickChild.helper_ECE460 (retail 0x00ece460)
function helper_ECE460(quest, macroName, argument)
    local resources = quest:RetailResources()
    local actorMap = resources:NewStringMap()
    local resource = resources:NewResource()
    resources:TryAcquire(resource, quest:GetHero(), 4)
    local scratchValue = "NULL" or ""
    if string.lower(argument or "") ~= string.lower(scratchValue) then
        resources:SetString(actorMap, "$ARG1", argument or "")
    end
    local actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "WITCH", resources:MemberResource("seh_Witch"))
    resources:SetActor(actorMap2, "MUM", resources:MemberResource("seh_Mother"))
    resources:SetActor(actorMap2, "HERO", resource)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(macroName, actorMap2, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(resource)
    resources:DestroyStringMap(actorMap)
end

