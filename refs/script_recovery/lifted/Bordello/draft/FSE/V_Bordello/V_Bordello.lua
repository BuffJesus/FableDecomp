-- Generated native draft: V_Bordello. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local bVar4
    quest:AddEntityBinding("Madame", "V_Bordello/Entities/Madame", 1)
    quest:AddEntityBinding("BordelloLady", "V_Bordello/Entities/BordelloLady", 1)
    quest:AddEntityBinding("BordelloGuard", "V_Bordello/Entities/BordelloGuard", 1)
    quest:AddEntityBinding("Magicman", "V_Bordello/Entities/Magicman", 1)
    quest:AddEntityBinding("BordelloClient", "V_Bordello/Entities/BordelloClient", 1)
    quest:AddEntityBinding("M_HeroDownstairs", "V_Bordello/Entities/M_HeroDownstairs", 1)
    quest:AddEntityBinding("BordelloEntrance", "V_Bordello/Entities/BordelloEntrance")
    quest:FinalizeEntityBindings()
    quest:CreateThread("WatchForBordelloStatus")  -- native thread body NScript::CQ_AmbushTradersScript::WatchForAllTradersDead: lift it as function WatchForBordelloStatus(quest)
    quest:CreateThread("WatchForDeedStatus")  -- native thread body NScript::CV_BordelloScript::WatchForDeedStatus: lift it as function WatchForDeedStatus(quest)
    if not bVar4 then
    end
    quest:CreateThread("CreateCustomers")  -- native thread body NScript::CV_BordelloScript::CreateCustomers: lift it as function CreateCustomers(quest)
    if not bVar4 then
    end
    quest:CreateThread("AdjustTavernPrices")  -- native thread body CScriptGameResourceObjectScriptedThingBase_HandleBordelloNPC: lift it as function AdjustTavernPrices(quest)
    if not bVar4 then
    end
end

function Init(quest)
    local x_stk_c
    quest:SetStateBool("HadSex", false)
    quest:SetStateBool("HeroPartying", false)
    quest:SetStateBool("HeroTricking", false)
    quest:SetStateBool("MagicianSleeping", false)
    quest:SetStateBool("CutscenePlaying", false)
    quest:SetStateBool("PlayerOwned", false)
    quest:SetStateBool("BecomeNunnery", false)
    quest:SetStateBool("PlayerHasLeftRegion", false)
    quest:SetStateBool("HeroFoundDeeds", false)
    quest:SetStateBool("HeroFoundDeedsLocation", false)
    quest:SetStateInt("ClientsAlive", 0)
    quest:SetStateBool("WearingWhoreWig", false)
    quest:SetStateBool("ClientInUse_0", false)
    -- TODO(native): name field 0x59 (undefined1)
    quest:SetStateBool("self_0x59", false)
    -- TODO(native): name field 0x5a (undefined1)
    quest:SetStateBool("self_0x5a", false)
    x_stk_c = nil
    -- TODO(native): CScriptThing::operator=((CScriptThing *)__element("ClientInUse", 1),(CScriptThing *)0x0);
    x_stk_c = nil
    x_stk_c = 0
    quest:SetStateInt("BeersDrunk", 0)
    quest:SetStateBool("BeerSetToDPad", false)
end

