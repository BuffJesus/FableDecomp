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
function helper_ECE460(quest)
    local resources = quest:RetailResources()
    local scratchValue, p0, p1, actorMap, scratchValue2
    -- TODO(native): local_1c = malloc(0x18);
    -- TODO(native): *local_1c = 0;
    -- TODO(native): *(undefined4 *)(local_1c + 4) = 0;
    -- TODO(native): *(undefined1 **)(local_1c + 8) = local_1c;
    -- TODO(native): *(undefined1 **)(local_1c + 0xc) = local_1c;
    local resource = resources:NewResource()
    local p2 = quest:GetHero()
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    if unaff_EBP ~= nil then
        -- TODO(native): p1 = *unaff_EBP
        p1 = nil --[[unresolved native value]]
    end
    if resource ~= nil then
        -- TODO(native): p0 = *xStack_10
        p0 = nil --[[unresolved native value]]
    end
    if _stricmp(p0,p1,p2,resource) ~= 0 then
        resources:SetString(0, "$ARG1", resource)
    end
    -- TODO(native): puStack_34 = malloc(0x24);
    -- TODO(native): *puStack_34 = 0;
    -- TODO(native): *(undefined4 *)(puStack_34 + 4) = 0;
    -- TODO(native): *(undefined1 **)(puStack_34 + 8) = puStack_34;
    -- TODO(native): *(undefined1 **)(puStack_34 + 0xc) = puStack_34;
    resources:SetActor(actorMap, "WITCH", resources:MemberResource("seh_Witch"))
    resources:SetActor(actorMap, "MUM", resources:MemberResource("seh_Mother"))
    -- TODO(native): resources:SetActor(puStack_34, "HERO", &local_1c)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(resource, scratchValue2, 0, false, true)
    quest:FixMovieSequenceCamera(false)
    if scratchValue ~= 0 then
        -- TODO(native): StdMap_DestroyNode(&xStack_28,unaff_EBP[1]);
        -- TODO(native): unaff_EBP[2] = unaff_EBP;
        -- TODO(native): unaff_EBP[1] = 0;
        -- TODO(native): unaff_EBP[3] = unaff_EBP;
    end
    if unaff_EBP ~= nil then
        -- TODO(native): free(unaff_EBP);
    end
    resources:ReleaseResource(resource)
    if nil ~= nil then
        -- TODO(native): free(xStack_1c);
    end
end

