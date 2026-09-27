-- Readable native conversion: CS_OakValeRevisited. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- CS_OakValeRevisited.Main (retail 0x00ee82d0)
function Main(quest, param2, param3, param4, param5)
    quest:FinalizeEntityBindings()
    local isRegionLoaded = quest:IsRegionLoaded("OakBay")
    while true do
        if isRegionLoaded then
            if quest:IsActiveThreadTerminating() then return end
            -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)0x64,(CFontBank *)ppVar3,unaff_ESI,unaff_EBX,pCVar4, unaff_retaddr,native_arg_param_2,native_arg_param_3,native_arg_param_4,native_arg_param_5);
            helper_EE8390(quest)
            return
        end
        if not quest:NewScriptFrame() then break end
        isRegionLoaded = quest:IsRegionLoaded("OakBay")
    end
end

-- CS_OakValeRevisited.OakValeFire (retail 0x00ee8870)
function OakValeFire(quest)
    local scratchValue, createEffectAtPos, scratchValue4, scratchValue8, scratchValue9, this_00
    local scratchValue11, scratchValue12
    -- TODO(native): pCVar8 = std::map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> ::operator[]((map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> *)(this + 0x60),local_28);
    -- TODO(native): local CVar3 = *pCVar8
    while scratchValue ~= 1 do
        if not quest:NewScriptFrame() then return end
        -- TODO(native): pCVar8 = std::map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> ::operator[]((map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> *)(this + 0x60),local_28);
        -- TODO(native): CVar3 = *pCVar8
        scratchValue = nil --[[unresolved native value]]
    end
    if quest:IsActiveThreadTerminating() then return end
    local getAllThingsWithScriptName = quest:GetAllThingsWithScriptName("Q_REVISITED_FIREPOINT")
    -- TODO(native): iVar14 = *pCVar1
--[[unresolved native value]]
    -- TODO(native): this_00 = (vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> *)(this + 0x54);
    -- TODO(native): std::vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> ::resize(this_00,(iVar10 - iVar14) / 0xc);
    local scratchValue3 = (quest:GetStateListCount("FirePoint") * 12) - *getAllThingsWithScriptName >> 31
    scratchValue12 = 0
    scratchValue8 = this
    if ((quest:GetStateListCount("FirePoint") * 12) - *getAllThingsWithScriptName) / 12 + scratchValue3 ~= scratchValue3 then
        scratchValue4 = 0
        repeat
            if quest:IsActiveThreadTerminating() then return end
            -- TODO(native): iVar4 = *this_00
    --[[unresolved native value]]
            -- TODO(native): uVar9 = (**(*(*pCVar1 + iVar14) + 0x18))("FirePoints",0,0,0)
--[[unresolved native value]]
            createEffectAtPos = quest:CreateEffectAtPos("OAKVALE_BURNING_PATCH", nil --[[missing]])
            -- TODO(native): piVar5 = *(iVar10 + 8)
    --[[unresolved native value]]
            -- TODO(native): u_stk_18 = *(iVar10 + 4)
--[[unresolved native value]]
            local scratchValue10 = scratchValue4 + 8 + nil
            -- TODO(native): piVar6 = *piVar2
    --[[unresolved native value]]
            if nil == nil then scratchValue4 = scratchValue4 + 12; scratchValue12 = scratchValue12 + 1; scratchValue8 = scratchValue9; goto continue_1 end
            if nil ~= nil then
                -- TODO(native): *piVar6 = *piVar6 - 1;
                if **scratchValue10 == 0 then
                    -- TODO(native): (*(code *)((int *)*piVar2)[1])();
                end
            end
            -- TODO(native): *(undefined4 *)(iVar14 + 4 + iVar4) = u_stk_18;
            -- TODO(native): *piVar2 = (int)piVar5;
            if nil ~= nil then
                -- TODO(native): *piVar5 = *piVar5 + 1;
            end
            scratchValue4 = scratchValue4 + 12
            scratchValue12 = scratchValue12 + 1
            scratchValue8 = scratchValue9
            ::continue_1::
        until scratchValue12 >= (((quest:GetStateListCount("FirePoint") * 12) - *getAllThingsWithScriptName) / 12)
    end
    if quest:IsActiveThreadTerminating() then return end
    -- TODO(native): pCVar8 = std::map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> ::operator[]((map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> *)(pCVar11 + 0x60),(CCharString *)&stack0xffffffd0);
    -- TODO(native): CVar3 = *pCVar8
    while nil --[[unresolved native value]] ~= nil do
        -- TODO(native): (**(code **)(*(int *)(pCVar11 + 0x3c) + 0x1c))();
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): pCVar8 = std::map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> ::operator[]((map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> *)(pCVar11 + 0x60),(CCharString *)&stack0xffffffd0);
        -- TODO(native): CVar3 = *pCVar8
    end
    if quest:IsActiveThreadTerminating() then return end
    -- TODO(native): iVar14 = *pCVar1