function OnPersist(quest, context)
    local hadSex = quest:GetStateBool("HadSex") or false
    hadSex = quest:PersistTransferBool(context, "HadSex", hadSex)
    quest:SetStateBool("HadSex", hadSex)
    local heroPartying = quest:GetStateBool("HeroPartying") or false
    heroPartying = quest:PersistTransferBool(context, "HeroPartying", heroPartying)
    quest:SetStateBool("HeroPartying", heroPartying)
    local heroTricking = quest:GetStateBool("HeroTricking") or false
    heroTricking = quest:PersistTransferBool(context, "HeroTricking", heroTricking)
    quest:SetStateBool("HeroTricking", heroTricking)
    local magicianSleeping = quest:GetStateBool("MagicianSleeping") or false
    magicianSleeping = quest:PersistTransferBool(context, "MagicianSleeping", magicianSleeping)
    quest:SetStateBool("MagicianSleeping", magicianSleeping)
    local playerOwned = quest:GetStateBool("PlayerOwned") or false
    playerOwned = quest:PersistTransferBool(context, "PlayerOwned", playerOwned)
    quest:SetStateBool("PlayerOwned", playerOwned)
    local becomeNunnery = quest:GetStateBool("BecomeNunnery") or false
    becomeNunnery = quest:PersistTransferBool(context, "BecomeNunnery", becomeNunnery)
    quest:SetStateBool("BecomeNunnery", becomeNunnery)
    local playerHasLeftRegion = quest:GetStateBool("PlayerHasLeftRegion") or false
    playerHasLeftRegion = quest:PersistTransferBool(context, "PlayerHasLeftRegion", playerHasLeftRegion)
    quest:SetStateBool("PlayerHasLeftRegion", playerHasLeftRegion)
    local heroFoundDeeds = quest:GetStateBool("HeroFoundDeeds") or false
    heroFoundDeeds = quest:PersistTransferBool(context, "HeroFoundDeeds", heroFoundDeeds)
    quest:SetStateBool("HeroFoundDeeds", heroFoundDeeds)
    local heroFoundDeedsLocation = quest:GetStateBool("HeroFoundDeedsLocation") or false
    heroFoundDeedsLocation = quest:PersistTransferBool(context, "HeroFoundDeedsLocation", heroFoundDeedsLocation)
    quest:SetStateBool("HeroFoundDeedsLocation", heroFoundDeedsLocation)
    local clientInUse[0] = quest:GetStateBool("ClientInUse[0]") or false
    clientInUse[0] = quest:PersistTransferBool(context, "ClientInUse[0]", clientInUse[0])
    quest:SetStateBool("ClientInUse[0]", clientInUse[0])
    local clientInUse[1] = quest:GetStateBool("ClientInUse[1]") or false
    clientInUse[1] = quest:PersistTransferBool(context, "ClientInUse[1]", clientInUse[1])
    quest:SetStateBool("ClientInUse[1]", clientInUse[1])
    local clientInUse[2] = quest:GetStateBool("ClientInUse[2]") or false
    clientInUse[2] = quest:PersistTransferBool(context, "ClientInUse[2]", clientInUse[2])
    quest:SetStateBool("ClientInUse[2]", clientInUse[2])
    local beersDrunk = quest:GetStateInt("BeersDrunk") or 0
    beersDrunk = quest:PersistTransferInt(context, "BeersDrunk", beersDrunk)
    quest:SetStateInt("BeersDrunk", beersDrunk)
end

function WatchForBordelloStatus(quest)
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar1 = not alive
    repeat
        if bVar1 then
            return
        end
        bVar1 = quest:IsRegionLoaded("Bordello")
        if not bVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            bVar1 = quest:IsRegionLoaded("Bordello")
            while not bVar1 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return
                end
                bVar1 = quest:IsRegionLoaded("Bordello")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return
            end
            quest:SetStateBool("PlayerHasLeftRegion", true)
            quest:SetStateBool("HeroPartying", false)
            quest:SetStateBool("MagicianSleeping", false)
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    until false
end

function WatchForDeedStatus(quest)
    local CVar1, bVar3, pCVar4, r1
    local alive = true
    bVar3 = quest:IsRegionLoaded("Bordello")
    while not bVar3 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            return
        end
        bVar3 = quest:IsRegionLoaded("Bordello")
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        if quest:GetStateBool("HeroFoundDeeds") then
            alive = not quest:IsActiveThreadTerminating()
            return
        end
        r1 = quest:GetThingWithScriptName("M_HiddenDeeds")
        quest:EntitySetInLimbo(r1, true, true)
        CVar1 = quest:GetStateBool("HeroFoundDeedsLocation")
        while not CVar1 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then
                return
            end
            CVar1 = quest:GetStateBool("HeroFoundDeedsLocation")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then
            -- LAB_00e3a26a: (native jump target)
            return
        end
        quest:EntitySetInLimbo(r1, false, true)
        pCVar4 = quest:GetHero()
        bVar3 = quest:IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", pCVar4)
        while not bVar3 do
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if bVar3 then goto LAB_00e3a331 end
            pCVar4 = quest:GetHero()
            bVar3 = quest:IsObjectInThingsPossession("OBJECT_DEEDS_BORDELLO", pCVar4)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            quest:SetStateBool("HeroFoundDeeds", true)
        end
        ::LAB_00e3a331::
    end
