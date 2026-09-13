-- Generated native draft: NOVI_Victim. Review coverage report before use.
-- Not copied from the working port; registration remains disabled.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateBool("DoneThanks", false)
    __native_entity_state:SetStateBool("DisplayedGameInfo", false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetThingHasInformation(me, false, false, false)
    quest:SetIsPushableByHero(me, false)
    quest:EntitySetAsUseMovementInActions(me, false)
    quest:EntitySetAsScared(me, true)
end

function Main(quest, me)
    local CVar19, bVar6, cVar7, fVar13, fVar4, pCVar1, pCVar15, pCVar16, pCVar17, pCVar22, pCVar8, pcVar14, ppVar18, ppVar20, r1, r10, r11, r12, r13, r2, r3, r4, r5, r6, r7, r8, r9, uVar12, uVar21, uVar5
    local alive = true
    -- TODO(native): map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> amStack_70 [12];
    -- TODO(native): CCarriedReadableDef aCStack_64 [16];
    -- TODO(native): C3DClothPrimitive aCStack_54 [12];
    -- TODO(native): C3DClothPrimitive aCStack_48 [24];
    -- TODO(native): C3DClothPrimitive aCStack_30 [12];
    -- TODO(native): C3DClothPrimitive aCStack_24 [36];
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_114);
    if bVar6 then
    end
    -- TODO(native): ppVar24 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar1;
    cVar7 = me:AcquireControl(4)
    while not cVar7 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00dbde21 end
        cVar7 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00dbde21 end
    r1 = quest:GetThingWithScriptName("NOVI_Bully")
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        repeat
            if not quest:GetStateBool("BullySubdued") then
                if quest:GetStateBool("VictimShake") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    quest:SetStateBool("VictimShake", false)
                    quest:EntitySetFacingAngleTowardsThing(r1, nil --[[missing]])
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                if not __native_entity_state:GetStateBool("DoneThanks") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    __native_entity_state:SetStateBool("DoneThanks", true)
                    -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffee0);
                    if bVar6 then
                    end
                    quest:EntitySetAsScared(nil --[[missing]], false)
                    cVar7 = quest:GetStateBool("BullyRanOff")
                    while not cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00dbde18 end
                        cVar7 = quest:GetStateBool("BullyRanOff")
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    quest:SetIsPushableByHero(nil --[[missing]], false)
                    quest:EntitySetAsUseMovementInActions(nil --[[missing]], false)
                    quest:ClearThingHasInformation(nil --[[missing]])
                end
            end
            cVar7 = me:IsTalkedToByHero()
            if cVar7 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                if not quest:GetStateBool("BullySubdued") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    quest:EntitySetAsScared(nil --[[missing]], nil --[[missing]])
                    r2 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(r2, nil --[[missing]])
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_a4);
                    pCVar15 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffec0);
                    if bVar6 then
                    end
                    cVar7 = me:AcquireControl(4)
                    uVar21 = SUB41(me,0)
                    while not cVar7 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dbd3e3
                        cVar7 = me:AcquireControl(4)
                        uVar21 = SUB41(me,0)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        -- LAB_00dbddc1: (native jump target)
                        quest:PauseAllNonScriptedEntities(false)
                        break
                    end
                    if not quest:GetStateBool("HeroAttackedVictim") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dbddc1
                        -- TODO(native): uVar9 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeb4);
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        fVar4 = _DAT_0122dedc
                        if fVar4 < fVar13 then
                            bVar6 = false
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_048_VICTIM_PLEA"
                            pCVar8 = quest:GetHero()
                            r3 = me:Speak(pCVar8, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dbd3e3
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbddc1
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00dbd3e3: (native jump target)
                            quest:PauseAllNonScriptedEntities(false)
                            break
                        end
                        -- TODO(native): uVar9 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffeb4);
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        fVar4 = _DAT_0122dedc
                        if fVar4 < fVar13 then
                            bVar6 = false
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_048_VICTIM_PLEA_AFTER_ATTACK"
                            pCVar8 = quest:GetHero()
                            r4 = me:Speak(pCVar8, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dbddc1
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbd3e3
                        end
                    end
                    quest:EntitySetAsScared(me, true)
                    quest:EntitySetFacingAngleTowardsThing(me, nil --[[missing]], false)
                    quest:PauseAllNonScriptedEntities(false)
                    -- TODO(native): this = aCStack_90;
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_a0);
                    pCVar15 = ""
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                    -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&pppuStack_12c);
                    if bVar6 then
                    end
                    cVar7 = me:AcquireControl(4)
                    while true do
                        uVar21 = SUB41(me,0)
                        if not (not cVar7) then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dbd148
                        cVar7 = me:AcquireControl(4)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        -- LAB_00dbdda6: (native jump target)
                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                        break
                    end
                    if not quest:GetStateBool("HeroAttackedVictim") then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00dbdda6
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec8);
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        fVar4 = _DAT_0122dedc
                        if fVar4 < fVar13 then
                            CVar19 = 0x0
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_048_VICTIM_THANKS"
                            pCVar8 = quest:GetHero()
                            r5 = me:Speak(pCVar8, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), (CVar19 ~= 0))
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dbd148
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbdda6
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            -- LAB_00dbd148: (native jump target)
                            quest:PauseAllNonScriptedEntities((CVar19 ~= 0))
                            break
                        end
                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec8);
                        fVar13 = quest:GetHealth(nil --[[missing]])
                        fVar4 = _DAT_0122dedc
                        if fVar4 < fVar13 then
                            CVar19 = 0x0
                            pCVar17 = 0x1
                            pCVar16 = 0x0
                            pCVar15 = 0x0
                            pcVar14 = "TEXT_QST_048_VICTIM_THANKS_AFTER_HIT"
                            pCVar8 = quest:GetHero()
                            r6 = me:Speak(pCVar8, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), (CVar19 ~= 0))
                            bVar6 = me:IsPerformingScriptTask()
                            if bVar6 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00dbdda6
                                    bVar6 = me:IsPerformingScriptTask()
                                until not (bVar6)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00dbd148
                        end
                    end
                    quest:PauseAllNonScriptedEntities((CVar19 ~= 0))
                    -- TODO(native): this = (CScriptGameResourceObjectMovieBase *)aCStack_a0;
                end
                if not __native_entity_state:GetStateBool("DisplayedGameInfo") then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    cVar7 = quest:IsXbox()
                    if not cVar7 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then break end
                        pCVar15 = "TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS_PC"
                        quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS_PC")
                        cVar7 = quest:MsgIsGameInfoClickedPast()
                        while not cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00dbde18 end
                            cVar7 = quest:MsgIsGameInfoClickedPast()
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then break end
                        pCVar15 = "TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS"
                        quest:DisplayGameInfo("TEXT_QST_048_INSTRUCTION_HITTING_FRIENDS")
                        cVar7 = quest:MsgIsGameInfoClickedPast()
                        while not cVar7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00dbde18 end
                            cVar7 = quest:MsgIsGameInfoClickedPast()
                        end
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    __native_entity_state:SetStateBool("DisplayedGameInfo", true)
                end
            end
            uVar12 = uStack_100 | 1
            cVar7 = me:MsgIsHitByHero()
            if not cVar7 then
                uVar12 = uVar5 | 3
                cVar7 = me:MsgIsHitByAnySpecialAbilityFromHero()
                if cVar7 then
                    uVar12 = uVar5 | 7
                    cVar7 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                    if not cVar7 then return end  -- TODO(native): goto LAB_00dbd69d
                end
                -- TODO(native): cStack_101 = '\0';
            else
                -- LAB_00dbd69d: (native jump target)
                -- TODO(native): cStack_101 = '\x01';
            end
            if (uVar12 & 4) ~= 0 then
                uVar12 = uVar12 & 0xfffffffb
            end
            if (uVar12 & 2) ~= 0 then
                uVar12 = uVar12 & 0xfffffffd
            end
            if (uVar12 & 1) ~= 0 then
                -- TODO(native): uStack_100 = uVar12 & 0xfffffffe;
            end
            if cStack_101 ~= 0 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    r7 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(r7, nil --[[missing]])
                    r8 = quest:GetHero()
                    quest:EntitySetThingAsAllyOfThing(r8, nil --[[missing]])
                    require("NewOakValeIntro.native_quest_helpers").AddBadDeed(quest, me, 2)
                    quest:SetStateBool("HeroAttackedVictim", true)
                    if not quest:GetStateBool("GivenHeroTeddy") then
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            quest:SetStateBool("GivenHeroTeddy", true)
                            -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffee0);
                            if bVar6 then
                            end
                            cVar7 = me:AcquireControl(4)
                            while not cVar7 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00dbde18 end
                                cVar7 = me:AcquireControl(4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_80);
                                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_80);
                                if bVar6 then
                                end
                                r9 = quest:GetHero()
                                cVar7 = me:AcquireControl(4)
                                while not cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00dbdddc end
                                    r10 = quest:GetHero()
                                    cVar7 = me:AcquireControl(4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    -- TODO(native): StdMap_Construct_API();
                                    -- TODO(native): pCVar10 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_70,aCStack_cc);
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar10, (CScriptGameResourceObjectScriptedThingBase *)pCVar22);
                                    -- TODO(native): pCVar10 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_70,aCStack_a8);
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar10,pCVar23);
                                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_64);
                                    pCVar15 = ""
                                    quest:StartMovieSequence()
                                    quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                    quest:FixMovieSequenceCamera(nil --[[missing]])
                                    ppVar18 = 0x0
                                    -- TODO(native): RunCutsceneMacro_Func();
                                    quest:FixMovieSequenceCamera(false)
                                    quest:ClearThingHasInformation(me)
                                    quest:EntitySetFacingAngleTowardsThing(me, r10, false)
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- TODO(native): StdMap_Destroy_API();
                                    goto LAB_00dbdc58
                                end
                                ::LAB_00dbdddc::
                            end
                        end
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            ppVar18 = quest:AddNewConversation(r9, (ppVar18 ~= 0), nil --[[missing]])
                            r11 = quest:GetThingWithScriptName("NOVI_Bully")
                            bVar6 = aCStack_f8:IsAlive()
                            if not bVar6 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00dbde0f end
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_e0);
                                pCVar8 = ""
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&pppuStack_12c);
                                if bVar6 then
                                end
                                cVar7 = me:AcquireControl(4)
                                uVar21 = SUB41(me,0)
                                while not cVar7 do
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00dbddea end
                                    cVar7 = me:AcquireControl(4)
                                    uVar21 = SUB41(me,0)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if alive then
                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec8);
                                    fVar13 = quest:GetHealth(r11)
                                    fVar4 = _DAT_0122dedc
                                    if fVar4 < fVar13 then
                                        CVar19 = 0x0
                                        pCVar17 = 0x1
                                        pCVar16 = 0x0
                                        pCVar15 = 0x2
                                        pcVar14 = "TEXT_QST_048_VICTIM_EVIL_BROS"
                                        pCVar8 = quest:GetHero()
                                        r12 = me:Speak(pCVar8, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), (CVar19 ~= 0))
                                        bVar6 = me:IsPerformingScriptTask()
                                        if bVar6 then
                                            repeat
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then goto LAB_00dbddea end
                                                bVar6 = me:IsPerformingScriptTask()
                                            until not (bVar6)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00dbddf9 end
                                    end
                                    -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffec8);
                                    if bVar6 then
                                    end
                                    quest:PauseAllNonScriptedEntities((CVar19 ~= 0))
                                    -- TODO(native): goto LAB_00dbdc4f
                                end
                                ::LAB_00dbddf9::
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                goto LAB_00dbde06
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                quest:AddPersonToConversation(nil --[[missing]], nil --[[missing]])
                                ppVar20 = ppVar18
                                quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_048_VICTIM_EVIL_BROS", nil --[[missing]], nil --[[missing]])
                                quest:AddLineToConversation(ppVar18, "TEXT_QST_048_BULLY_HERO_ATTACKS_VICTIM", me, nil --[[missing]], false)
                                -- LAB_00dbdc4f: (native jump target)
                                goto LAB_00dbdc58
                            end
                            goto LAB_00dbde0f
                        end
                    end
                end
                break
            end
            ::LAB_00dbdc58::
            if quest:GetStateBool("VictimComplainsAboutLosingTeddy") then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                -- TODO(native): bVar6 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffee0);
                if bVar6 then
                end
                cVar7 = me:AcquireControl(4)
                while not cVar7 do
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00dbde18 end
                    cVar7 = me:AcquireControl(4)
                end
                alive = not quest:IsActiveThreadTerminating()
                if not alive then break end
                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffee0);
                fVar13 = quest:GetHealth(nil --[[missing]])
                fVar4 = _DAT_0122dedc
                if fVar4 < fVar13 then
                    bVar6 = false
                    pCVar17 = 0x1
                    pCVar16 = 0x0
                    pCVar15 = 0x0
                    pcVar14 = "TEXT_QST_048_VICTIM_EVIL_BROS_10"
                    pCVar8 = quest:GetHero()
                    r13 = me:Speak(pCVar8, pcVar14, pCVar15, (pCVar16 ~= 0), (pCVar17 ~= 0), bVar6)
                    bVar6 = me:IsPerformingScriptTask()
                    if bVar6 then
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00dbde18 end
                            bVar6 = me:IsPerformingScriptTask()
                        until not (bVar6)
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                end
                quest:SetStateBool("VictimComplainsAboutLosingTeddy", false)
            end
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then break end
        until false
    end
    goto LAB_00dbde18
    ::LAB_00dbddea::
    quest:PauseAllNonScriptedEntities(nil --[[missing]])
    ::LAB_00dbde06::
    ::LAB_00dbde0f::
    ::LAB_00dbde18::
    ::LAB_00dbde21::
end

