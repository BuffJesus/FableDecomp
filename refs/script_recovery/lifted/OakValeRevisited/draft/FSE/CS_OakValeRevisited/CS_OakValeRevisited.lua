-- Generated native draft: CS_OakValeRevisited. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest, native_arg_param_2, native_arg_param_3, native_arg_param_4, native_arg_param_5)
    local bVar2
    local alive = true
    quest:FinalizeEntityBindings()
    local cVar1 = quest:IsRegionLoaded("OakBay")
    while true do
        if cVar1 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if not bVar2 then
                -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)0x64,(CFontBank *)ppVar3,unaff_ESI,unaff_EBX,pCVar4, unaff_retaddr,native_arg_param_2,native_arg_param_3,native_arg_param_4,native_arg_param_5);
                helper_EE8390(quest)
            end
            return
        end
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then break end
        cVar1 = quest:IsRegionLoaded("OakBay")
    end
end

function OakValeFire(quest)
    local CVar3, bVar7, iVar10, iVar14, iVar4, local_14, pCVar1, pCVar11, pCVar13, pC_stk_1c, piVar2, piVar5, piVar6, this_00, uVar12, uVar9, u_stk_18, u_stk_20
    local alive = true
    -- TODO(native): pCVar8 = std::map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> ::operator[]((map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> *)(this + 0x60),local_28);
    -- TODO(native): local CVar3 = *pCVar8
    while CVar3 ~= 0x1 do
        alive = quest:NewScriptFrame()
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if bVar7 then
            return
        end
        -- TODO(native): pCVar8 = std::map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> ::operator[]((map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> *)(this + 0x60),local_28);
        -- TODO(native): CVar3 = *pCVar8
        CVar3 = nil --[[unresolved native value]]
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar7 = not alive
    if not bVar7 then
        pCVar1 = this + 0x48
        pCVar13 = "Q_REVISITED_FIREPOINT"
        pCVar1 = quest:GetAllThingsWithScriptName(pCVar13)
        -- TODO(native): iVar14 = *pCVar1
        iVar14 = nil --[[unresolved native value]]
        iVar10 = (quest:GetStateListCount("FirePoint") * 0xc)
        -- TODO(native): this_00 = (vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> *)(this + 0x54);
        local_14 = nil
        -- TODO(native): std::vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> ::resize(this_00,(iVar10 - iVar14) / 0xc);
        iVar14 = (quest:GetStateListCount("FirePoint") * 0xc) - *pCVar1 >> 0x1f
        u_stk_20 = 0
        pCVar11 = this
        if ((quest:GetStateListCount("FirePoint") * 0xc) - *pCVar1) / 0xc + iVar14 ~= iVar14 then
            iVar14 = 0
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar7 = not alive
                if bVar7 then
                    return
                end
                iVar10 = **(pCVar11 + 0x40)
                -- TODO(native): iVar4 = *this_00
                iVar4 = nil --[[unresolved native value]]
                -- TODO(native): uVar9 = (**(*(*pCVar1 + iVar14) + 0x18))("FirePoints",0,0,0)
                uVar9 = nil --[[unresolved native value]]
                iVar10 = quest:CreateEffectAtPos("OAKVALE_BURNING_PATCH", nil --[[missing]])
                -- TODO(native): piVar5 = *(iVar10 + 8)
                piVar5 = nil --[[unresolved native value]]
                -- TODO(native): u_stk_18 = *(iVar10 + 4)
                u_stk_18 = nil --[[unresolved native value]]
                piVar2 = (iVar14 + 8 + iVar4)
                -- TODO(native): piVar6 = *piVar2
                piVar6 = nil --[[unresolved native value]]
                if piVar6 ~= piVar5 then
                    if piVar6 ~= nil then
                        -- TODO(native): *piVar6 = *piVar6 + -1;
                        if **piVar2 == 0 then
                            -- TODO(native): (*(code *)((int *)*piVar2)[1])();
                        end
                    end
                    -- TODO(native): *(undefined4 *)(iVar14 + 4 + iVar4) = u_stk_18;
                    -- TODO(native): *piVar2 = (int)piVar5;
                    if piVar5 ~= nil then
                        -- TODO(native): *piVar5 = *piVar5 + 1;
                    end
                end
                iVar14 = iVar14 + 0xc
                u_stk_20 = u_stk_20 + 1
                pCVar11 = pC_stk_1c
            until not (u_stk_20 < (((quest:GetStateListCount("FirePoint") * 0xc) - *pCVar1) / 0xc))
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar7 = not alive
        if not bVar7 then
            -- TODO(native): pCVar8 = std::map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> ::operator[]((map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> *)(pCVar11 + 0x60),(CCharString *)&stack0xffffffd0);
            -- TODO(native): CVar3 = *pCVar8
            CVar3 = nil --[[unresolved native value]]
            while CVar3 ~= nil do
                -- TODO(native): (**(code **)(*(int *)(pCVar11 + 0x3c) + 0x1c))();
                alive = not quest:IsActiveThreadTerminating()
                bVar7 = not alive
                if bVar7 then
                    return
                end
                -- TODO(native): pCVar8 = std::map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> ::operator[]((map<CCharString,C2DVector,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,C2DVector>_>_> *)(pCVar11 + 0x60),(CCharString *)&stack0xffffffd0);
                -- TODO(native): CVar3 = *pCVar8
                CVar3 = nil --[[unresolved native value]]
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar7 = not alive
            if not bVar7 then
                -- TODO(native): iVar14 = *pCVar1
                iVar14 = nil --[[unresolved native value]]
                iVar10 = (quest:GetStateListCount("FirePoint") * 0xc)
                iVar10 = nil
                -- TODO(native): std::vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> ::resize(this_00,(iVar10 - iVar14) / 0xc);
                iVar14 = (quest:GetStateListCount("Fires") * 0xc) - *this_00 >> 0x1f
                uVar12 = 0
                if ((quest:GetStateListCount("Fires") * 0xc) - *this_00) / 0xc + iVar14 ~= iVar14 then
                    iVar14 = 0
                    repeat
                        alive = not quest:IsActiveThreadTerminating()
                        bVar7 = not alive
                        if bVar7 then
                            return
                        end
                        -- TODO(native): (**(code **)(*(int *)(pCVar11 + 0x3c) + 0x1b0))(*(int *)this_00 + iVar14,0,1);
                        uVar12 = uVar12 + 1
                        iVar14 = iVar14 + 0xc
                    until not (uVar12 < (((quest:GetStateListCount("Fires") * 0xc) - *this_00) / 0xc))
                end
                alive = not quest:IsActiveThreadTerminating()
            end
        end
    end