end

function CreateCustomers(quest)
    local bVar1, iVar3, pCVar4, pCVar5, r1, r2
    local alive = true
    local iVar2 = quest:RegisterTimer()
    local i_stk_34 = iVar2
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    repeat
        if bVar1 then
            -- LAB_00e3a684: (native jump target)
            quest:DeregisterTimer(i_stk_34)
            return
        end
        bVar1 = quest:IsRegionLoaded("Bordello")
        if bVar1 then
            repeat
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    quest:DeregisterTimer(i_stk_34)
                    return
                end
                iVar3 = quest:GetTimer(i_stk_34)
                if (iVar3 == 0) and (quest:GetStateInt("ClientsAlive") < 3) then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar1 = not alive
                    if bVar1 then
                        quest:DeregisterTimer(i_stk_34)
                        return
                    end
                    quest:SetTimer(i_stk_34, 0xf)
                    if not quest:GetStateBool("BecomeNunnery") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if bVar1 then
                            quest:DeregisterTimer(i_stk_34)
                            return
                        end
                        pCVar4 = quest:GetThingWithScriptName("M_ClientSpawn")
                        bVar1 = false
                        pCVar5 = pCVar4:GetPos()
                        r1 = quest:CreateCreature("CREATURE_BS_VILLAGER_MALE", pCVar5, "BordelloClient")
                        r1 = nil
                        pCVar4 = nil
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        bVar1 = not alive
                        if bVar1 then
                            quest:DeregisterTimer(i_stk_34)
                            return
                        end
                        pCVar4 = quest:GetThingWithScriptName("M_ClientSpawn")
                        bVar1 = false
                        pCVar5 = pCVar4:GetPos()
                        r2 = quest:CreateCreature("CREATURE_BS_VILLAGER_FEMALE", pCVar5, "BordelloClient")
                        r2 = nil
                        pCVar4 = nil
                    end
                    iVar2 = i_stk_34
                end
                bVar1 = quest:IsRegionLoaded("Bordello")
            until not (bVar1)
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            -- LAB_00e3a69c: (native jump target)
            quest:DeregisterTimer(i_stk_34)
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
    until false
end

function AdjustTavernPrices(quest)
    local pCVar6
    local alive = true
    alive = not quest:IsActiveThreadTerminating()
    local bVar5 = not alive
    if not bVar5 then
        while true do
            bVar5 = quest:IsRegionLoaded("Bordello")
            while not bVar5 do
                alive = quest:NewScriptFrame()
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                bVar5 = quest:IsRegionLoaded("Bordello")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then break end
            pCVar6 = quest:GetHero()
            pCVar6 = quest:GetNearestWithDefName(pCVar6, "VILLAGE_BORDELLO")
            -- TODO(native): CScriptThing::operator=((CScriptThing *)__element("ClientInUse", 1),(CScriptThing *)pCVar6);
            pCVar6 = nil
            pCVar6 = quest:GetHero()
            pCVar6 = quest:GetNearestWithDefName(pCVar6, "CREATURE_OAKVALE_VILLAGER_FEMALE_BARMAID")
            quest:EntitySetOpinionReactionMask(pCVar6, "OPINION_REACTION_MASK_DONT_SCREAM")
            pCVar6 = nil
            pCVar6 = quest:GetHero()
            pCVar6 = quest:GetNearestWithDefName(pCVar6, "CREATURE_OAKVALE_VILLAGER_MALE_BARMAN")
            quest:EntitySetOpinionReactionMask(pCVar6, "OPINION_REACTION_MASK_DONT_SCREAM")
            pCVar6 = nil
            bVar5 = quest:IsRegionLoaded("Bordello")
            if bVar5 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    if quest:GetStateBool("PlayerOwned") then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        quest:SetTradingPriceMult(quest:ReadGlobalGameDataFloat(0x4fc))
                        break
                    end
                    bVar5 = quest:IsRegionLoaded("Bordello")
                until not (bVar5)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            bVar5 = quest:IsRegionLoaded("Bordello")
            if bVar5 then
                repeat
                    alive = quest:NewScriptFrame()
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    bVar5 = quest:IsRegionLoaded("Bordello")
                until not (bVar5)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            quest:SetTradingPriceMult(1.0)
            quest:SetStateInt("BeersDrunk", 0)
            alive = quest:NewScriptFrame()
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
        end
    end
