-- Generated native draft: V_SickChild. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar7, cVar1, pCVar5
    local alive = true
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
    bVar7 = quest:IsQuestCompleted("QS_GuardianSisterInfo")
    while not bVar7 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if bVar7 then
            return
        end
        bVar7 = quest:IsQuestCompleted("QS_GuardianSisterInfo")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar7 = not alive
    if not bVar7 then
        quest:ActivateQuest("V_SickChild_Activate")
        quest:ActivateQuest("V_SickChildBarrowFields")
        cVar1 = quest:GetStateBool("FinishedQuest")
        while not cVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar7 = not alive
            if bVar7 then
                return
            end
            cVar1 = quest:GetStateBool("FinishedQuest")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if not bVar7 then
            bVar7 = false
            pCVar5 = quest:GetActiveQuestName()
            quest:SetQuestAsCompleted(pCVar5, bVar7, false, false)
            bVar7 = quest:IsRegionLoaded("BowerstoneSlums")
            if bVar7 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar7 = not alive
                    if bVar7 then
                        return
                    end
                    bVar7 = quest:IsRegionLoaded("BowerstoneSlums")
                until not (bVar7)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar7 = not alive
            if not bVar7 then
                quest:DeactivateQuestLater("V_SickChildBarrowFields", 0)
                quest:DeactivateQuestLater("V_SickChild_Activate", 0)
                pCVar5 = quest:GetActiveQuestName()
                quest:DeactivateQuestLater(pCVar5, 0)
            end
        end
    end
end

function Init(quest)
    quest:SetStateBool("FinishedQuest", false)
    quest:SetStateBool("GotFishingSpotMushroom", false)
    quest:SetStateBool("MotherIntroDone", false)
    quest:SetStateInt("MansLoverState", 0)
    quest:SetStateBool("OwnerAlive", true)
    quest:SetStateBool("LaughingWomanKilled", false)
end

function OnPersist(quest, context)
    local finishedQuest = quest:GetStateBool("FinishedQuest") or false
    finishedQuest = quest:PersistTransferBool(context, "FinishedQuest", finishedQuest)
    quest:SetStateBool("FinishedQuest", finishedQuest)
    local gotFishingSpotMushroom = quest:GetStateBool("GotFishingSpotMushroom") or false
    gotFishingSpotMushroom = quest:PersistTransferBool(context, "GotFishingSpotMushroom", gotFishingSpotMushroom)
    quest:SetStateBool("GotFishingSpotMushroom", gotFishingSpotMushroom)
    local motherIntroDone = quest:GetStateBool("MotherIntroDone") or false
    motherIntroDone = quest:PersistTransferBool(context, "MotherIntroDone", motherIntroDone)
    quest:SetStateBool("MotherIntroDone", motherIntroDone)
    local mansLoverState = quest:GetStateInt("MansLoverState") or 0
    mansLoverState = quest:PersistTransferInt(context, "MansLoverState", mansLoverState)
    quest:SetStateInt("MansLoverState", mansLoverState)
    local ownerAlive = quest:GetStateBool("OwnerAlive") or false
    ownerAlive = quest:PersistTransferBool(context, "OwnerAlive", ownerAlive)
    quest:SetStateBool("OwnerAlive", ownerAlive)
    local laughingWomanKilled = quest:GetStateBool("LaughingWomanKilled") or false
    laughingWomanKilled = quest:PersistTransferBool(context, "LaughingWomanKilled", laughingWomanKilled)
    quest:SetStateBool("LaughingWomanKilled", laughingWomanKilled)
end