end

function helper_EE8390(quest)
    local resources = quest:RetailResources()
    local cVar5, iVar8, pppuVar12, pppuVar4, ppuVar14, ppuVar2, this_01, uVar11
    local alive = true
    quest:CreateThread("OakValeFire")  -- native thread body NScript::CCS_OakValeRevisitedScript::OakValeFire: lift it as function OakValeFire(quest)
    local bVar9 = not bVar9
    -- TODO(native): local_44 = (int *)(uint)bVar9;
    if bVar9 then
    end
    local uStack_64 = quest:GetThingWithScriptName("OV_HERO_START_MARKER")
    local ppVar6 = quest:GetHero()
    quest:EntityTeleportToThing(ppVar6, uStack_64)
    local appu_stk_24 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    local local_40 = resources:NewResource()
    local puVar7 = resources:NewActorMap()
    local pppuStack_7c = local_40
    ppVar6 = quest:GetHero()
    -- TODO(native): StartScriptingEntity: unresolved entity receiver/resource in quest context; arguments: 
    -- TODO(native): this_01 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)&uStack_64,(CCharString *)&stack0xffffff94);
    -- TODO(native): local ppuVar2 = *(this_01 + 0xc)
    if ppuVar2 ~= local_40 then
        ppuVar14 = local_40
        if ppuVar2 ~= nil then
            -- TODO(native): *ppuVar2 = *ppuVar2 + -1;
            if **(this_01 + 0xc) == 0 then
                -- TODO(native): (*(code *)(*(int **)(this_01 + 0xc))[1])();
            end
        end
        -- TODO(native): *(pair<EHeroMorphType,CParticleMorphs::CEntry> **)(this_01 + 8) = ppVar13;
        -- TODO(native): *(undefined ***)(this_01 + 0xc) = ppuVar14;
        if ppuVar14 ~= nil then
            -- TODO(native): *ppuVar14 = *ppuVar14 + 1;
        end
    end
    local pCVar10 = this + 0x60
    -- TODO(native): RunCutsceneMacro_Func();
    if nil ~= nil then
        -- TODO(native): StdMap_DestroyNode(*(undefined4 *)pvStack_74[(4) / 0xc + 1]);
        -- TODO(native): *(void **)pvStack_74[(8) / 0xc + 1] = pvStack_74;
        -- TODO(native): *(undefined4 *)pvStack_74[(4) / 0xc + 1] = 0;
        -- TODO(native): *(void **)pvStack_74[(0xc) / 0xc + 1] = pvStack_74;
    end
    quest:PauseAllNonScriptedEntities(false)
    iVar8 = 0
    local iVar3 = (quest:GetStateListCount("FirePoint") * 0xc)
    -- TODO(native): this = (vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> *)(this + 0x54);
    local pvStack_74 = nil
    -- TODO(native): std::vector<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>,std::allocator<CVectorMap<unsigned_long,CRandomAppearanceMorph::CTextureMorph,CKeyPairCompareLess<unsigned_long,CRandomAppearanceMorph::CTextureMorph>_>_>_> ::resize(this,(iVar3 - iVar8) / 0xc);
    iVar8 = (quest:GetStateListCount("Fires") * 0xc) - *this >> 0x1f
    uVar11 = 0
    if ((quest:GetStateListCount("Fires") * 0xc) - *this) / 0xc + iVar8 ~= iVar8 then
        iVar8 = 0
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar9 = not alive
            if bVar9 then
                return
            end
            -- TODO(native): quest:RemoveThing(ppVar6, *this + iVar8, false)
            uVar11 = uVar11 + 1
            iVar8 = iVar8 + 0xc
        until not (uVar11 < (((quest:GetStateListCount("Fires") * 0xc) - *this) / 0xc))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar9 = not alive
    if not bVar9 then
        quest:DeactivateQuestLater("CS_OakValeRevisited", 0)
        pvStack_74 = quest:GetAllThingsWithScriptName("Fence")
        iVar8 = pppuVar12 - "CS_OakValeRevisited" >> 0x1f
        uVar11 = 0
        pppuVar4 = "CS_OakValeRevisited"
        if (pppuVar12 - "CS_OakValeRevisited") / 0xc + iVar8 ~= iVar8 then
            iVar8 = 0
            repeat
                -- TODO(native): cVar5 = (**(*("CS_OakValeRevisited" + iVar8) + 300))()
                cVar5 = nil --[[unresolved native value]]
                if cVar5 then
                    quest:RemoveThing(nil --[[missing]], ("CS_OakValeRevisited" + iVar8), false)
                end
                uVar11 = uVar11 + 1
                iVar8 = iVar8 + 0xc
                pppuVar4 = "CS_OakValeRevisited"
            until not (uVar11 < ((pppuVar12 - "CS_OakValeRevisited") / 0xc))
        end
        -- TODO(native): for (; pppuVar4 != pppuVar12; pppuVar4 = pppuVar4 + 3) {
        -- TODO(native): (*(code *)**pppuVar4)(0);
    end
    end
end