end

function null(quest)
    local bVar1
    local alive = true
    while true do
        bVar1 = quest:IsRegionLoaded("Bordello")
        if (not bVar1) or (not quest:GetStateBool("BeerSetToDPad")) then
            bVar1 = false
        end
        if not bVar1 then break end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar1 = not alive
        if bVar1 then
            return
        end
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    if not bVar1 then
        quest:SetStateBool("BeerSetToDPad", false)
        quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, -1)
    end
end

function helper_E3E320(quest)
    local bVar3, bVar4, bVar5, bVar6, cVar8, pCVar7
    bVar5 = false
    bVar6 = false
    pCVar7 = quest:GetHero()
    bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS")
    if bVar3 then
        goto LAB_00e3e3d9
    else
        bVar5 = true
        bVar6 = false
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS_GOOD")
        if bVar3 then goto LAB_00e3e3d9 end
        bVar5 = true
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_TROUSERS_DRESS_EVIL")
        cVar8 = 0
        if bVar3 then goto LAB_00e3e3d9 end
    end
    goto FLOW_past_lab_00e3e3d9
    ::LAB_00e3e3d9::
    cVar8 = 1
    ::FLOW_past_lab_00e3e3d9::
    if bVar6 then
    end
    if bVar5 then
    end
    bVar5 = false
    bVar6 = false
    pCVar7 = quest:GetHero()
    bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS")
    if bVar3 then
        goto LAB_00e3e4c2
    else
        bVar5 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS_GOOD")
        if bVar3 then goto LAB_00e3e4c2 end
        bVar5 = true
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar4 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_SHIRT_DRESS_EVIL")
        bVar3 = false
        if bVar4 then goto LAB_00e3e4c2 end
    end
    goto FLOW_past_lab_00e3e4c2
    ::LAB_00e3e4c2::
    bVar3 = true
    ::FLOW_past_lab_00e3e4c2::
    if bVar6 then
    end
    if bVar5 then
    end
    bVar6 = false
    if bVar3 then
        cVar8 = cVar8 + 1
    end
    pCVar7 = quest:GetHero()
    bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS")
    if bVar5 then
        goto LAB_00e3e5ad
    else
        pCVar7 = quest:GetHero()
        bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS_GOOD")
        if bVar5 then goto LAB_00e3e5ad end
        bVar6 = true
        pCVar7 = quest:GetHero()
        bVar3 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_GLOVES_DRESS_EVIL")
        bVar5 = false
        if bVar3 then goto LAB_00e3e5ad end
    end
    goto FLOW_past_lab_00e3e5ad
    ::LAB_00e3e5ad::
    bVar5 = true
    ::FLOW_past_lab_00e3e5ad::
    if bVar6 then
    end
    if bVar5 then
        cVar8 = cVar8 + 1
    end
    pCVar7 = quest:GetHero()
    bVar6 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_HAT_WHOREWIG")
    if bVar6 then
        pCVar7 = quest:GetHero()
        bVar5 = quest:IsWearingClothingItem(pCVar7, "OBJECT_HERO_NO_BOOTS")
        bVar6 = true
        if bVar5 then goto LAB_00e3e66d end
    end
    bVar6 = false
    ::LAB_00e3e66d::
    if bVar6 then
        cVar8 = cVar8 + 1
    end
    return cVar8 == '\x04'
end

