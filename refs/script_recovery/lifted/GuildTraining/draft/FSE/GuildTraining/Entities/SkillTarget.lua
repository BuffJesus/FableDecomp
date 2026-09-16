-- Generated native draft: SkillTarget. Review coverage report before use.
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
    local bVar4, cVar3, fVar16, fVar18, iVar6, pCVar14, pCVar17, pCVar20, pCVar8, pCVar9, pfVar7, ppVar12, r1, r2, r3, r4, r5, r6, r7, r8, uVar10, uVar11, uVar5
    local alive = true
    fVar16 = me:GetAngleXY()
    r1 = quest:GetNearestWithScriptName(nil --[[missing]], "DummyEndMarker")
    r2 = quest:GetNearestWithScriptName(r1, "DummyStartMarker")
    if __native_entity_state:GetStateInt("DummyNumber") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d41f6f end
        uVar5 = __ftol2()
        __native_entity_state:SetStateInt("Speed", uVar5)
        iVar6 = quest:GetNearestWithScriptName(me, "StaticDummyMarker3")
        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&stack0xfffffe4c, (CCountedPointer<class_CDiskFileWin32> *)(iVar6 + 4));
    elseif __native_entity_state:GetStateInt("DummyNumber") == 2 then
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d41f6f end
        uVar5 = __ftol2()
        __native_entity_state:SetStateInt("Speed", uVar5)
        ppVar12 = iVar6
        iVar6 = quest:GetNearestWithScriptName(ppVar12, "StaticDummyMarker2")
        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&stack0xfffffe4c, (CCountedPointer<class_CDiskFileWin32> *)(iVar6 + 4));
    else
        alive = not quest:IsActiveThreadTerminating()
        if not alive then goto LAB_00d41f6f end
        uVar5 = __ftol2()
        __native_entity_state:SetStateInt("Speed", uVar5)
        ppVar12 = iVar6
        iVar6 = quest:GetNearestWithScriptName(ppVar12, "StaticDummyMarker1")
        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&stack0xfffffe4c, (CCountedPointer<class_CDiskFileWin32> *)(iVar6 + 4));
    end
    uVar5 = __ftol2()
    __native_entity_state:SetStateInt("DummyWorth", uVar5)
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            cVar3 = quest:GetMasterGameState("SkillTrainingStarted")
            while cVar3 ~= '\x01' do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then goto LAB_00d41f6f end
                cVar3 = quest:GetMasterGameState("SkillTrainingStarted")
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                alive = not quest:IsActiveThreadTerminating()
                iVar6 = 0
                cVar3 = extraout_AL_06
                repeat
                    if cVar3 ~= 0 then
                        -- LAB_00d42f10: (native jump target)
                        return
                    end
                    pfVar7 = me:GetPos()
                    -- TODO(native): fStack_d4 = *pfVar7;
                    -- TODO(native): fStack_d0 = pfVar7[1];
                    -- TODO(native): fStack_cc = pfVar7[2];
                    if piStack_19c == nil then
                    else
                        pfVar7 = (**(*piStack_19c + 0x18))()
                    end
                    -- TODO(native): fStack_178 = *pfVar7;
                    -- TODO(native): fStack_174 = pfVar7[1];
                    -- TODO(native): fStack_170 = pfVar7[2];
                    if piStack_190 == nil then
                    else
                        pfVar7 = (**(*piStack_190 + 0x18))()
                    end
                    -- TODO(native): fVar2 = DAT_0143e8e4;
                    -- TODO(native): fVar18 = DAT_0143e8e0;
                    -- TODO(native): fStack_13c = *pfVar7;
                    -- TODO(native): fStack_138 = pfVar7[1];
                    -- TODO(native): fStack_134 = pfVar7[2];
                    -- TODO(native): fStack_9c = (float)*(int *)(this + 0x1c);
                    -- TODO(native): fStack_154 = DAT_0143e8e0;
                    -- TODO(native): fStack_150 = DAT_0143e8e4;
                    -- TODO(native): uStack_14c = DAT_0143e8e8;
                    -- TODO(native): fStack_c4 = (fStack_13c - fStack_178) / fStack_9c;
                    -- TODO(native): cStack_185 = '\0';
                    quest:SetMasterGameState("SkillDummyReset", false)
                    -- TODO(native): fStack_c0 = (fStack_138 - fStack_174) / fStack_9c;
                    -- TODO(native): fStack_88 = (fStack_178 - fStack_13c) / fStack_9c;
                    -- TODO(native): fStack_84 = (fStack_174 - fStack_138) / fStack_9c;
                    -- TODO(native): fStack_94 = (fVar18 - fStack_d4) / fStack_9c;
                    -- TODO(native): fStack_90 = (fVar2 - fStack_d0) / fStack_9c;
                    -- TODO(native): fStack_a0 = (fStack_178 - fVar18) / fStack_9c;
                    -- TODO(native): fStack_9c = (fStack_174 - fVar2) / fStack_9c;
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d42f10
                        -- TODO(native): fStack_74 = fStack_cc;
                        -- TODO(native): fStack_44 = fStack_90 * (float)iStack_184;
                        -- TODO(native): fStack_7c = fStack_94 * (float)iStack_184 + fStack_d4;
                        -- TODO(native): fStack_78 = fStack_44 + fStack_d0;
                        quest:EntityTeleportToPosition(me, 0, 0)
                        iVar6 = iVar6 + 1
                    until not (iVar6 ~= __native_entity_state:GetStateInt("Speed"))
                    alive = not quest:IsActiveThreadTerminating()
                    if (not alive) or (function() alive = not quest:IsActiveThreadTerminating(); return not alive end)() then return end  -- TODO(native): goto LAB_00d42f10
                    if quest:GetMasterGameState("MovingDummiesNeeded") == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d42f10
                        while true do
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d42f10
                                cVar3 = me:MsgIsHitByHeroWithProjectileWeapon()
                            until not (not cVar3)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f10
                            if _DAT_012316f0 < fStack_c8 then break end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f10
                            quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + -1)
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if (not alive) or (function() alive = not quest:IsActiveThreadTerminating(); return not alive end)() then return end  -- TODO(native): goto LAB_00d42f10
                        quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + 1)
                        iVar6 = 0
                        quest:EntitySetTargetable(me, false)
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f10
                            -- TODO(native): uStack_5c = uStack_14c;
                            -- TODO(native): fStack_2c = fStack_9c * (float)iStack_184;
                            -- TODO(native): fStack_64 = fStack_a0 * (float)iStack_184 + fStack_154;
                            -- TODO(native): fStack_60 = fStack_2c + fStack_150;
                            quest:EntityTeleportToPosition(me, 0, 0)
                            iVar6 = iVar6 + 1
                        until not (iVar6 ~= __native_entity_state:GetStateInt("Speed"))
                        alive = not quest:IsActiveThreadTerminating()
                        if (not alive) or (function() alive = not quest:IsActiveThreadTerminating(); return not alive end)() then return end  -- TODO(native): goto LAB_00d42f10
                        cVar3 = quest:GetMasterGameState("MovingDummiesNeeded")
                        while cVar3 ~= '\x01' do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f10
                            cVar3 = quest:GetMasterGameState("MovingDummiesNeeded")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d42f10
                        quest:EntitySetTargetable(me, true)
                    end
                    -- TODO(native): CTimer::CTimer((CTimer *)&stack0xfffffe4c);
                    quest:SetTimer(ppVar19, 0)
                    -- TODO(native): cStack_17d = '\0';
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                        if cStack_185 == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                            -- TODO(native): fStack_38 = fStack_c0 * (float)iStack_184;
                            -- TODO(native): fStack_b0 = fStack_170;
                            -- TODO(native): fStack_b8 = fStack_c4 * (float)iStack_184 + fStack_178;
                            -- TODO(native): fStack_b4 = fStack_38 + fStack_174;
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                            -- TODO(native): fStack_50 = fStack_84 * (float)iStack_184;
                            -- TODO(native): fStack_68 = fStack_134;
                            -- TODO(native): fStack_70 = fStack_88 * (float)iStack_184 + fStack_13c;
                            -- TODO(native): fStack_6c = fStack_50 + fStack_138;
                        end
                        quest:EntityTeleportToPosition(me, 0, 0)
                        cVar3 = me:MsgIsHitByHeroWithProjectileWeapon()
                        if cVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                            if quest:GetMasterGameState("MovingDummiesNeeded") == '\x01' then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                fVar18 = 6.0
                                -- TODO(native): piStack_58 = piVar1;
                                pCVar8 = quest:GetThingWithScriptName("ArcheryRing")
                                pCVar9 = quest:GetHero()
                                bVar4 = IsDistanceBetweenThingsOver(pCVar9,pCVar8,fVar18)
                                alive = not quest:IsActiveThreadTerminating()
                                if bVar4 then
                                    if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                    cVar3 = quest:IsQuestActive("Q_GuildTrainingSkill")
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not cVar3 then
                                        if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                        ppVar12 = iVar6
                                        r3 = quest:GetThingWithScriptName("SkillApprentice")
                                        uVar5 = quest:AddNewConversation(r3, false, false)
                                        uVar10 = quest:GetHero()
                                        quest:AddPersonToConversation(uVar5, uVar10)
                                        uVar10 = quest:GetHero()
                                        quest:AddLineToConversation(uVar5, "TEXT_QST_028_APPRENTICE_SKILL_HIT_OUT", uVar10, pCVar9, false)
                                    else
                                        if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                        r4 = quest:GetThingWithScriptName("TheGuildmaster")
                                        uVar5 = quest:AddNewConversation(r4, false, false)
                                        uVar10 = quest:GetHero()
                                        quest:AddPersonToConversation(uVar5, uVar10)
                                        uVar10 = quest:GetHero()
                                        quest:AddLineToConversation(uVar5, "TEXT_QST_028_MAZE_HIT_OUT", uVar10, pCVar8, false)
                                    end
                                else
                                    if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                    uVar5 = quest:GetHero()
                                    r5 = quest:PlaySoundOnThing(uVar5, "SND_ARROWIMPACT_02")
                                    if _DAT_01238010 <= me then
                                        if _DAT_012316f0 <= me then
                                            if me < _DAT_0129f0b0 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                if alive then
                                                    quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + __native_entity_state:GetStateInt("DummyWorth") * 3)
                                                    goto LAB_00d42874
                                                end
                                                -- TODO(native): goto LAB_00d42f0b
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                            iVar6 = __native_entity_state:GetStateInt("DummyWorth") << 2
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                            iVar6 = __native_entity_state:GetStateInt("DummyWorth") << 1
                                        end
                                        quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + iVar6)
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                        quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + __native_entity_state:GetStateInt("DummyWorth"))
                                    end
                                    ::LAB_00d42874::
                                    iVar6 = quest:GetTimer(ppVar19)
                                    if iVar6 < 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                        uVar11 = rand()
                                        uVar11 = uVar11 & 0x80000003
                                        bVar4 = uVar11 == 0
                                        if uVar11 < 0 then
                                            bVar4 = (uVar11 - 1 | 0xfffffffc) == 0xffffffff
                                        end
                                        if bVar4 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                            quest:SetTimer(ppVar19, 8)
                                            cVar3 = quest:IsQuestActive("Q_GuildTrainingSkill")
                                            alive = not quest:IsActiveThreadTerminating()
                                            if not cVar3 then
                                                if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                                r6 = quest:GetThingWithScriptName("SkillApprentice")
                                                uVar5 = quest:AddNewConversation(r6, auStack_168, false)
                                                uVar10 = quest:GetHero()
                                                quest:AddPersonToConversation(uVar5, uVar10)
                                                if _DAT_01238010 <= me then
                                                    if _DAT_012316f0 <= me then
                                                        if _DAT_0129f0b0 <= me then
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if not alive then return end  -- TODO(native): goto LAB_00d42efe
                                                            uVar10 = quest:GetHero()
                                                            quest:AddLineToConversation(uVar5, "TEXT_QST_028_APPRENTICE_SKILL_EXCELLENT_HIT", r6, uVar10, false)
                                                            -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_104;
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if not alive then
                                                                -- LAB_00d42efe: (native jump target)
                                                                pCVar14 = r6
                                                                -- TODO(native): goto LAB_00d42f02
                                                            end
                                                            uVar10 = quest:GetHero()
                                                            quest:AddLineToConversation(uVar5, "TEXT_QST_028_APPRENTICE_SKILL_GOOD_HIT", r6, uVar10, false)
                                                            -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_d8;
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then return end  -- TODO(native): goto LAB_00d42efe
                                                        uVar10 = quest:GetHero()
                                                        quest:AddLineToConversation(uVar5, "TEXT_QST_028_APPRENTICE_SKILL_MEDIUM_HIT", r6, uVar10, false)
                                                        -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> *)aCStack_e0;
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then return end  -- TODO(native): goto LAB_00d42efe
                                                    uVar10 = quest:GetHero()
                                                    quest:AddLineToConversation(uVar5, "TEXT_QST_028_APPRENTICE_SKILL_SOFT_HIT", r6, uVar10, false)
                                                    -- TODO(native): paVar13 = (allocator<std::pair<EHeroMorphType,CParticleMorphs::CEntry>_> * )aCStack_110;
                                                end
                                                pCVar14 = r6
                                            else
                                                if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                                r7 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                uVar5 = quest:AddNewConversation(r7, false, false)
                                                uVar10 = quest:GetHero()
                                                quest:AddPersonToConversation(uVar5, uVar10)
                                                if _DAT_01238010 <= me then
                                                    if _DAT_012316f0 <= me then
                                                        if _DAT_0129f0b0 <= me then
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if not alive then return end  -- TODO(native): goto LAB_00d42ef8
                                                            uVar10 = quest:GetHero()
                                                            quest:AddLineToConversation(uVar5, "TEXT_QST_028_GUILDMASTER_SKILL_EXCELLENT_HIT", r7, uVar10, false)
                                                            pCVar14 = r7
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            if not alive then
                                                                -- LAB_00d42ef8: (native jump target)
                                                                pCVar14 = r7
                                                                -- LAB_00d42f02: (native jump target)
                                                                -- TODO(native): goto LAB_00d42f0b
                                                            end
                                                            uVar10 = quest:GetHero()
                                                            quest:AddLineToConversation(uVar5, "TEXT_QST_028_GUILDMASTER_SKILL_GOOD_HIT", r7, uVar10, false)
                                                            pCVar14 = r7
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        if not alive then return end  -- TODO(native): goto LAB_00d42ef8
                                                        uVar10 = quest:GetHero()
                                                        quest:AddLineToConversation(uVar5, "TEXT_QST_028_GUILDMASTER_SKILL_MEDIUM_HIT", r7, uVar10, false)
                                                        pCVar14 = r7
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    if not alive then return end  -- TODO(native): goto LAB_00d42ef8
                                                    uVar10 = quest:GetHero()
                                                    quest:AddLineToConversation(uVar5, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", r7, uVar10, false)
                                                    pCVar14 = r7
                                                end
                                            end
                                        end
                                    end
                                    quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                ppVar12 = quest:GetHero()
                                r8 = quest:PlaySoundOnThing(ppVar12, "SND_ARROWIMPACT_02")
                                quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
                            end
                        end
                        -- TODO(native): iStack_184 = iStack_184 + 1;
                        if iStack_184 == __native_entity_state:GetStateInt("Speed") then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                            -- TODO(native): cStack_185 = cStack_185 == '\0';
                        end
                        if quest:GetMasterGameState("SkillRepeatKnown") ~= 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                            if quest:GetMasterGameState("SkillRepeating") == '\x01' then
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                -- TODO(native): cStack_17d = '\x01';
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                            end
                            quest:SetMasterGameState("SkillDummyReset", true)
                            cVar3 = quest:GetMasterGameState("SkillRepeatKnown")
                            while cVar3 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                                cVar3 = quest:GetMasterGameState("SkillRepeatKnown")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            if not alive then return end  -- TODO(native): goto LAB_00d42f0b
                        end
                    until not (cStack_17d == 0)
                    alive = not quest:IsActiveThreadTerminating()
                    if not alive then
                        -- LAB_00d42f0b: (native jump target)
                        -- TODO(native): goto LAB_00d42f10
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    iVar6 = 0
                    cVar3 = extraout_AL_53
                until false
            end
        end
    end
    ::LAB_00d41f6f::
end

function Init(quest, me)
    -- TODO(native): iStack_4 = this;
    local pCVar1 = me:GetDataString()
    local lVar2 = GFCharStringToInt(pCVar1)
    __native_entity_state:SetStateInt("DummyNumber", lVar2)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

