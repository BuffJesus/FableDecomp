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
    local bVar3, bVar4, cVar2, c_stk_165, c_stk_16d, fVar12, fVar13, f_stk_11c, f_stk_120, f_stk_124, f_stk_134, f_stk_138, f_stk_13c, f_stk_14, f_stk_158, f_stk_15c, f_stk_160, f_stk_20, f_stk_2c, f_stk_38, f_stk_6c, f_stk_70, f_stk_78, f_stk_7c, f_stk_84, f_stk_88, f_stk_a8, f_stk_ac, f_stk_b4, f_stk_b8, f_stk_bc, iVar11, iVar8, i_stk_16c, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, pCVar6, pfVar7, piVar1, r1, r2, r3, r4, r5, r6, r7, r8, thing1, uVar5, uVar9, x_stk_188
    local alive = true
    fVar12 = me:GetAngleXY()
    r1 = quest:GetNearestWithScriptName(me, "DummyEndMarker")
    r2 = quest:GetNearestWithScriptName(me, "DummyStartMarker")
    x_stk_188 = nil
    if __native_entity_state:GetStateInt("DummyNumber") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d41f6f end
        uVar5 = __ftol2()
        __native_entity_state:SetStateInt("Speed", uVar5)
        pCVar6 = quest:GetNearestWithScriptName(me, "StaticDummyMarker3")
        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&piStack_190,(int)&*(int *)(pCVar6 + 0x4));
    elseif __native_entity_state:GetStateInt("DummyNumber") == 2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d41f6f end
        uVar5 = __ftol2()
        __native_entity_state:SetStateInt("Speed", uVar5)
        pCVar6 = quest:GetNearestWithScriptName(pCVar6, "StaticDummyMarker2")
        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&piStack_190,(int)&*(int *)(pCVar6 + 0x4));
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d41f6f end
        uVar5 = __ftol2()
        __native_entity_state:SetStateInt("Speed", uVar5)
        pCVar6 = quest:GetNearestWithScriptName(pCVar6, "StaticDummyMarker1")
        -- TODO(native): CCountedPointer<CDiskFileWin32>::operator= ((CCountedPointer<CDiskFileWin32> *)&piStack_190,(int)&*(int *)(pCVar6 + 0x4));
    end
    uVar5 = __ftol2()
    __native_entity_state:SetStateInt("DummyWorth", uVar5)
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar3 = not alive
    if not bVar3 then
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if not bVar3 then
            cVar2 = quest:GetMasterGameState("SkillTrainingStarted")
            i_stk_16c = 0
            while cVar2 ~= '\x01' do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                if bVar3 then goto LAB_00d41f6f end
                cVar2 = quest:GetMasterGameState("SkillTrainingStarted")
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar3 = not alive
            if not bVar3 then
                alive = not quest:IsActiveThreadTerminating()
                bVar3 = not alive
                iVar11 = 0
                fVar13 = fVar12
                repeat
                    if bVar3 then
                        -- LAB_00d42f10: (native jump target)
                        return
                    end
                    pfVar7 = me:GetPos()
                    f_stk_bc = pfVar7.x
                    f_stk_b8 = pfVar7.y
                    f_stk_b4 = pfVar7.z
                    if not (r2 ~= nil and not r2:IsNull()) then
                    else
                        pfVar7 = r2:GetPos()
                    end
                    f_stk_160 = pfVar7.x
                    f_stk_15c = pfVar7.y
                    f_stk_158 = pfVar7.z
                    if not (r1 ~= nil and not r1:IsNull()) then
                    else
                        pfVar7 = r1:GetPos()
                    end
                    f_stk_124 = pfVar7.x
                    f_stk_120 = pfVar7.y
                    f_stk_11c = pfVar7.z
                    if piStack_190 == nil then
                    else
                        pfVar7 = (**(*piStack_190 + 0x18))()
                    end
                    f_stk_84 = __native_entity_state:GetStateInt("Speed")
                    f_stk_13c = *pfVar7
                    f_stk_138 = pfVar7[1]
                    f_stk_134 = pfVar7[2]
                    f_stk_ac = (f_stk_124 - f_stk_160) / f_stk_84
                    c_stk_16d = 0
                    quest:SetMasterGameState("SkillDummyReset", false)
                    f_stk_a8 = (f_stk_120 - f_stk_15c) / f_stk_84
                    f_stk_70 = (f_stk_160 - f_stk_124) / f_stk_84
                    f_stk_6c = (f_stk_15c - f_stk_120) / f_stk_84
                    f_stk_7c = (f_stk_13c - f_stk_bc) / f_stk_84
                    f_stk_78 = (f_stk_138 - f_stk_b8) / f_stk_84
                    f_stk_88 = (f_stk_160 - f_stk_13c) / f_stk_84
                    f_stk_84 = (f_stk_15c - f_stk_138) / f_stk_84
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        f_stk_2c = f_stk_78 * i_stk_16c
                        -- TODO(native): xStack_58 = f_stk_7c * (float)i_stk_16c + f_stk_bc;
                        quest:EntityTeleportToPosition(me, &xStack_58, fVar13, false, false)
                        iVar11 = iVar11 + 1
                        i_stk_16c = iVar11
                    until not (iVar11 ~= __native_entity_state:GetStateInt("Speed"))
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    native_arg_sequence_1 = false
                    if bVar3 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if not native_arg_sequence_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        return
                    end
                    if quest:GetMasterGameState("MovingDummiesNeeded") == 0 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        while true do
                            repeat
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    return
                                end
                                bVar3 = me:MsgIsHitByHeroWithProjectileWeapon()
                            until not (not bVar3)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            if 0.0 < fStack_b0 then break end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            piVar1 = (__native_entity_state:GetStateInt("self_0x18") + 0xa4)
                            -- TODO(native): *piVar1 = *piVar1 + -1;
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        native_arg_sequence_2 = false
                        if bVar3 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                        if not native_arg_sequence_2 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                native_arg_sequence_2 = true
                            else
                                native_arg_sequence_2 = false
                            end
                        end
                        if native_arg_sequence_2 then
                            return
                        end
                        piVar1 = (__native_entity_state:GetStateInt("self_0x18") + 0xa4)
                        -- TODO(native): *piVar1 = *piVar1 + 1;
                        iVar11 = 0
                        i_stk_16c = 0
                        quest:EntitySetTargetable(me, false)
                        repeat
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            f_stk_14 = f_stk_84 * i_stk_16c
                            -- TODO(native): xStack_40 = f_stk_88 * (float)i_stk_16c + f_stk_13c;
                            quest:EntityTeleportToPosition(me, &xStack_40, fVar13, false, false)
                            iVar11 = iVar11 + 1
                            i_stk_16c = iVar11
                        until not (iVar11 ~= __native_entity_state:GetStateInt("Speed"))
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        native_arg_sequence_3 = false
                        if bVar3 then
                            native_arg_sequence_3 = true
                        else
                            native_arg_sequence_3 = false
                        end
                        if not native_arg_sequence_3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                native_arg_sequence_3 = true
                            else
                                native_arg_sequence_3 = false
                            end
                        end
                        if native_arg_sequence_3 then
                            return
                        end
                        cVar2 = quest:GetMasterGameState("MovingDummiesNeeded")
                        while cVar2 ~= '\x01' do
                            alive = quest:NewScriptFrame(me)
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            cVar2 = quest:GetMasterGameState("MovingDummiesNeeded")
                        end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        quest:EntitySetTargetable(me, true)
                    end
                    i_stk_16c = 0
                    -- TODO(native): CTimer::CTimer((CTimer *)&xStack_190);
                    quest:SetTimer(xStack_190, 0)
                    c_stk_165 = 0
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            return
                        end
                        if c_stk_16d == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            f_stk_20 = f_stk_a8 * i_stk_16c
                            -- TODO(native): fStack_a0 = f_stk_ac * (float)i_stk_16c + f_stk_160;
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            f_stk_38 = f_stk_6c * i_stk_16c
                            -- TODO(native): fStack_58 = f_stk_70 * (float)i_stk_16c + f_stk_124;
                        end
                        quest:EntityTeleportToPosition(me, pPos, fVar13, false, false)
                        bVar3 = me:MsgIsHitByHeroWithProjectileWeapon()
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            if quest:GetMasterGameState("MovingDummiesNeeded") == '\x01' then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    return
                                end
                                fVar13 = 6.0
                                pCVar6 = quest:GetThingWithScriptName("ArcheryRing")
                                thing1 = quest:GetHero()
                                bVar3 = quest:IsDistanceBetweenThingsOver(thing1, pCVar6, fVar13)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar3 then
                                    if bVar4 then
                                        return
                                    end
                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingSkill")
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar3 then
                                        if bVar4 then
                                            return
                                        end
                                        r3 = quest:GetThingWithScriptName("TheGuildmaster")
                                        iVar8 = quest:AddNewConversation(r3, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_MAZE_HIT_OUT", pCVar6, thing1, false)
                                    else
                                        if bVar4 then
                                            return
                                        end
                                        r4 = quest:GetThingWithScriptName("SkillApprentice")
                                        iVar8 = quest:AddNewConversation(r4, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_HIT_OUT", pCVar6, r2, false)
                                    end
                                else
                                    if bVar4 then
                                        return
                                    end
                                    pCVar6 = quest:GetHero()
                                    r5 = quest:PlaySoundOnThing(pCVar6, "SND_ARROWIMPACT_02")
                                    if 0.0 <= xStack_18c then
                                        if 0.0 <= xStack_18c then
                                            if xStack_18c < 0.0 then
                                                alive = not quest:IsActiveThreadTerminating()
                                                bVar3 = not alive
                                                if not bVar3 then
                                                    quest:SetMasterGameState("SkillScore", quest:GetMasterGameState("SkillScore") + __native_entity_state:GetStateInt("DummyWorth") * 3)
                                                    goto LAB_00d42874
                                                end
                                                return
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                return
                                            end
                                            iVar11 = __native_entity_state:GetStateInt("DummyWorth") << 2
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                return
                                            end
                                            iVar11 = __native_entity_state:GetStateInt("DummyWorth") << 1
                                        end
                                        piVar1 = (__native_entity_state:GetStateInt("self_0x18") + 0xa4)
                                        -- TODO(native): *piVar1 = *piVar1 + iVar11;
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            return
                                        end
                                        piVar1 = (__native_entity_state:GetStateInt("self_0x18") + 0xa4)
                                        -- TODO(native): *piVar1 = *piVar1 + *(int *)(this + 0x24);
                                    end
                                    ::LAB_00d42874::
                                    iVar11 = quest:GetTimer(xStack_190)
                                    if iVar11 < 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            return
                                        end
                                        uVar9 = uVar9 & 0x80000003
                                        bVar3 = uVar9 == 0
                                        if uVar9 < 0 then
                                            bVar3 = (uVar9 - 1 | 0xfffffffc) == 0xffffffff
                                        end
                                        if bVar3 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                return
                                            end
                                            quest:SetTimer(xStack_190, 8)
                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingSkill")
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar3 then
                                                if bVar4 then
                                                    return
                                                end
                                                r6 = quest:GetThingWithScriptName("TheRealGuildmaster")
                                                iVar8 = quest:AddNewConversation(r6, false, false)
                                                pCVar6 = quest:GetHero()
                                                quest:AddPersonToConversation(iVar8, pCVar6)
                                                if 0.0 <= xStack_18c then
                                                    if 0.0 <= xStack_18c then
                                                        if 0.0 <= xStack_18c then
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then __cleanup_LAB_00d42ef8(); return end
                                                            pCVar6 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_SKILL_EXCELLENT_HIT", pCVar6, r1, false)
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then
                                                                __cleanup_LAB_00d42ef8()
                                                                return
                                                            end
                                                            pCVar6 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_SKILL_GOOD_HIT", pCVar6, nil --[[missing]], false)
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then __cleanup_LAB_00d42ef8(); return end
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_SKILL_MEDIUM_HIT", pCVar6, nil --[[missing]], false)
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then __cleanup_LAB_00d42ef8(); return end
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", pCVar6, nil --[[missing]], false)
                                                end
                                            else
                                                if bVar4 then
                                                    return
                                                end
                                                r7 = quest:GetThingWithScriptName("SkillApprentice")
                                                iVar8 = quest:AddNewConversation(r7, false, false)
                                                pCVar6 = quest:GetHero()
                                                quest:AddPersonToConversation(iVar8, pCVar6)
                                                if 0.0 <= xStack_18c then
                                                    if 0.0 <= xStack_18c then
                                                        if 0.0 <= xStack_18c then
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then return end  -- TODO(native): goto LAB_00d42efe
                                                            pCVar6 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_EXCELLENT_HIT", pCVar6, nil --[[missing]], false)
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then
                                                                -- LAB_00d42efe: (native jump target)
                                                                return
                                                            end
                                                            pCVar6 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_GOOD_HIT", pCVar6, nil --[[missing]], false)
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then return end  -- TODO(native): goto LAB_00d42efe
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_MEDIUM_HIT", pCVar6, nil --[[missing]], false)
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then return end  -- TODO(native): goto LAB_00d42efe
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_SOFT_HIT", pCVar6, nil --[[missing]], false)
                                                end
                                            end
                                        end
                                    end
                                    quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        return
                                    end
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        return
                                    end
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    return
                                end
                                pCVar6 = quest:GetHero()
                                r8 = quest:PlaySoundOnThing(pCVar6, "SND_ARROWIMPACT_02")
                                quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
                            end
                        end
                        i_stk_16c = i_stk_16c + 1
                        if i_stk_16c == __native_entity_state:GetStateInt("Speed") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            c_stk_16d = c_stk_16d == 0
                            i_stk_16c = 0
                        end
                        if quest:GetMasterGameState("SkillRepeatKnown") ~= 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                            if quest:GetMasterGameState("SkillRepeating") == '\x01' then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    return
                                end
                                c_stk_165 = '\x01'
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    return
                                end
                            end
                            quest:SetMasterGameState("SkillDummyReset", true)
                            cVar2 = quest:GetMasterGameState("SkillRepeatKnown")
                            while cVar2 ~= 0 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    return
                                end
                                cVar2 = quest:GetMasterGameState("SkillRepeatKnown")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                return
                            end
                        end
                    until not (c_stk_165 == 0)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- LAB_00d42f0b: (native jump target)
                        return
                    end
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                until false
            end
        end
    end
    ::LAB_00d41f6f::
end

function Init(quest, me)
    local this_00 = me:GetDataString()
    local iVar1 = GFCharStringToInt(this_00)
    __native_entity_state:SetStateInt("DummyNumber", iVar1)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