function helper_E3E720(quest, native_arg_param_2, native_arg_param_3)
    local resources = quest:RetailResources()
    local bVar6, pCVar7, piStack_20, piVar2, piVar4
    local alive = true
    quest:SetStateBool("CutscenePlaying", true)
    local xStack_10 = resources:NewResource()
    local pScriptObject = xStack_10
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_1c = resources:NewActorMap()
    resources:SetActor(xStack_1c, "HERO", xStack_10)
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1c,&xStack_24);
    -- TODO(native): piStack_20 = *(int **)(this + 0x80);
    -- TODO(native): local piVar2 = *(pCVar7 + 0xc)
    local uVar3 = quest:GetStateInt("self_0x7c")
    if piVar2 ~= piStack_20 then
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piStack_20;
        if piStack_20 ~= nil then
            -- TODO(native): *piStack_20 = *piStack_20 + 1;
        end
    end
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1c,&xStack_24);
    -- TODO(native): piStack_20 = *(int **)(this + 0x90);
    -- TODO(native): piVar2 = *(pCVar7 + 0xc)
    piVar2 = nil --[[unresolved native value]]
    uVar3 = quest:GetStateInt("self_0x8c")
    if piVar2 ~= piStack_20 then
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piStack_20;
        if piStack_20 ~= nil then
            -- TODO(native): *piStack_20 = *piStack_20 + 1;
        end
    end
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1c,&xStack_24);
    -- TODO(native): piVar2 = *(this + 0xa0)
    piVar2 = nil --[[unresolved native value]]
    -- TODO(native): local piVar4 = *(pCVar7 + 0xc)
    uVar3 = quest:GetStateInt("self_0x9c")
    if piVar4 ~= piVar2 then
        if piVar4 ~= nil then
            -- TODO(native): *piVar4 = *piVar4 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piVar2;
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + 1;
        end
    end
    -- TODO(native): pCVar7 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)xStack_1c,&xStack_24);
    -- TODO(native): piVar2 = *(this + 0xb0)
    piVar2 = nil --[[unresolved native value]]
    -- TODO(native): piVar4 = *(pCVar7 + 0xc)
    piVar4 = nil --[[unresolved native value]]
    uVar3 = quest:GetStateInt("self_0xac")
    if piVar4 ~= piVar2 then
        if piVar4 ~= nil then
            -- TODO(native): *piVar4 = *piVar4 + -1;
            if **(pCVar7 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(pCVar7 + 0xc))[1])();
            end
        end
        -- TODO(native): *(undefined4 *)(pCVar7 + 8) = uVar3;
        -- TODO(native): *(int **)(pCVar7 + 0xc) = piVar2;
        if piVar2 ~= nil then
            -- TODO(native): *piVar2 = *piVar2 + 1;
        end
    end
    quest:FixMovieSequenceCamera(true)
    local cVar5 = native_arg_param_3
    if native_arg_param_3 then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            resources:DestroyActorMap(xStack_1c)
            resources:ReleaseResource(xStack_10)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(true)
    end
    -- TODO(native): RunCutsceneMacro_Func((CCharString *)&native_arg_param_2,xStack_1c,(void *)0x0,(CTCCarryable *)(this + 0xb4),false,true);
    if cVar5 then
        alive = not quest:IsActiveThreadTerminating()
        bVar6 = not alive
        if bVar6 then
            -- LAB_00e3ea29: (native jump target)
            resources:DestroyActorMap(xStack_1c)
            resources:ReleaseResource(xStack_10)
            return
        end
        -- TODO(native): SetCutsceneSkippableWhilePaused is not a ForgeFSE binding
        quest:SetCutsceneSkippableWhilePaused(false)
    end
    quest:FixMovieSequenceCamera(false)
    quest:SetStateBool("CutscenePlaying", false)
    -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)(this + 0xb4));
    resources:DestroyActorMap(xStack_1c)
    resources:ReleaseResource(xStack_10)
end

function helper_E3E6B0(quest)
    local bVar1
    if quest:GetStateBool("HeroTricking") then
        bVar1 = helper_E3E320(quest)
        if bVar1 then
            return "_HEROWHORE"
        end
    end
    bVar1 = helper_E3E320(quest)
    if bVar1 then
        return "_HEROLADY"
    end
    return "_HEROMAN"
end