function helper_ECE460(quest)
    local resources = quest:RetailResources()
    local amStack_28, i_stk_14, p0, p1, pCVar7, pOther, piVar1, piVar2, ppuVar4, xStack_28
    -- TODO(native): local_1c = malloc(0x18);
    -- TODO(native): *local_1c = 0;
    -- TODO(native): *(undefined4 *)(local_1c + 4) = 0;
    -- TODO(native): *(undefined1 **)(local_1c + 8) = local_1c;
    -- TODO(native): *(undefined1 **)(local_1c + 0xc) = local_1c;
    local xStack_10 = resources:NewResource()
    local p3 = 0x0
    local p2 = quest:GetHero()
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    if unaff_EBP == nil then
    else
        -- TODO(native): p1 = *unaff_EBP
        p1 = nil --[[unresolved native value]]
    end
    if nil == nil then
    else
        -- TODO(native): p0 = *0x0
        p0 = nil --[[unresolved native value]]
    end
    local iVar6 = _stricmp(p0,p1,p2,p3)
    if iVar6 ~= 0 then
        pOther = 0x0
        resources:SetString(amStack_28, "$ARG1", pOther)
    end
    -- TODO(native): puStack_34 = malloc(0x24);
    -- TODO(native): *puStack_34 = 0;
    -- TODO(native): *(undefined4 *)(puStack_34 + 4) = 0;
    -- TODO(native): *(undefined1 **)(puStack_34 + 8) = puStack_34;
    -- TODO(native): *(undefined1 **)(puStack_34 + 0xc) = puStack_34;
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&puStack_34,&xStack_30);
    -- TODO(native): local piVar1 = *(this + 0x64)
    -- TODO(native): local piVar2 = *(pCVar7 + 0xc)
    local uVar3 = quest:GetStateInt("self_0x60")
    if piVar2 ~= piVar1 then
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piVar1;
        if piVar1 ~= nil then
            -- TODO(native): *piVar1 = *piVar1 + 1;
        end
    end
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&puStack_34,&xStack_30);
    -- TODO(native): piVar1 = *(this + 0x54)
    piVar1 = nil --[[unresolved native value]]
    -- TODO(native): piVar2 = *(pCVar7 + 0xc)
    piVar2 = nil --[[unresolved native value]]
    uVar3 = quest:GetStateInt("self_0x50")
    if piVar2 ~= piVar1 then
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piVar1;
        if piVar1 ~= nil then
            -- TODO(native): *piVar1 = *piVar1 + 1;
        end
    end
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&puStack_34,&xStack_30);
    local ppuVar5 = 0x0
    -- TODO(native): local ppuVar4 = *(pCVar7 + 0xc)
    if ppuVar4 ~= nil then
        if ppuVar4 ~= nil then
            -- TODO(native): *ppuVar4 = *ppuVar4 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(int *)(pCVar7 + 8) = i_stk_14;
        -- TODO(native): *(undefined ***)(pCVar7 + 0xc) = ppuVar5;
        if ppuVar5 ~= nil then
            -- TODO(native): *ppuVar5 = *ppuVar5 + 1;
        end
    end
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(0x0, xStack_28, 0, false, true)
    quest:FixMovieSequenceCamera(false)
    if i_stk_14 ~= 0 then
        -- TODO(native): StdMap_DestroyNode(&xStack_28,unaff_EBP[1]);
        -- TODO(native): unaff_EBP[2] = unaff_EBP;
        -- TODO(native): unaff_EBP[1] = 0;
        -- TODO(native): unaff_EBP[3] = unaff_EBP;
    end
    if unaff_EBP ~= nil then
        -- TODO(native): free(unaff_EBP);
    end
    resources:ReleaseResource(0x0)
    if 0 ~= 0 then
        -- TODO(native): LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray((LTextTreeWalkThrough *)&xStack_1c,*(int *)((int)xStack_1c + 4));
        -- TODO(native): *(void **)((int)xStack_1c + 8) = xStack_1c;
        -- TODO(native): *(undefined4 *)((int)xStack_1c + 4) = 0;
        -- TODO(native): *(void **)((int)xStack_1c + 0xc) = xStack_1c;
    end
    if nil ~= nil then
        -- TODO(native): free(xStack_1c);
    end
end

