-- Generated native draft: BirdKiller. Review coverage report before use.
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
    local __native_condition_1, __native_condition_2, __native_condition_3, bVar4, cVar5, fVar15, fVar22, fVar3, iVar9, pCVar1, pCVar18, pCVar19, pCVar20, pCVar8, paVar12, pcVar16, piStack_bc, ppVar10, ppVar23, r1, r2, r3, r4, r5, r6, uVar14, uVar6, uVar7
    local alive = true
    uVar14 = 0
    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_8c);
    if bVar4 then
    end
    cVar5 = me:AcquireControl(4)
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
    quest:EntitySetAsKillable(me, false)
    quest:SetThingHasInformation(me)
    ppVar23 = 0x1
    me:SetFriendsWithEverythingFlag(nil --[[missing]])
    if __native_entity_state:GetStateInt("BirdMode") == 0 then
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d4ef90 end
        r1 = quest:GetAllThingsWithScriptName("BirdMarker")
        if 0 / 0xc + (0 >> 0x1f) ~= 0 >> 0x1f then
            repeat
                alive = not quest:IsActiveThreadTerminating()
                if not alive then return end  -- TODO(native): goto LAB_00d4e0f2
                uVar7 = (**(*0x0 + 0x18))("KillBird")
                r2 = quest:CreateCreature(uVar7, nil --[[missing]], "CREATURE_BIRD_GUILD_SPARROW")
                quest:SetThingPersistent(r2, auStack_58)
                -- TODO(native): piStack_bc = piStack_bc + 3;
                uVar14 = uVar14 + 1
            until not (uVar14 < (0 / 0xc))
        end
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            -- LAB_00d4e0f2: (native jump target)
            return
        end
        quest:SetStateInt("CurrentBirdsKilled", 0)
        __native_entity_state:SetStateInt("BirdMode", 2)
    end
    piStack_bc = quest:RegisterTimer()
    quest:SetTimer(piStack_bc, 0x0)
    iVar9 = __native_entity_state:GetStateInt("BirdMode")
    while iVar9 == 2 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d4ef87 end
        fVar22 = 5.5
        pCVar8 = quest:GetHero()
        bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar8, me, fVar22)
        __native_condition_1 = bVar4
        if __native_condition_1 then
            iVar9 = quest:GetTimer(piStack_bc)
            __native_condition_1 = iVar9 < 1
        end
        if __native_condition_1 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d4ef87 end
            ppVar10 = quest:AddNewConversation(me, false, (ppVar23 ~= 0))
            uVar7 = quest:GetHero()
            quest:AddPersonToConversation(ppVar10, uVar7)
            uVar7 = quest:GetHero()
            quest:AddLineToConversation(ppVar10, "TEXT_QST_028_BIRD_KILLER_HELP", me, uVar7, false)
            quest:SetTimer(piStack_bc, 0xf)
        end
        cVar5 = me:IsTalkedToByHero()
        if cVar5 then
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                pCVar18 = ""
                quest:StartMovieSequence()
                ppVar10 = 0x1
                quest:PauseAllNonScriptedEntities((ppVar10 ~= 0))
                if not __native_entity_state:GetStateBool("HaveChatted") then
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        __native_entity_state:SetStateBool("HaveChatted", true)
                        -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&uStack_c4);
                        fVar15 = quest:GetHealth(me)
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar15 then
                            bVar4 = false
                            pCVar20 = 0x1
                            pCVar19 = 0x0
                            pCVar18 = 0x0
                            pcVar16 = "TEXT_QST_028_BIRD_KILLER_GREET"
                            pCVar8 = quest:GetHero()
                            r3 = me:Speak(pCVar8, pcVar16, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d4e91f
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d4e978 end
                        end
                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_BIRD_KILLER_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "", "TEXT_OBJECT_HERO_ANSWER_NO", true)
                        -- TODO(native): goto LAB_00d4e5e3
                    end
                    ::LAB_00d4e978::
                    quest:PauseAllNonScriptedEntities(false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): ppVar17 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)aCStack_5c;
                        quest:GiveHeroYesNoQuestion("TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", "TEXT_QST_028_BIRD_KILLER_REPEAT_QUESTION", ppVar17)
                        -- LAB_00d4e5e3: (native jump target)
                        iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                        while iVar9 < 0 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                quest:DeregisterTimer(piStack_bc)
                                if (0x1 ~= nil) and (*0x1 = *0x1 + -1, *0x1 == 0) then
                                end
                                return
                            end
                            iVar9 = quest:MsgIsQuestionAnsweredYesOrNo()
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            alive = not quest:IsActiveThreadTerminating()
                            if iVar9 == 1 then
                                if alive then
                                    __native_entity_state:SetStateInt("BirdMode", 1)
                                    -- TODO(native): bVar4 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_70);
                                    if bVar4 then
                                    end
                                    uVar7 = quest:GetHero()
                                    cVar5 = me:AcquireControl(4)
                                    while not cVar5 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then
                                            -- LAB_00d4e91f: (native jump target)
                                            quest:PauseAllNonScriptedEntities(false)
                                            quest:DeregisterTimer(piStack_bc)
                                            return
                                        end
                                        uVar7 = quest:GetHero()
                                        cVar5 = me:AcquireControl(4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        -- TODO(native): StdMap_Construct_API();
                                        -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_50,(CCharString *)&uStack_80);
                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar21);
                                        -- TODO(native): pCVar11 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_50,(CCharString *)&iStack_90);
                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar11,pCVar21);
                                        -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                                        -- TODO(native): StdMap_Destroy_API();
                                        -- TODO(native): goto LAB_00d4e853
                                    end
                                end
                                -- TODO(native): goto LAB_00d4e978
                            end
                            if alive then
                                -- TODO(native): uVar7 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&uStack_c4);
                                uVar6 = SUB41(ppVar10,0)
                                fVar15 = quest:GetHealth(nil --[[missing]])
                                fVar3 = _DAT_0122dedc
                                if fVar3 < fVar15 then
                                    bVar4 = false
                                    pCVar20 = 0x1
                                    pCVar19 = 0x0
                                    pCVar18 = 0x0
                                    pcVar16 = "TEXT_QST_028_BIRD_KILLER_REFUSE"
                                    pCVar8 = quest:GetHero()
                                    r4 = me:Speak(pCVar8, pcVar16, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar4)
                                    bVar4 = me:IsPerformingScriptTask()
                                    if bVar4 then
                                        repeat
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then
                                                quest:PauseAllNonScriptedEntities(false)
                                                quest:DeregisterTimer(piStack_bc)
                                                return
                                            end
                                            bVar4 = me:IsPerformingScriptTask()
                                        until not (bVar4)
                                    end
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d4e9e1 end
                                end
                                -- LAB_00d4e853: (native jump target)
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d4e87a
                            end
                        end
                    end
                    ::LAB_00d4e9e1::
                    quest:PauseAllNonScriptedEntities(false)
                end
            end
            goto LAB_00d4ef87
        end
        ::LAB_00d4e87a::
        iVar9 = __native_entity_state:GetStateInt("BirdMode")
    end
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        iVar9 = __native_entity_state:GetStateInt("BirdMode")
        while iVar9 == 1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d4ef87 end
            fVar22 = 5.5
            pCVar8 = quest:GetHero()
            bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar8, me, fVar22)
            __native_condition_2 = bVar4
            if __native_condition_2 then
                iVar9 = quest:GetTimer(piStack_bc)
                __native_condition_2 = iVar9 < 1
            end
            if __native_condition_2 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d4ef87 end
                if __native_entity_state:GetStateInt("CurrentBirds") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4ef87 end
                    ppVar10 = quest:AddNewConversation(me, false, false)
                    uVar7 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar10, uVar7)
                    uVar7 = quest:GetHero()
                    quest:AddLineToConversation(ppVar10, "TEXT_QST_028_BIRD_KILLER_ANY", me, uVar7, false)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4ef87 end
                    ppVar10 = quest:AddNewConversation(me, false, true)
                    uVar7 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar10, uVar7)
                    uVar7 = quest:GetHero()
                    quest:AddLineToConversation(ppVar10, "TEXT_QST_028_BIRD_KILLER_ANY_MORE", me, uVar7, false)
                end
                quest:SetTimer(piStack_bc, 0xf)
            end
            cVar5 = me:IsTalkedToByHero()
            if cVar5 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d4ef87 end
                if quest:GetStateInt("CurrentBirdsKilled") == 1 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4ef87 end
                    ppVar10 = quest:AddNewConversation(me, false, nil --[[missing]])
                    uVar7 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar10, uVar7)
                    uVar7 = quest:GetHero()
                    quest:AddLineToConversation(ppVar10, "TEXT_QST_028_BIRD_KILLER_ONE", me, uVar7, false)
                    quest:Pause(0x3f800000)
                    uVar7 = __ftol2()
                    quest:GiveHeroGold(0)
                elseif quest:GetStateInt("CurrentBirdsKilled") == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4ef87 end
                    ppVar10 = quest:AddNewConversation(me, false, nil --[[missing]])
                    uVar7 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar10, uVar7)
                    uVar7 = quest:GetHero()
                    quest:AddLineToConversation(ppVar10, "TEXT_QST_028_BIRD_KILLER_NONE", me, uVar7, false)
                    quest:Pause(0x3f800000)
                else
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4ef87 end
                    ppVar10 = quest:AddNewConversation(me, false, nil --[[missing]])
                    uVar7 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar10, uVar7)
                    uVar7 = quest:GetHero()
                    quest:AddLineToConversation(ppVar10, "TEXT_QST_028_BIRD_KILLER_MORE", me, uVar7, false)
                    quest:Pause(0x3f800000)
                    uVar7 = __ftol2()
                    quest:GiveHeroGold(0)
                end
                __native_entity_state:SetStateInt("CurrentBirds", __native_entity_state:GetStateInt("CurrentBirds") + quest:GetStateInt("CurrentBirdsKilled"))
                if quest:GetStateInt("CurrentBirdsKilled") ~= 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then goto LAB_00d4ef87 end
                    quest:SetStateInt("CurrentBirdsKilled", 0)
                    ppVar10 = quest:AddNewConversation(me, false, nil --[[missing]])
                    if __native_entity_state:GetStateInt("CurrentBirds") == 7 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4ef87 end
                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&uStack_64);
                        pCVar18 = ""
                        quest:StartMovieSequence()
                        quest:PauseAllNonScriptedEntities(true)
                        -- TODO(native): uVar6 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ((CScriptGameResourceObjectScriptedThingBase *)&uStack_c4);
                        fVar15 = quest:GetHealth(nil --[[missing]])
                        fVar3 = _DAT_0122dedc
                        if fVar3 < fVar15 then
                            bVar4 = false
                            pCVar20 = 0x1
                            pCVar19 = 0x0
                            pCVar18 = 0x0
                            pcVar16 = "TEXT_QST_028_BIRD_KILLER_DONE"
                            pCVar8 = quest:GetHero()
                            r5 = me:Speak(pCVar8, pcVar16, pCVar18, (pCVar19 ~= 0), (pCVar20 ~= 0), bVar4)
                            bVar4 = me:IsPerformingScriptTask()
                            if bVar4 then
                                repeat
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then
                                        quest:PauseAllNonScriptedEntities(false)
                                        goto LAB_00d4ef87
                                    end
                                    bVar4 = me:IsPerformingScriptTask()
                                until not (bVar4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then
                                quest:PauseAllNonScriptedEntities(false)
                                goto LAB_00d4ef87
                            end
                        end
                        uVar7 = __ftol2()
                        quest:GiveHeroGold(0)
                        __native_entity_state:SetStateInt("BirdMode", 3)
                        quest:ClearThingHasInformation(me)
                        quest:PauseAllNonScriptedEntities(false)
                    else
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d4ef87 end
                        r6 = quest:GetHero()
                        quest:AddPersonToConversation(ppVar10, r6)
                        uVar7 = quest:GetHero()
                        quest:AddLineToConversation(ppVar10, "TEXT_QST_028_BIRD_KILLER_NOT_DONE", me, uVar7, false)
                        quest:Pause(0x3f800000)
                    end
                end
            end
            iVar9 = __native_entity_state:GetStateInt("BirdMode")
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            alive = not quest:IsActiveThreadTerminating()
            cVar5 = not alive
            while not cVar5 do
                fVar22 = 5.5
                pCVar8 = quest:GetHero()
                bVar4 = quest:IsDistanceBetweenThingsUnder(pCVar8, me, fVar22)
                __native_condition_3 = bVar4
                if __native_condition_3 then
                    iVar9 = quest:GetTimer(piStack_bc)
                    __native_condition_3 = iVar9 < 1
                end
                if __native_condition_3 then
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then break end
                    ppVar23 = quest:AddNewConversation(me, false, nil --[[missing]])
                    uVar7 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar23, uVar7)
                    uVar7 = quest:GetHero()
                    quest:AddLineToConversation(ppVar23, "TEXT_QST_028_BIRD_KILLER_FINISHED", me, uVar7, false)
                    quest:SetTimer(piStack_bc, 0xf)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                cVar5 = not alive
            end
        end
    end
    ::LAB_00d4ef87::
    ::LAB_00d4ef90::
end

function Init(quest, me)
    __native_entity_state:SetStateBool("HaveChatted", false)
    __native_entity_state:SetStateInt("BirdMode", 0)
    __native_entity_state:SetStateInt("CurrentBirds", 0)
    quest:SetThingPersistent(me, true)
end

function OnPersist(quest, context)
    local birdMode = quest:GetStateBool("BirdMode") or false
    birdMode = quest:PersistTransferBool(context, "BirdMode", birdMode)
    quest:SetStateBool("BirdMode", birdMode)
    local currentBirds = quest:GetStateBool("CurrentBirds") or false
    currentBirds = quest:PersistTransferBool(context, "CurrentBirds", currentBirds)
    quest:SetStateBool("CurrentBirds", currentBirds)
end

function OnPredicateFail(quest, me)
end