--[[unresolved native value]]
    -- TODO(native): std::vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> ::resize(this_00,(iVar10 - iVar14) / 0xc);
    local scratchValue5 = (quest:GetStateListCount("Fires") * 12) - *this_00 >> 31
    scratchValue11 = 0
    if not (((quest:GetStateListCount("Fires") * 12) - *this_00) / 12 + scratchValue5 ~= scratchValue5) then return end
    repeat
        if quest:IsActiveThreadTerminating() then return end
        -- TODO(native): (**(code **)(*(int *)(pCVar11 + 0x3c) + 0x1b0))(*(int *)this_00 + iVar14,0,1);
        scratchValue11 = scratchValue11 + 1
    until scratchValue11 >= (((quest:GetStateListCount("Fires") * 12) - *this_00) / 12)
end

-- CS_OakValeRevisited.helper_EE8390 (retail 0x00ee8390)
function helper_EE8390(quest)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local scratchValue2, pppuVar12, pppuVar, scratchValue3, this_01, scratchValue4
    quest:CreateThread("OakValeFire")  -- native thread body NScript::CCS_OakValeRevisitedScript::OakValeFire: lift it as function OakValeFire(quest)
    local isActiveThreadTerminating = not isActiveThreadTerminating
    -- TODO(native): local_44 = (int *)(uint)bVar9;
    local ovHeroStartMarker = quest:GetThingWithScriptName("OV_HERO_START_MARKER")
    quest:EntityTeleportToThing(hero, ovHeroStartMarker)
    local movie = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    local resource2 = resources:NewResource()
    local actorMap = resources:NewActorMap()
    local resource3 = resource2
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    -- TODO(native): this_01 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_64,(CCharString *)&stack0xffffff94);
    -- TODO(native): local ppuVar2 = *(this_01 + 0xc)
    if scratchValue3 ~= resource2 then
        local resource = resource2
        if scratchValue3 ~= nil then
            -- TODO(native): *ppuVar2 = *ppuVar2 - 1;
            if **(this_01 + 12) == 0 then
                -- TODO(native): (*(code *)(*(int **)(this_01 + 0xc))[1])();
            end
        end
        -- TODO(native): *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(this_01 + 8) = ppVar13;
        -- TODO(native): *(undefined ***)(this_01 + 0xc) = ppuVar14;
        if resource ~= nil then
            -- TODO(native): *ppuVar14 = *ppuVar14 + 1;
        end
    end
    local scratchValue5 = this + 96
    -- TODO(native): RunCutsceneMacro_Func();
    if nil ~= nil then
        -- TODO(native): StdMap_DestroyNode(*(undefined4 *)pvStack_74[(4) / 0xc + 1]);
        -- TODO(native): *(void **)pvStack_74[(8) / 0xc + 1] = pvStack_74;
        -- TODO(native): *(undefined4 *)pvStack_74[(4) / 0xc + 1] = 0;
        -- TODO(native): *(void **)pvStack_74[(0xc) / 0xc + 1] = pvStack_74;
    end
    quest:PauseAllNonScriptedEntities(false)
    scratchValue2 = 0
    local getStateListCount = quest:GetStateListCount("FirePoint") * 12
    -- TODO(native): this = (vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> *)(this + 0x54);
    local fence = nil
    -- TODO(native): std::vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> ::resize(this,(iVar3 - iVar8) / 0xc);
    scratchValue2 = (quest:GetStateListCount("Fires") * 12) - *this >> 31
    scratchValue4 = 0
    if ((quest:GetStateListCount("Fires") * 12) - *this) / 12 + scratchValue2 ~= scratchValue2 then
        scratchValue2 = 0
        repeat
            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating then
                return
            end
            -- TODO(native): quest:RemoveThing(ppVar6, *this + iVar8, false)
            scratchValue4 = scratchValue4 + 1
            scratchValue2 = scratchValue2 + 12
        until scratchValue4 >= (((quest:GetStateListCount("Fires") * 12) - *this) / 12)
    end
    isActiveThreadTerminating = quest:IsActiveThreadTerminating()
    if not isActiveThreadTerminating then
        quest:DeactivateQuestLater("CS_OakValeRevisited", 0)
        fence = quest:GetAllThingsWithScriptName("Fence")
        scratchValue2 = pppuVar12 - "CS_OakValeRevisited" >> 31
        scratchValue4 = 0
        pppuVar = "CS_OakValeRevisited"
        if (pppuVar12 - "CS_OakValeRevisited") / 12 + scratchValue2 ~= scratchValue2 then
            scratchValue2 = 0
            repeat
                -- TODO(native): cVar5 = (**(*("CS_OakValeRevisited" + iVar8) + 300))()
                local scratchValue = nil --[[unresolved native value]]
                if scratchValue then
                    quest:RemoveThing(nil --[[missing]], "CS_OakValeRevisited" + scratchValue2, false)
                end
                scratchValue4 = scratchValue4 + 1
                scratchValue2 = scratchValue2 + 12
                pppuVar = "CS_OakValeRevisited"
            until scratchValue4 >= ((pppuVar12 - "CS_OakValeRevisited") / 12)
        end
        -- TODO(native): for (; pppuVar4 != pppuVar12; pppuVar4 = pppuVar4 + 3) {
        -- TODO(native): (*(code *)**pppuVar4)(0);
    end
    end
end

