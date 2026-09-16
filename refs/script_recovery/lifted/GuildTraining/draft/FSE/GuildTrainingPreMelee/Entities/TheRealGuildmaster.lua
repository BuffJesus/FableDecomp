-- Generated native draft: TheRealGuildmaster. Review coverage report before use.
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
    local CVar31, CVar40, CVar42, __native_condition_1, aVar29, aVar30, bVar2, cVar3, fVar1, fVar15, fVar32, iVar41, iVar5, pCVar18, pCVar21, pCVar22, pCVar23, pCVar24, pCVar25, pCVar28, pCVar33, pCVar4, pCVar9, paVar11, paVar12, pcVar17, pcVar20, piVar10, piVar8, ppVar19, ppVar27, puVar7, r1, r10, r11, r12, r13, r14, r15, r16, r17, r18, r19, r2, r20, r21, r22, r23, r24, r25, r26, r27, r28, r29, r3, r30, r31, r32, r33, r34, r35, r36, r37, r38, r39, r4, r40, r41, r42, r43, r44, r5, r6, r7, r8, r9, uVar14, uVar16, uVar26, uVar37
    local alive = true
    cVar3 = quest:GetStateBool("GuildmasterTeleport")
    uVar14 = 0
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        cVar3 = quest:GetStateBool("GuildmasterTeleport")
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    pCVar18 = "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_01"
    pCVar28 = "Q_GuildTraining"
    quest:SetQuestCardObjective("", "", "Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_01")
    -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&ppuStack_18c);
    if bVar2 then
    end
    -- TODO(native): ppVar34 = (pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)pCVar4;
    cVar3 = me:AcquireControl(4)
    while not cVar3 do
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d55c4f end
        cVar3 = me:AcquireControl(4)
    end
    alive = not quest:IsActiveThreadTerminating()
    if not alive then goto LAB_00d55c4f end
    quest:EntitySetAsKillable(nil --[[missing]], false)
    quest:SetIsPushableByHero(nil --[[missing]], false)
    quest:SetThingHasInformation(nil --[[missing]])
    r1 = quest:GetThingWithScriptName("M_MeleeTeacherStand")
    quest:EntityTeleportToThing(r1, nil --[[missing]])
    r2 = quest:RegisterTimer()
    quest:SetTimer(0, uVar14)
    -- TODO(native): cStack_185 = '\x01';
    repeat
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d55c46 end
        cVar3 = me:IsTalkedToByHero()
        if cVar3 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then goto LAB_00d55c46 end
            -- TODO(native): cStack_185 = '\0';
        end
        fVar32 = 5.5
        pCVar4 = quest:GetHero()
        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar4, me, fVar32)
        -- TODO(native): if ((!bVar2) || (iVar5 = GSI->GetTimer(), 0 < iVar5)) goto switchD_00d5317f_default;
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d55c46 end
        r3 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
        r4 = quest:GetHero()
        quest:AddPersonToConversation(nil --[[missing]], r4)
        quest:SetTimer(nil --[[missing]], nil --[[missing]])
        -- TODO(native): switch(uVar14) {
        -- TODO(native): case 0:
        r5 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(r5, nil --[[missing]])
        r6 = quest:GetHero()
        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FIRST", r6, nil --[[missing]])
        me:PlayAnimation("ST_WAVE_SPECIAL_02", false, false, false, true, DAT_01375748, false)
        uVar14 = 1
        break
        -- TODO(native): case 1:
        r7 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(r7, nil --[[missing]])
        r8 = quest:GetHero()
        quest:AddLineToConversation(uVar14, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_SECOND", r8, nil --[[missing]])
        goto LAB_00d53316
        -- TODO(native): case 2:
        r9 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(r9, nil --[[missing]])
        r10 = quest:GetHero()
        quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_THIRD", r10, nil --[[missing]])
        uVar14 = 3
        break
        -- TODO(native): case 3:
        r11 = quest:GetHero()
        quest:EntitySetFacingAngleTowardsThing(r11, nil --[[missing]])
        r12 = quest:GetHero()
        quest:AddLineToConversation(uVar14, "TEXT_QST_028_GUILDMASTER_PREMELEE_COMMENT_FOURTH", r12, nil --[[missing]])
        ::LAB_00d53316::
        uVar14 = 2
    end
    -- TODO(native): switchD_00d5317f_default:
    until not (cStack_185 ~= 0)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        quest:SetStateBool("WhisperStopFollowing", true)
        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_168);
        -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_168);
        if bVar2 then
        end
        r13 = quest:GetHero()
        cVar3 = me:AcquireControl(4)
        while not cVar3 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            if not alive then return end  -- TODO(native): goto LAB_00d533bb
            r14 = quest:GetHero()
            cVar3 = me:AcquireControl(4)
        end
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            -- TODO(native): StdMap_Construct_API();
            -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_158,(CCharString *)&stack0xfffffe58);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6, (CScriptGameResourceObjectScriptedThingBase *)pCVar33);
            -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_158,(CCharString *)&stack0xfffffe58);
            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar23);
            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_140);
            quest:StartMovieSequence()
            quest:PauseAllNonScriptedEntities((uVar14 ~= 0))
            quest:FixMovieSequenceCamera(nil --[[missing]])
            ppVar27 = 0x0
            -- TODO(native): RunCutsceneMacro_Func();
            quest:FixMovieSequenceCamera((ppVar27 ~= 0))
            ppVar27 = quest:GetThingWithScriptName("PreMeleeWhisper")
            quest:RemoveThing(ppVar27)
            quest:PauseAllNonScriptedEntities(nil --[[missing]])
            -- TODO(native): StdMap_Destroy_API();
            cVar3 = quest:IsXbox()
            if not cVar3 then
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH_PC")
                    cVar3 = quest:MsgIsGameInfoClickedPast()
                    while not cVar3 do
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d55c46 end
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffe58,(CFontBank *)ppVar34,pCVar35,pCVar36, pCVar28,pCVar18,(ulong)puVar38,(float)puVar39,(bool)CVar40, (bool)SUB41(unaff_ESI,0));
                        -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffe58,(CFontBank *)ppVar34,pCVar35,pCVar36, pCVar28,pCVar18,(ulong)puVar38,(float)puVar39,(bool)CVar40, (bool)SUB41(unaff_ESI,0));
                        -- TODO(native): goto LAB_00d536c0
                    end
                end
            else
                alive = not quest:IsActiveThreadTerminating()
                if alive then
                    quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_PUNCH")
                    cVar3 = quest:MsgIsGameInfoClickedPast()
                    while true do
                        if not (not cVar3) then break end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then goto LAB_00d55c46 end
                        cVar3 = quest:MsgIsGameInfoClickedPast()
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    if alive then
                        -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffe58,(CFontBank *)ppVar34,pCVar35,pCVar36, pCVar28,pCVar18,(ulong)puVar38,(float)puVar39,(bool)CVar40, (bool)SUB41(unaff_ESI,0));
                        -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffe58,(CFontBank *)ppVar34,pCVar35,pCVar36, pCVar28,pCVar18,(ulong)puVar38,(float)puVar39,(bool)CVar40, (bool)SUB41(unaff_ESI,0));
                        -- LAB_00d536c0: (native jump target)
                        -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffe60);
                        quest:SetTimer(nil --[[missing]], nil --[[missing]])
                        quest:SetStateInt("PreMeleeMode", 1)
                        quest:SetStateInt("DummyHits", 0)
                        uVar16 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", nil --[[missing]], nil --[[missing]])
                        -- TODO(native): auStack_160[0] = (undefined4)uVar16;
                        quest:DisplayQuestInfo(nil --[[missing]])
                        iVar41 = 0
                        iVar5 = quest:GetStateInt("DummyHits")
                        while iVar5 < 7 do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then goto LAB_00d55c3d end
                            quest:UpdateQuestInfoCounter(iVar41, nil --[[missing]], nil --[[missing]])
                            if iVar41 ~= quest:GetStateInt("DummyHits") then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d55c3d end
                                iVar41 = quest:GetStateInt("DummyHits")
                                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                            end
                            iVar5 = quest:GetTimer(nil --[[missing]])
                            if iVar5 < 1 then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d55c3d end
                                ppVar27 = quest:AddNewConversation(r14, nil --[[missing]], nil --[[missing]])
                                r15 = quest:GetHero()
                                quest:AddPersonToConversation(nil --[[missing]], r15)
                                r16 = quest:GetHero()
                                quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_PREMELEE_HIT_DUMMY", r16, r13)
                                cVar3 = quest:IsXbox()
                                if not cVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d55c3d end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP_PC")
                                    cVar3 = quest:MsgIsGameInfoClickedPast()
                                    while not cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00d55c3d end
                                        cVar3 = quest:MsgIsGameInfoClickedPast()
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then goto LAB_00d55c3d end
                                    quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_ATTACK_HELP")
                                    cVar3 = quest:MsgIsGameInfoClickedPast()
                                    while not cVar3 do
                                        alive = quest:NewScriptFrame(me)
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then goto LAB_00d55c3d end
                                        cVar3 = quest:MsgIsGameInfoClickedPast()
                                    end
                                end
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then goto LAB_00d55c3d end
                                quest:SetTimer(nil --[[missing]], nil --[[missing]])
                            end
                            iVar5 = quest:GetStateInt("DummyHits")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if alive then
                            quest:RemoveQuestInfoElement(nil --[[missing]])
                            quest:DisplayQuestInfo(nil --[[missing]])
                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&iStack_170);
                            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&iStack_170);
                            if bVar2 then
                            end
                            r17 = quest:GetHero()
                            cVar3 = me:AcquireControl(4)
                            while not cVar3 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d53a0b
                                r18 = quest:GetHero()
                                cVar3 = me:AcquireControl(4)
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if alive then
                                -- TODO(native): StdMap_Construct_API();
                                -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_158,(CCharString *)&stack0xfffffe58);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6, (CScriptGameResourceObjectScriptedThingBase *)pCVar33);
                                -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_158,(CCharString *)&stack0xfffffe58);
                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar23);
                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_140);
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                quest:FixMovieSequenceCamera(nil --[[missing]])
                                ppVar27 = 0x0
                                -- TODO(native): RunCutsceneMacro_Func();
                                quest:FixMovieSequenceCamera((ppVar27 ~= 0))
                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                -- TODO(native): StdMap_Destroy_API();
                                cVar3 = quest:IsXbox()
                                if not cVar3 then
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK_PC")
                                        cVar3 = quest:MsgIsGameInfoClickedPast()
                                        CVar40 = SUB41(iVar41,0)
                                        while not cVar3 do
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00d55c3d end
                                            cVar3 = quest:MsgIsGameInfoClickedPast()
                                            CVar40 = SUB41(iVar41,0)
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffe58,(CFontBank *)ppVar34,pCVar35, pCVar36,pCVar28,pCVar18,(ulong)puVar38,(float)puVar39,(bool)CVar40, (bool)SUB41(unaff_ESI,0));
                                            -- TODO(native): goto LAB_00d53c7e
                                        end
                                    end
                                else
                                    alive = not quest:IsActiveThreadTerminating()
                                    if alive then
                                        quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_STICK")
                                        cVar3 = quest:MsgIsGameInfoClickedPast()
                                        while true do
                                            CVar40 = SUB41(iVar41,0)
                                            if not (not cVar3) then break end
                                            alive = quest:NewScriptFrame(me)
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then goto LAB_00d55c3d end
                                            cVar3 = quest:MsgIsGameInfoClickedPast()
                                        end
                                        alive = not quest:IsActiveThreadTerminating()
                                        if alive then
                                            -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffe58,(CFontBank *)ppVar34,pCVar35, pCVar36,pCVar28,pCVar18,(ulong)puVar38,(float)puVar39,(bool)CVar40, (bool)SUB41(unaff_ESI,0));
                                            -- LAB_00d53c7e: (native jump target)
                                            quest:SetStateInt("PreMeleeMode", 2)
                                            quest:SetStateInt("DummyHits", 0)
                                            quest:SetTimer(nil --[[missing]], nil --[[missing]])
                                            uVar16 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_TARGET_DUMMY", nil --[[missing]], nil --[[missing]])
                                            -- TODO(native): auStack_160[0] = (undefined4)uVar16;
                                            quest:DisplayQuestInfo(nil --[[missing]])
                                            iVar41 = 0
                                            iVar5 = quest:GetStateInt("DummyHits")
                                            while iVar5 < 7 do
                                                alive = quest:NewScriptFrame(me)
                                                alive = not quest:IsActiveThreadTerminating()
                                                if not alive then goto LAB_00d55c3d end
                                                quest:UpdateQuestInfoCounter(iVar41, nil --[[missing]], nil --[[missing]])
                                                if iVar41 ~= quest:GetStateInt("DummyHits") then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then goto LAB_00d55c3d end
                                                    iVar41 = quest:GetStateInt("DummyHits")
                                                    quest:SetTimer(nil --[[missing]], nil --[[missing]])
                                                end
                                                iVar5 = quest:GetTimer(nil --[[missing]])
                                                if iVar5 < 1 then
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then goto LAB_00d55c3d end
                                                    ppVar27 = quest:AddNewConversation(r18, nil --[[missing]], nil --[[missing]])
                                                    r19 = quest:GetHero()
                                                    quest:AddPersonToConversation(nil --[[missing]], r19)
                                                    r20 = quest:GetHero()
                                                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_PREMELEE_STICK_USAGE", r20, r17)
                                                    cVar3 = quest:IsXbox()
                                                    if not cVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC")
                                                        cVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not cVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if not alive then goto LAB_00d55c3d end
                                                            cVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then goto LAB_00d55c3d end
                                                        quest:DisplayGameInfo("TEXT_QST_028_ONSCREENHELP_WIELD_HELP")
                                                        cVar3 = quest:MsgIsGameInfoClickedPast()
                                                        while not cVar3 do
                                                            alive = quest:NewScriptFrame(me)
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if not alive then goto LAB_00d55c3d end
                                                            cVar3 = quest:MsgIsGameInfoClickedPast()
                                                        end
                                                    end
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then goto LAB_00d55c3d end
                                                    quest:SetTimer(nil --[[missing]], nil --[[missing]])
                                                end
                                                iVar5 = quest:GetStateInt("DummyHits")
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            if alive then
                                                quest:RemoveQuestInfoElement(nil --[[missing]])
                                                quest:DisplayQuestInfo(nil --[[missing]])
                                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)&uStack_184);
                                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_148);
                                                quest:StartMovieSequence()
                                                -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_178);
                                                -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_178);
                                                if bVar2 then
                                                end
                                                r21 = quest:GetHero()
                                                cVar3 = me:AcquireControl(4)
                                                while not cVar3 do
                                                    alive = quest:NewScriptFrame(me)
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then return end  -- TODO(native): goto LAB_00d53ff2
                                                    r22 = quest:GetHero()
                                                    cVar3 = me:AcquireControl(4)
                                                end
                                                alive = not quest:IsActiveThreadTerminating()
                                                if alive then
                                                    r23 = quest:GetThingWithScriptName("PreMeleeDummy")
                                                    if piStack_108 == nil then
                                                    else
                                                        puVar7 = (**(*piStack_108 + 0x18))()
                                                    end
                                                    -- TODO(native): uStack_138 = *puVar7;
                                                    -- TODO(native): uStack_134 = puVar7[1];
                                                    -- TODO(native): auStack_130[0] = puVar7[2];
                                                    -- TODO(native): CreateEffect is not a ForgeFSE binding
                                                    quest:CreateEffect()
                                                    -- TODO(native): StdMap_Construct_API();
                                                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aCStack_17c,(CCharString *)&stack0xfffffe34);
                                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar23);
                                                    -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[]((map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)aCStack_17c,(CCharString *)&stack0xfffffe34);
                                                    -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6,pCVar23);
                                                    quest:FixMovieSequenceCamera(nil --[[missing]])
                                                    ppVar27 = 0x0
                                                    -- TODO(native): RunCutsceneMacro_Func();
                                                    quest:PauseAllNonScriptedEntities((ppVar27 ~= 0))
                                                    iVar5 = quest:CreateExperienceOrb(nil --[[missing]], nil --[[missing]])
                                                    -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&stack0xfffffe40, (CCountedPointer<class_CDiskFileWin32> *)(iVar5 + 4));
                                                    quest:EntitySetCutsceneBehaviour(iVar5, nil --[[missing]])
                                                    pcVar17 = 0x0
                                                    ppVar27 = 0x0
                                                    -- TODO(native): RunCutsceneMacro_Func(0,0,0,1);
                                                    quest:FixMovieSequenceCamera(false)
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    -- TODO(native): StdMap_Destroy_API();
                                                    cVar3 = quest:IsXbox()
                                                    if not cVar3 then
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if alive then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_PC")
                                                            cVar3 = quest:MsgIsGameInfoClickedPast()
                                                            CVar40 = SUB41(iVar41,0)
                                                            CVar42 = SUB41(unaff_ESI,0)
                                                            while not cVar3 do
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                if not alive then goto LAB_00d55c34 end
                                                                cVar3 = quest:MsgIsGameInfoClickedPast()
                                                                CVar40 = SUB41(iVar41,0)
                                                                CVar42 = SUB41(unaff_ESI,0)
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if alive then
                                                                -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffe58, (CFontBank *)ppVar34,pCVar35,pCVar36,pCVar28,pCVar18, (ulong)puVar38,(float)puVar39,(bool)CVar40,(bool)CVar42);
                                                                -- TODO(native): goto LAB_00d5439e
                                                            end
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if alive then
                                                            quest:DisplayGameInfo("TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP")
                                                            cVar3 = quest:MsgIsGameInfoClickedPast()
                                                            CVar42 = SUB41(unaff_ESI,0)
                                                            while true do
                                                                CVar40 = SUB41(iVar41,0)
                                                                if not (not cVar3) then break end
                                                                alive = quest:NewScriptFrame(me)
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                if not alive then goto LAB_00d55c34 end
                                                                cVar3 = quest:MsgIsGameInfoClickedPast()
                                                                CVar42 = SUB41(unaff_ESI,0)
                                                            end
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if alive then
                                                                -- TODO(native): CSubtitleRenderer::SetText((CSubtitleRenderer *)&stack0xfffffe58, (CFontBank *)ppVar34,pCVar35,pCVar36,pCVar28,pCVar18, (ulong)puVar38,(float)puVar39,(bool)CVar40,(bool)CVar42);
                                                                -- LAB_00d5439e: (native jump target)
                                                                -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffe64);
                                                                quest:SetTimer(pcVar17, nil --[[missing]])
                                                                bVar2 = aCStack_17c:IsAlive()
                                                                if bVar2 then
                                                                    repeat
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        if not alive then goto LAB_00d55c2b end
                                                                        iVar5 = quest:GetTimer(nil --[[missing]])
                                                                        if iVar5 < 1 then
                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                            if not alive then goto LAB_00d55c2b end
                                                                            ppVar27 = quest:AddNewConversation(r23, nil --[[missing]], nil --[[missing]])
                                                                            r24 = quest:GetHero()
                                                                            quest:AddPersonToConversation(nil --[[missing]], r24)
                                                                            r25 = quest:GetHero()
                                                                            quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_PREMELEE_INSTRUCTIONS_XP_REPEAT_10", r25, r22)
                                                                            quest:SetTimer(nil --[[missing]], nil --[[missing]])
                                                                        end
                                                                        bVar2 = aCStack_17c:IsAlive()
                                                                    until not (bVar2)
                                                                end
                                                                alive = not quest:IsActiveThreadTerminating()
                                                                if alive then
                                                                    quest:Pause(nil --[[missing]])
                                                                    -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aaStack_16c);
                                                                    -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aaStack_16c);
                                                                    if bVar2 then
                                                                    end
                                                                    r26 = quest:GetHero()
                                                                    cVar3 = me:AcquireControl(4)
                                                                    while not cVar3 do
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        if not alive then return end  -- TODO(native): goto LAB_00d54dfa
                                                                        r27 = quest:GetHero()
                                                                        cVar3 = me:AcquireControl(4)
                                                                    end
                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                    if alive then
                                                                        -- TODO(native): StdMap_Construct_API();
                                                                        -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)auStack_130,(CCharString *)&stack0xfffffe58);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6, (CScriptGameResourceObjectScriptedThingBase *)pCVar33 );
                                                                        -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](( map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> *)auStack_130,(CCharString *)&stack0xfffffe58);
                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator= ((CScriptGameResourceObjectScriptedThingBase *)pCVar6, pCVar23);
                                                                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_140);
                                                                        quest:StartMovieSequence()
                                                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                                        quest:FixMovieSequenceCamera(nil --[[missing]])
                                                                        ppVar27 = 0x0
                                                                        -- TODO(native): RunCutsceneMacro_Func();
                                                                        quest:FixMovieSequenceCamera((ppVar27 ~= 0))
                                                                        quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                                        -- TODO(native): StdMap_Destroy_API();
                                                                        quest:GiveHeroQuestCardDirectly("OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", "OBJECT_QUEST_CARD_TRAINING_KILL_BEETLES", nil --[[missing]])
                                                                        quest:SetQuestCardObjective("Q_GuildTrainingWoodsMelee", "Q_GuildTrainingWoodsMelee", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "GuildWoods")
                                                                        quest:KickOffQuestStartScreen("Q_GuildTrainingWoodsMelee", nil --[[missing]], nil --[[missing]])
                                                                        pCVar18 = 0xd54777
                                                                        alive = quest:NewScriptFrame(me)
                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                        if alive then
                                                                            alive = quest:NewScriptFrame(me)
                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                            if alive then
                                                                                alive = quest:NewScriptFrame(me)
                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                if alive then
                                                                                    alive = quest:NewScriptFrame(me)
                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                    if alive then
                                                                                        alive = quest:NewScriptFrame(me)
                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                        if alive then
                                                                                            cVar3 = quest:DisplayTutorial(nil --[[missing]])
                                                                                            if not cVar3 then
                                                                                                -- LAB_00d54846: (native jump target)
                                                                                                ppVar27 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapRemoveMarker(ppVar27)
                                                                                                ppVar27 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                quest:MiniMapAddMarker(ppVar27, "HUD_ORB_GREEN_SMALL")
                                                                                                uVar26 = SUB41(&"MK_GTM_WD_GUARD",0)
                                                                                                piVar8 = quest:GetThingWithScriptName("MK_GTM_WD_GUARD")
                                                                                                pCVar24 = 0x1
                                                                                                pCVar22 = 0x0
                                                                                                pCVar21 = 0x0
                                                                                                pCVar4 = piVar8:GetPos()
                                                                                                me:MoveToPosition(pCVar4, pcVar20, SUB41(pCVar25,0))
                                                                                                uVar37 = CONCAT13(1,(int3)pCVar28)
                                                                                                quest:SetQuestCardObjective("Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01", "Q_GuildTraining", "TEXT_QUEST_GUILD_TRAINING_OBJECTIVE_02_OPTION_01")
                                                                                                -- TODO(native): uStack_180 = uStack_180 & 0xffffff;
                                                                                                repeat
                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    if not alive then goto LAB_00d55c2b end
                                                                                                    if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then return end  -- TODO(native): goto LAB_00d54f9c
                                                                                                    if not __native_entity_state:GetStateBool("WoodsEndPlayed") then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        if not alive then goto LAB_00d55c2b end
                                                                                                        __native_entity_state:SetStateBool("WoodsEndPlayed", true)
                                                                                                        ppVar27 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapRemoveMarker(ppVar27)
                                                                                                        ppVar27 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                                                                        quest:MiniMapAddMarker(ppVar27, "HUD_ORB_QUEST_CORE")
                                                                                                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_140);
                                                                                                        -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)aCStack_140);
                                                                                                        if bVar2 then
                                                                                                        end
                                                                                                        r28 = quest:GetHero()
                                                                                                        cVar3 = me:AcquireControl(4)
                                                                                                        while not cVar3 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if not alive then return end  -- TODO(native): goto LAB_00d55c91
                                                                                                            r29 = quest:GetHero()
                                                                                                            cVar3 = me:AcquireControl(4)
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        if not alive then
                                                                                                            -- LAB_00d55c91: (native jump target)
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        -- TODO(native): StdMap_Construct_API();
                                                                                                        -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_158,aCStack_b8);
                                                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(( CScriptGameResourceObjectScriptedThingBase *) pCVar6,pCVar23);
                                                                                                        -- TODO(native): pCVar6 = std::map<CCharString,CCountedPointer<NUISystem::CComponent>,std::less<CCharString>,std::allocator<std::pair<CCharString_const_,CCountedPointer<NUISystem::CComponent>_>_>_> ::operator[](amStack_158,aCStack_a4);
                                                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::operator=(( CScriptGameResourceObjectScriptedThingBase *) pCVar6,pCVar23);
                                                                                                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_140);
                                                                                                        pCVar28 = ""
                                                                                                        quest:StartMovieSequence()
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar21 ~= 0))
                                                                                                        quest:FixMovieSequenceCamera((pCVar22 ~= 0))
                                                                                                        ppVar27 = 0x0
                                                                                                        -- TODO(native): RunCutsceneMacro_Func();
                                                                                                        paVar11 = "TEXT_OBJECT_HERO_ANSWER_YES"
                                                                                                        -- TODO(native): ppVar27 = ( pair<enum_EHeroMorphType,class_CParticleMorphs::CEntry> *)aCStack_f8;
                                                                                                        quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                                                                                        iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        while iVar5 < 0 do
                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if not alive then return end  -- TODO(native): goto LAB_00d55c63
                                                                                                            iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        if not alive then
                                                                                                            -- LAB_00d55c72: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities((pCVar24 ~= 0))
                                                                                                            -- LAB_00d55c7f: (native jump target)
                                                                                                            -- TODO(native): StdMap_Destroy_API();
                                                                                                            -- TODO(native): goto LAB_00d55c91
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        if iVar5 == 1 then
                                                                                                            if not alive then
                                                                                                                -- LAB_00d55c63: (native jump target)
                                                                                                                quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                                                                                -- TODO(native): goto LAB_00d55c7f
                                                                                                            end
                                                                                                            quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
                                                                                                            quest:Pause(nil --[[missing]])
                                                                                                            paVar12 = "Data\\Video\\2_guild_split_1_comp.xmv"
                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                        else
                                                                                                            if not alive then return end  -- TODO(native): goto LAB_00d55c72
                                                                                                            -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(( CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffe34);
                                                                                                            fVar15 = quest:GetHealth(r29)
                                                                                                            fVar1 = _DAT_0122dedc
                                                                                                            if fVar1 < fVar15 then
                                                                                                                bVar2 = false
                                                                                                                pCVar22 = 0x1
                                                                                                                pCVar21 = 0x0
                                                                                                                pCVar28 = 0x0
                                                                                                                pcVar20 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                pCVar4 = quest:GetHero()
                                                                                                                r30 = me:Speak(pCVar4, pcVar20, pCVar28, (pCVar21 ~= 0), (pCVar22 ~= 0), bVar2)
                                                                                                                bVar2 = me:IsPerformingScriptTask()
                                                                                                                if bVar2 then
                                                                                                                    repeat
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        if not alive then return end  -- TODO(native): goto LAB_00d55c63
                                                                                                                        bVar2 = me:IsPerformingScriptTask()
                                                                                                                    until not (bVar2)
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                if not alive then return end  -- TODO(native): goto LAB_00d55c72
                                                                                                            end
                                                                                                            piVar10 = (**(**(this + 4) + 0x120 ))()
                                                                                                            bVar2 = false
                                                                                                            ppVar19 = 0x0
                                                                                                            pCVar28 = 0xd54ef5
                                                                                                            pCVar4 = quest:IsXbox()
                                                                                                            me:MoveToPosition(nil --[[missing]], pcVar17, pCVar18, (pCVar28 ~= 0), SUB41(ppVar19,0))
                                                                                                        end
                                                                                                        quest:FixMovieSequenceCamera((ppVar19 ~= 0))
                                                                                                        pCVar18 = 0xd54f36
                                                                                                        quest:PauseAllNonScriptedEntities(bVar2)
                                                                                                        -- TODO(native): StdMap_Destroy_API();
                                                                                                    else
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                            -- LAB_00d54f9c: (native jump target)
                                                                                                            bVar2 = false
                                                                                                        else
                                                                                                            -- TODO(native): uStack_144 = uStack_144 | 1;
                                                                                                            cVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            if not cVar3 then return end  -- TODO(native): goto LAB_00d54f9c
                                                                                                            bVar2 = true
                                                                                                        end
                                                                                                        if (uStack_144 & 1) ~= 0 then
                                                                                                            -- TODO(native): uStack_144 = uStack_144 & 0xfffffffe;
                                                                                                        end
                                                                                                        if bVar2 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if not alive then goto LAB_00d55c2b end
                                                                                                            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *) &stack0xfffffe68);
                                                                                                            if bVar2 then
                                                                                                            end
                                                                                                            cVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            while cVar3 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                if not alive then goto LAB_00d55c2b end
                                                                                                                cVar3 = quest:IsQuestActive("Q_GuildTrainingWoodsMelee")
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if not alive then goto LAB_00d55c2b end
                                                                                                            -- TODO(native): bVar2 = C3DMeshInfo::HasPhysicsMesh((C3DMeshInfo *)&stack0xfffffe68 );
                                                                                                            if bVar2 then
                                                                                                            end
                                                                                                            cVar3 = me:AcquireControl(4)
                                                                                                            while not cVar3 do
                                                                                                                alive = quest:NewScriptFrame(me)
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                if not alive then goto LAB_00d55c2b end
                                                                                                                cVar3 = me:AcquireControl(4)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if not alive then goto LAB_00d55c2b end
                                                                                                            -- TODO(native): CCarriedReadableDef::CCarriedReadableDef(aCStack_168);
                                                                                                            pCVar25 = ""
                                                                                                            quest:StartMovieSequence()
                                                                                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                                                                            -- TODO(native): uVar14 = CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(( CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffe5c);
                                                                                                            fVar15 = quest:GetHealth(r28)
                                                                                                            fVar1 = _DAT_0122dedc
                                                                                                            if fVar15 <= fVar1 then
                                                                                                                -- LAB_00d551d4: (native jump target)
                                                                                                                pCVar28 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION"
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                                                                                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                aVar29 = SUB41(uVar14,0)
                                                                                                                while iVar5 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    if not alive then return end  -- TODO(native): goto LAB_00d55c9f
                                                                                                                    iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                    aVar29 = SUB41(uVar14,0)
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                if alive then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    if iVar5 == 1 then
                                                                                                                        if not alive then
                                                                                                                            -- LAB_00d55c9f: (native jump target)
                                                                                                                            quest:PauseAllNonScriptedEntities(nil --[[missing]])
                                                                                                                            goto LAB_00d55c2b
                                                                                                                        end
                                                                                                                        quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
                                                                                                                        quest:Pause(nil --[[missing]])
                                                                                                                        pCVar28 = "Data\\Video\\2_guild_split_1_comp.xmv"
                                                                                                                        quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                        uVar37 = uVar37 & 0xffffff
                                                                                                                    else
                                                                                                                        if not alive then goto LAB_00d55cba end
                                                                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(( CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffe58);
                                                                                                                        fVar15 = quest:GetHealth(r27)
                                                                                                                        fVar1 = _DAT_0122dedc
                                                                                                                        if fVar1 < fVar15 then
                                                                                                                            CVar40 = 0x0
                                                                                                                            pCVar22 = 0x1
                                                                                                                            pCVar21 = 0x0
                                                                                                                            pCVar28 = 0x0
                                                                                                                            pcVar20 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar4 = quest:GetHero()
                                                                                                                            r31 = me:Speak(pCVar4, pcVar20, pCVar28, (pCVar21 ~= 0), (pCVar22 ~= 0), (CVar40 ~= 0))
                                                                                                                            bVar2 = me:IsPerformingScriptTask()
                                                                                                                            if bVar2 then
                                                                                                                                repeat
                                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                                    if not alive then return end  -- TODO(native): goto LAB_00d55c9f
                                                                                                                                    bVar2 = me:IsPerformingScriptTask()
                                                                                                                                until not (bVar2)
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            if not alive then goto LAB_00d55cba end
                                                                                                                        end
                                                                                                                        piVar10 = (**(**(this + 4) + 0x120 ))()
                                                                                                                        bVar2 = false
                                                                                                                        ppVar19 = 0x0
                                                                                                                        pCVar28 = 0xd55443
                                                                                                                        pCVar4 = quest:IsXbox()
                                                                                                                        me:MoveToPosition(nil --[[missing]], pCVar28, SUB41(ppVar19,0))
                                                                                                                    end
                                                                                                                    quest:PauseAllNonScriptedEntities((ppVar19 ~= 0))
                                                                                                                    goto LAB_00d55480
                                                                                                                end
                                                                                                            else
                                                                                                                CVar40 = 0x0
                                                                                                                pCVar22 = 0x1
                                                                                                                pCVar21 = 0x0
                                                                                                                pCVar28 = 0x0
                                                                                                                pcVar20 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                pCVar4 = quest:GetHero()
                                                                                                                r32 = me:Speak(pCVar4, pcVar20, pCVar28, (pCVar21 ~= 0), (pCVar22 ~= 0), (CVar40 ~= 0))
                                                                                                                bVar2 = me:IsPerformingScriptTask()
                                                                                                                if bVar2 then
                                                                                                                    repeat
                                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                                        if not alive then return end  -- TODO(native): goto LAB_00d55c9f
                                                                                                                        bVar2 = me:IsPerformingScriptTask()
                                                                                                                    until not (bVar2)
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                if alive then return end  -- TODO(native): goto LAB_00d551d4
                                                                                                            end
                                                                                                            ::LAB_00d55cba::
                                                                                                            quest:PauseAllNonScriptedEntities((CVar40 ~= 0))
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                    end
                                                                                                    ::LAB_00d55480::
                                                                                                    cVar3 = (**(*(me) + 0x6c) )()
                                                                                                    if cVar3 ~= 0 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        if not alive then goto LAB_00d55c2b end
                                                                                                        -- TODO(native): CCarriedReadableDef::CCarriedReadableDef((CCarriedReadableDef *)aCStack_114);
                                                                                                        pCVar28 = ""
                                                                                                        quest:StartMovieSequence()
                                                                                                        pCVar4 = 0x1
                                                                                                        quest:PauseAllNonScriptedEntities((pCVar4 ~= 0))
                                                                                                        me:ClearCommands()
                                                                                                        if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") ~= 0 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if alive then
                                                                                                                -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(( CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffe5c);
                                                                                                                fVar15 = quest:GetHealth(r26)
                                                                                                                fVar1 = _DAT_0122dedc
                                                                                                                if fVar1 < fVar15 then
                                                                                                                    aVar29 = 0x0
                                                                                                                    pCVar24 = 0x1
                                                                                                                    pCVar22 = 0x0
                                                                                                                    pCVar21 = 0x0
                                                                                                                    pcVar20 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END"
                                                                                                                    pCVar9 = quest:GetHero()
                                                                                                                    r33 = me:Speak(pCVar9, pcVar20, pCVar21, (pCVar22 ~= 0), (pCVar24 ~= 0), (aVar29 ~= 0))
                                                                                                                    bVar2 = me:IsPerformingScriptTask()
                                                                                                                    if bVar2 then
                                                                                                                        repeat
                                                                                                                            alive = quest:NewScriptFrame(me)
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            if not alive then return end  -- TODO(native): goto LAB_00d555f3
                                                                                                                            bVar2 = me:IsPerformingScriptTask()
                                                                                                                        until not (bVar2)
                                                                                                                    end
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    if not alive then goto LAB_00d55cd5 end
                                                                                                                end
                                                                                                                pCVar21 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION"
                                                                                                                quest:GiveHeroYesNoQuestion("TEXT_QST_028_GUILDMASTER_PREMELEE_END_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                                                                                                iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                while iVar5 < 0 do
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    if not alive then return end  -- TODO(native): goto LAB_00d555f3
                                                                                                                    iVar5 = quest:MsgIsQuestionAnsweredYesOrNo()
                                                                                                                end
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                if alive then
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    if iVar5 == 1 then
                                                                                                                        if alive then
                                                                                                                            -- TODO(native): (**(code **)(**(int **)(this + 4) + 0x5d4 ))();
                                                                                                                            -- TODO(native): (**(code **)(**(int **)(this + 4) + 0x5e0 ))();
                                                                                                                            quest:PlayAVIMovie("Data\\\\Video\\\\2_guild_split_1_comp.xmv")
                                                                                                                            goto LAB_00d5595a
                                                                                                                        end
                                                                                                                        -- TODO(native): goto LAB_00d555f3
                                                                                                                    end
                                                                                                                    if alive then
                                                                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(( CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffe5c);
                                                                                                                        fVar15 = quest:GetHealth(r21)
                                                                                                                        fVar1 = _DAT_0122dedc
                                                                                                                        CVar31 = SUB41(pCVar28,0)
                                                                                                                        if fVar1 < fVar15 then
                                                                                                                            aVar29 = 0x0
                                                                                                                            pCVar24 = 0x1
                                                                                                                            pCVar22 = 0x0
                                                                                                                            pCVar21 = 0x0
                                                                                                                            pcVar20 = "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO"
                                                                                                                            pCVar9 = quest:GetHero()
                                                                                                                            r34 = me:Speak(pCVar9, pcVar20, pCVar21, (pCVar22 ~= 0), (pCVar24 ~= 0), (aVar29 ~= 0))
                                                                                                                            bVar2 = me:IsPerformingScriptTask()
                                                                                                                            CVar31 = SUB41(pCVar28,0)
                                                                                                                            if bVar2 then
                                                                                                                                repeat
                                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                                    if not alive then return end  -- TODO(native): goto LAB_00d555f3
                                                                                                                                    bVar2 = me:IsPerformingScriptTask()
                                                                                                                                    CVar31 = SUB41(pCVar28,0)
                                                                                                                                until not (bVar2)
                                                                                                                            end
                                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                                            if not alive then goto LAB_00d55cd5 end
                                                                                                                        end
                                                                                                                        pCVar25 = "MK_GTM_WD_GUARD"
                                                                                                                        piVar10 = (**(**(this + 4) + 0x120 ))()
                                                                                                                        pCVar21 = 0x1
                                                                                                                        pcVar20 = 0x0
                                                                                                                        pCVar9 = quest:IsXbox()
                                                                                                                        me:MoveToPosition(nil --[[missing]], SUB41(pCVar4,0))
                                                                                                                        -- TODO(native): ppuStack_18c = (undefined **) ((uint)ppuStack_18c & 0xffffff);
                                                                                                                        goto LAB_00d5595a
                                                                                                                    end
                                                                                                                end
                                                                                                            end
                                                                                                            ::LAB_00d55cd5::
                                                                                                            quest:PauseAllNonScriptedEntities((pcVar20 ~= 0))
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        aVar29 = SUB41(pCVar4,0)
                                                                                                        if not alive then
                                                                                                            -- LAB_00d555f3: (native jump target)
                                                                                                            quest:PauseAllNonScriptedEntities((pCVar21 ~= 0))
                                                                                                            goto LAB_00d55c2b
                                                                                                        end
                                                                                                        -- TODO(native): CScriptGameResourceObjectScriptedThingBase::_GetScriptThing_CScriptGameResourceObjectScriptedThingBase__UBE_AVCScriptThing__XZ(( CScriptGameResourceObjectScriptedThingBase *) &stack0xfffffe5c);
                                                                                                        fVar15 = quest:GetHealth(nil --[[missing]])
                                                                                                        fVar1 = _DAT_0122dedc
                                                                                                        if fVar1 < fVar15 then
                                                                                                            aVar30 = 0x0
                                                                                                            pCVar22 = 0x1
                                                                                                            pCVar21 = 0x0
                                                                                                            pCVar28 = 0x0
                                                                                                            pcVar20 = "TEXT_QST_028_GUILDMASTER_PRE_MELEE_BEETLES_NOT_DEAD"
                                                                                                            pCVar4 = quest:GetHero()
                                                                                                            r35 = me:Speak(pCVar4, pcVar20, pCVar28, (pCVar21 ~= 0), (pCVar22 ~= 0), (aVar30 ~= 0))
                                                                                                            bVar2 = me:IsPerformingScriptTask()
                                                                                                            if bVar2 then
                                                                                                                repeat
                                                                                                                    alive = quest:NewScriptFrame(me)
                                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                                    if not alive then return end  -- TODO(native): goto LAB_00d55cd5
                                                                                                                    bVar2 = me:IsPerformingScriptTask()
                                                                                                                until not (bVar2)
                                                                                                            end
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if not alive then return end  -- TODO(native): goto LAB_00d555f3
                                                                                                        end
                                                                                                        ::LAB_00d5595a::
                                                                                                        quest:PauseAllNonScriptedEntities((aVar30 ~= 0))
                                                                                                    end
                                                                                                    if cStack_185 ~= 0 then
                                                                                                        fVar32 = 5.5
                                                                                                        pCVar4 = quest:GetHero()
                                                                                                        bVar2 = quest:IsDistanceBetweenThingsUnder(pCVar4, me, fVar32)
                                                                                                        __native_condition_1 = (bVar2) and (iVar5 = (**(*DAT_0143e8f8 + 0x168) )(), iVar5 < 1)
                                                                                                        if __native_condition_1 then
                                                                                                            bVar2 = me:IsPerformingScriptTask()
                                                                                                            __native_condition_1 = not bVar2
                                                                                                        end
                                                                                                        if __native_condition_1 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if not alive then goto LAB_00d55c2b end
                                                                                                            r36 = quest:AddNewConversation(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                                                                                            r37 = quest:GetHero()
                                                                                                            quest:AddPersonToConversation(nil --[[missing]], r37)
                                                                                                            quest:SetTimer(nil --[[missing]], nil --[[missing]])
                                                                                                            if quest:GetMasterGameState("ScorpionsDestroyedCutscenePlayed") == 0 then
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                if not alive then goto LAB_00d55c2b end
                                                                                                                if "TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC" == 1 then
                                                                                                                    r38 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(r38, nil --[[missing]])
                                                                                                                    r39 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_FIRST", r39, nil --[[missing]])
                                                                                                                    -- LAB_00d55b4e: (native jump target)
                                                                                                                elseif "TEXT_QST_028_ONSCREENHELP_WIELD_HELP_PC" == 2 then
                                                                                                                    r40 = quest:GetHero()
                                                                                                                    quest:EntitySetFacingAngleTowardsThing(r40, nil --[[missing]])
                                                                                                                    r41 = quest:GetHero()
                                                                                                                    quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_PREMELEE_END_NO_BEETLES_COMMENT_SECOND", r41, nil --[[missing]])
                                                                                                                    -- TODO(native): goto LAB_00d55b4e
                                                                                                                end
                                                                                                                -- TODO(native): iStack_170 = 1 - iStack_170;
                                                                                                            else
                                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                                if not alive then goto LAB_00d55c2b end
                                                                                                                r42 = quest:GetHero()
                                                                                                                quest:EntitySetFacingAngleTowardsThing(r42, nil --[[missing]])
                                                                                                                r43 = quest:GetHero()
                                                                                                                quest:AddLineToConversation(nil --[[missing]], "TEXT_QST_028_GUILDMASTER_PREMELEE_END_BEETLES_COMMENT_FIRST", r43, nil --[[missing]])
                                                                                                            end
                                                                                                        end
                                                                                                    end
                                                                                                    if uStack_180._3_1_ == 0 then
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        if not alive then goto LAB_00d55c2b end
                                                                                                        bVar2 = me:IsPerformingScriptTask()
                                                                                                        if not bVar2 then
                                                                                                            alive = not quest:IsActiveThreadTerminating()
                                                                                                            if not alive then goto LAB_00d55c2b end
                                                                                                            -- TODO(native): uStack_180 = CONCAT13(1,(undefined3)uStack_180);
                                                                                                            r44 = quest:GetHero()
                                                                                                            quest:EntitySetFacingAngleTowardsThing(r44, nil --[[missing]])
                                                                                                        end
                                                                                                    end
                                                                                                until not (cStack_185 ~= 0)
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                if extraout_AL_x00100 == 0 then
                                                                                                    quest:SetStateBool("HeroSleeps", true)
                                                                                                    quest:FadeScreenOut(nil --[[missing]], nil --[[missing]])
                                                                                                    quest:SetTimeOfDay(nil --[[missing]])
                                                                                                    quest:ChangeHeroHealthBy(nil --[[missing]], nil --[[missing]], nil --[[missing]])
                                                                                                    quest:ResetPlayerCreatureCombatMultiplier()
                                                                                                end
                                                                                            else
                                                                                                alive = not quest:IsActiveThreadTerminating()
                                                                                                if alive then
                                                                                                    cVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    while not cVar3 do
                                                                                                        alive = quest:NewScriptFrame(me)
                                                                                                        alive = not quest:IsActiveThreadTerminating()
                                                                                                        if not alive then goto LAB_00d55c2b end
                                                                                                        cVar3 = quest:MsgIsTutorialClickedPast()
                                                                                                    end
                                                                                                    alive = not quest:IsActiveThreadTerminating()
                                                                                                    if alive then return end  -- TODO(native): goto LAB_00d54846
                                                                                                end
                                                                                            end
                                                                                        end
                                                                                    end
                                                                                end
                                                                            end
                                                                        end
                                                                    else
                                                                        -- LAB_00d54dfa: (native jump target)
                                                                    end
                                                                end
                                                                ::LAB_00d55c2b::
                                                            end
                                                        end
                                                    end
                                                else
                                                    -- LAB_00d53ff2: (native jump target)
                                                end
                                                ::LAB_00d55c34::
                                            end
                                        end
                                    end
                                end
                            else
                                -- LAB_00d53a0b: (native jump target)
                            end
                        end
                        ::LAB_00d55c3d::
                    end
                end
            end
        else
            -- LAB_00d533bb: (native jump target)
        end
    end
    ::LAB_00d55c46::
    ::LAB_00d55c4f::
end

function Init(quest, me)
    __native_entity_state:SetStateBool("ChatJumped", false)
    __native_entity_state:SetStateBool("WoodsEndPlayed", false)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

