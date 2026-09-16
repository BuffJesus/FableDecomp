-- Generated native draft: MeleeApprentice. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local __native_entity_state = {}
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        __native_entity_state["GetState" .. kind] = function(_, name) return fields[name] end
        __native_entity_state["SetState" .. kind] = function(_, name, value) fields[name] = value end
    end
end

function Main(quest, me)
    local bVar4, fVar10, fVar3, pCVar12, pCVar15, pCVar16, pCVar18, pCVar8, pCVar9, pcVar14, piVar7, r2, r3, r4, r5, r6
    local alive = true
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_f8);
    if bVar4 then
    end
    local cVar5 = me:AcquireControl(4)
    while not cVar5 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        cVar5 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    quest:EntitySheatheWeapons(me)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAsKillable(nil --[[missing]], false)
    quest:EntitySetAllowBossPhaseChanges(nil --[[missing]], false)
    me:SetFriendsWithEverythingFlag(nil --[[missing]])
    local r1 = quest:GetThingWithScriptName("MeleeApprenticeMarker")
    alive = not quest:IsActiveThreadTerminating()
    cVar5 = not alive
    repeat
        if cVar5 then
            if (pCStack_11c ~= nil) and (*pCStack_11c = *pCStack_11c + -1, *pCStack_11c == 0) then
                -- TODO(native): (**(code **)(pCStack_11c + 4))();
            end
            -- TODO(native): iRam00000001 = iRam00000001 + -1;
            if iRam00000001 == 0 then
                -- TODO(native): (*_DAT_00000005)();
            end
            return
        end
        if quest:GetMasterGameState("MeleeApprenticeNeededForCutscene") ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d41a07
            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffecc);
            if bVar4 then
            end
            cVar5 = quest:GetMasterGameState("MeleeApprenticeNeededForCutscene")
            while cVar5 ~= 0 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d41a07
                cVar5 = quest:GetMasterGameState("MeleeApprenticeNeededForCutscene")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d41a07
            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffecc);
            if bVar4 then
            end
            cVar5 = me:AcquireControl(4)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d41a07
                cVar5 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d41a07
        end
        if quest:GetStateBool("StartedMeleeTesting") then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d41a07
            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffecc);
            if bVar4 then
            end
            cVar5 = quest:GetStateBool("StartedMeleeTesting")
            while cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d41a07
                cVar5 = quest:GetStateBool("StartedMeleeTesting")
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d41a07
            -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffecc);
            if bVar4 then
            end
            cVar5 = me:AcquireControl(4)
            while not cVar5 do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d41a07
                cVar5 = me:AcquireControl(4)
            end
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d41a07
            me:ClearCommands()
            quest:EntitySheatheWeapons(me, false)
        end
        cVar5 = quest:IsQuestActive("Q_GuildTrainingWoodsWill")
        alive = not quest:IsActiveThreadTerminating()
        if not cVar5 then
            if not alive then return end  -- TODO(native): goto LAB_00d41a07
            cVar5 = me:IsTalkedToByHero()
            if cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d41a07
                me:ClearCommands()
                cVar5 = quest:IsQuestActive("Q_GuildTrainingSkill")
                if not cVar5 then
                    cVar5 = quest:IsQuestActive("Q_GuildTrainingWill")
                    if cVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d41a07
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_c0);
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec0);
                        fVar10 = quest:GetHealth(r1)
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar10 then
                            bVar4 = false
                            pCVar15 = 0x1
                            pCVar12 = 0x0
                            pCVar16 = 0x0
                            pcVar14 = "TEXT_QST_028_TEEN_WHISPER_WILL_MOAN"
                            pCVar8 = quest:GetHero()
                            r2 = me:Speak(pCVar8, pcVar14, pCVar16, (pCVar12 ~= 0), (pCVar15 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        -- TODO(native): goto LAB_00d41a02
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                -- TODO(native): goto LAB_00d41a02
                            end
                        end
                        quest:PauseAllNonScriptedEntities(false)
                        -- TODO(native): goto LAB_00d41813
                    end
                    cVar5 = quest:IsQuestActive("Q_GuildTrainingDeparture")
                    if cVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d41a07
                        cVar5 = quest:IsQuestActive("Q_GuildTrainingWoodsDeparture")
                        alive = not quest:IsActiveThreadTerminating()
                        if not cVar5 then
                            if not alive then return end  -- TODO(native): goto LAB_00d41a07
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_e0);
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec0);
                            fVar10 = quest:GetHealth(nil --[[missing]])
                            fVar3 = _DAT_0122dedc
                            if fVar3 < fVar10 then
                                bVar4 = false
                                pCVar15 = 0x1
                                pCVar12 = 0x0
                                pCVar16 = 0x0
                                pcVar14 = "TEXT_QST_028_WHISPER_MELEE_MOAN"
                                pCVar8 = quest:GetHero()
                                r3 = me:Speak(pCVar8, pcVar14, pCVar16, (pCVar12 ~= 0), (pCVar15 ~= 0), bVar4)
                                bVar4 = me:IsPerformingScriptTask()
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- TODO(native): goto LAB_00d419fe
                                        end
                                        bVar4 = me:IsPerformingScriptTask()
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- LAB_00d419fe: (native jump target)
                                    -- TODO(native): goto LAB_00d41a02
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                        else
                            if not alive then return end  -- TODO(native): goto LAB_00d41a07
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_a0);
                            quest:StartMovieSequence()
                            quest:PauseAllNonScriptedEntities(true)
                            -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec0);
                            fVar10 = quest:GetHealth(nil --[[missing]])
                            fVar3 = _DAT_0122dedc
                            if fVar3 < fVar10 then
                                bVar4 = false
                                pCVar15 = 0x1
                                pCVar12 = 0x0
                                pCVar16 = 0x0
                                pcVar14 = "TEXT_QST_028_WHISPER_END_MOAN"
                                pCVar8 = quest:GetHero()
                                r4 = me:Speak(pCVar8, pcVar14, pCVar16, (pCVar12 ~= 0), (pCVar15 ~= 0), bVar4)
                                bVar4 = me:IsPerformingScriptTask()
                                if bVar4 then
                                    repeat
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            quest:PauseAllNonScriptedEntities(false)
                                            -- TODO(native): goto LAB_00d41a02
                                        end
                                        bVar4 = me:IsPerformingScriptTask()
                                    until not (bVar4)
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- LAB_00d41a02: (native jump target)
                                    -- TODO(native): goto LAB_00d41a07
                                end
                            end
                            quest:PauseAllNonScriptedEntities(false)
                        end
                        -- TODO(native): goto LAB_00d41813
                    end
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d41a07
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_d0);
                    quest:StartMovieSequence()
                    quest:PauseAllNonScriptedEntities(true)
                    -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&stack0xfffffec0);
                    fVar10 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar10 then
                        bVar4 = false
                        pCVar15 = 0x1
                        pCVar12 = 0x0
                        pCVar16 = 0x0
                        pcVar14 = "TEXT_QST_028_TEEN_WHISPER_SKILL_MOAN"
                        pCVar8 = quest:GetHero()
                        r5 = me:Speak(pCVar8, pcVar14, pCVar16, (pCVar12 ~= 0), (pCVar15 ~= 0), bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- TODO(native): goto LAB_00d41a02
                                end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00d41a02
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    -- LAB_00d41813: (native jump target)
                end
                if piVar26 == nil then
                else
                    pCVar8 = quest:IsXbox()
                end
                me:MoveToPosition(nil --[[missing]], 0x40400000, 0x1, false, true)
            end
            bVar4 = IsDistanceBetweenThingsOver(me,&stack0xfffffedc,4.0)
            local __native_condition_1 = bVar4
            if __native_condition_1 then
                bVar4 = me:IsPerformingScriptTask()
                __native_condition_1 = not bVar4
            end
            if __native_condition_1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    -- LAB_00d41a07: (native jump target)
                    return
                end
                if piVar26 == nil then
                else
                    pCVar8 = quest:IsXbox()
                end
                me:MoveToPosition(nil --[[missing]], 0x40400000, 0x1, false, true)
            end
        else
            if not alive then return end  -- TODO(native): goto LAB_00d41a07
            if not __native_entity_state:GetStateBool("WillWoodsChatDone") then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d41a07
                pCVar16 = "MK_GTM_WD_HEROWALK"
                piVar7 = quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK")
                pCVar15 = 0x1
                pcVar14 = 0x0
                pCVar8 = piVar7:GetPos()
                me:MoveToPosition(pCVar8, SUB41(ppVar19,0), SUB41(ppCVar20,0))
                __native_entity_state:SetStateBool("WillWoodsChatDone", true)
            else
                cVar5 = me:IsTalkedToByHero()
                if cVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then return end  -- TODO(native): goto LAB_00d41a07
                    me:ClearCommands()
                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_b0);
                    quest:StartMovieSequence()
                    pCVar16 = 0x1
                    quest:PauseAllNonScriptedEntities((pCVar16 ~= 0))
                    -- TODO(native): pcVar14 = (char *)CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffec0);
                    fVar10 = quest:GetHealth(nil --[[missing]])
                    fVar3 = _DAT_0122dedc
                    if fVar3 < fVar10 then
                        bVar4 = false
                        pCVar15 = 0x0
                        pCVar12 = 0x0
                        pCVar8 = quest:GetHero()
                        r6 = me:Speak(pCVar8, "TEXT_QST_028_WHISPER_SCORPION_WOODS", pCVar12, (pCVar15 ~= 0), true, bVar4)
                        bVar4 = me:IsPerformingScriptTask()
                        if bVar4 then
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then
                                    quest:PauseAllNonScriptedEntities(false)
                                    -- TODO(native): goto LAB_00d41a02
                                end
                                bVar4 = me:IsPerformingScriptTask()
                            until not (bVar4)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            quest:PauseAllNonScriptedEntities(false)
                            -- TODO(native): goto LAB_00d41a02
                        end
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    piVar7 = quest:GetThingWithScriptName("MK_GTM_WD_HEROWALK")
                    pCVar8 = piVar7:GetPos()
                    me:MoveToPosition(pCVar8, pCVar16, SUB41(ppVar19,0))
                end
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        cVar5 = not alive
    until false
end

function Init(quest, me)
    __native_entity_state:SetStateBool("WaitingForFight", true)
    __native_entity_state:SetStateBool("WillWoodsChatDone", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

