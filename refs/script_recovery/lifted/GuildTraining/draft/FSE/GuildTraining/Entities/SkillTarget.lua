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
    local bVar3, bVar4, cVar2, c_stk_165, c_stk_16d, fStack_b0, fVar11, fVar12, f_stk_11c, f_stk_120, f_stk_124, f_stk_134, f_stk_138, f_stk_13c, f_stk_14, f_stk_158, f_stk_15c, f_stk_160, f_stk_20, f_stk_2c, f_stk_38, f_stk_6c, f_stk_70, f_stk_78, f_stk_7c, f_stk_84, f_stk_88, f_stk_a8, f_stk_ac, f_stk_b4, f_stk_b8, f_stk_bc, iVar5, iVar8, i_stk_16c, native_arg_sequence_1, native_arg_sequence_2, native_arg_sequence_3, pCVar6, pPos, pfVar7, piStack_190, piVar1, r1, r2, r3, r4, r5, r6, r7, r8, thing1, uVar9, xStack_158, xStack_18c, xStack_190, x_stk_188
    local alive = true
    local function __cleanup_LAB_00d42ef8()
        quest:DeregisterTimer(xStack_190)
    end
    local function __cleanup_LAB_00d42efe()
        quest:DeregisterTimer(xStack_190)
    end
    local function __cleanup_LAB_00d42f02()
        quest:DeregisterTimer(xStack_190)
    end
    fVar11 = me:GetAngleXY()
    xStack_158 = fVar11
    r1 = quest:GetNearestWithScriptName(me, "DummyEndMarker")
    r2 = quest:GetNearestWithScriptName(me, "DummyStartMarker")
    x_stk_188 = nil
    if __native_entity_state:GetStateInt("DummyNumber") == 1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d41f6f end
        iVar5 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xeec)))
        __native_entity_state:SetStateInt("Speed", iVar5)
        pCVar6 = quest:GetNearestWithScriptName(me, "StaticDummyMarker3")
        piStack_190 = pCVar6
        fVar11 = quest:ReadGlobalGameDataFloat(0xef8)
    elseif __native_entity_state:GetStateInt("DummyNumber") == 2 then
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d41f6f end
        iVar5 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xee8)))
        __native_entity_state:SetStateInt("Speed", iVar5)
        pCVar6 = quest:GetNearestWithScriptName(pCVar6, "StaticDummyMarker2")
        piStack_190 = pCVar6
        fVar11 = quest:ReadGlobalGameDataFloat(0xef4)
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar3 = not alive
        if bVar3 then goto LAB_00d41f6f end
        iVar5 = math.tointeger(math.modf(quest:ReadGlobalGameDataFloat(0xee4)))
        __native_entity_state:SetStateInt("Speed", iVar5)
        pCVar6 = quest:GetNearestWithScriptName(pCVar6, "StaticDummyMarker1")
        piStack_190 = pCVar6
        fVar11 = quest:ReadGlobalGameDataFloat(0xef0)
    end
    iVar5 = math.tointeger(math.modf(fVar11))
    __native_entity_state:SetStateInt("DummyWorth", iVar5)
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
            while not cVar2 do
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
                iVar5 = 0
                fVar12 = xStack_158
                repeat
                    xStack_158 = fVar12
                    if bVar3 then
                        -- LAB_00d42f10: (native jump target)
                        return
                    end
                    pfVar7 = me:GetPos()
                    f_stk_bc = pfVar7.x
                    f_stk_b8 = pfVar7.y
                    f_stk_b4 = pfVar7.z
                    if not (r2 ~= nil and not r2:IsNull()) then
                        pfVar7 = {x = 0, y = 0, z = 0}
                    else
                        pfVar7 = r2:GetPos()
                    end
                    f_stk_160 = pfVar7.x
                    f_stk_15c = pfVar7.y
                    f_stk_158 = pfVar7.z
                    if not (r1 ~= nil and not r1:IsNull()) then
                        pfVar7 = {x = 0, y = 0, z = 0}
                    else
                        pfVar7 = r1:GetPos()
                    end
                    f_stk_124 = pfVar7.x
                    f_stk_120 = pfVar7.y
                    f_stk_11c = pfVar7.z
                    if not (piStack_190 ~= nil and not piStack_190:IsNull()) then
                        pfVar7 = {x = 0, y = 0, z = 0}
                    else
                        pfVar7 = piStack_190:GetPos()
                    end
                    f_stk_84 = __native_entity_state:GetStateInt("Speed")
                    f_stk_13c = pfVar7.x
                    f_stk_138 = pfVar7.y
                    f_stk_134 = pfVar7.z
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
                        -- TODO(native): quest:EntityTeleportToPosition(me, &xStack_58, fVar12, false, false)
                        iVar5 = iVar5 + 1
                        i_stk_16c = iVar5
                    until not (iVar5 ~= __native_entity_state:GetStateInt("Speed"))
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
                    if not quest:GetMasterGameState("MovingDummiesNeeded") then
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
                                fStack_b0 = me:MsgIsHitByHeroWithProjectileWeapon()
                                bVar3 = fStack_b0 ~= nil
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
                        iVar5 = 0
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
                            -- TODO(native): quest:EntityTeleportToPosition(me, &xStack_40, fVar12, false, false)
                            iVar5 = iVar5 + 1
                            i_stk_16c = iVar5
                        until not (iVar5 ~= __native_entity_state:GetStateInt("Speed"))
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
                        while not cVar2 do
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
                    xStack_190 = quest:RegisterTimer()
                    quest:SetTimer(xStack_190, 0)
                    c_stk_165 = 0
                    repeat
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        bVar3 = not alive
                        if bVar3 then
                            quest:DeregisterTimer(xStack_190)
                            return
                        end
                        if c_stk_16d == 0 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:DeregisterTimer(xStack_190)
                                return
                            end
                            f_stk_20 = f_stk_a8 * i_stk_16c
                            -- TODO(native): fStack_a0 = f_stk_ac * (float)i_stk_16c + f_stk_160;
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:DeregisterTimer(xStack_190)
                                return
                            end
                            f_stk_38 = f_stk_6c * i_stk_16c
                            -- TODO(native): fStack_58 = f_stk_70 * (float)i_stk_16c + f_stk_124;
                        end
                        quest:EntityTeleportToPosition(me, pPos, xStack_158, false, false)
                        xStack_18c = me:MsgIsHitByHeroWithProjectileWeapon()
                        bVar3 = xStack_18c ~= nil
                        if bVar3 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:DeregisterTimer(xStack_190)
                                return
                            end
                            if quest:GetMasterGameState("MovingDummiesNeeded") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:DeregisterTimer(xStack_190)
                                    return
                                end
                                fVar12 = 6.0
                                pCVar6 = quest:GetThingWithScriptName("ArcheryRing")
                                thing1 = quest:GetHero()
                                bVar3 = quest:IsDistanceBetweenThingsOver(thing1, pCVar6, fVar12)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar4 = not alive
                                if bVar3 then
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_190)
                                        return
                                    end
                                    bVar3 = quest:IsQuestActive("Q_GuildTrainingSkill")
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar4 = not alive
                                    if bVar3 then
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_190)
                                            return
                                        end
                                        r3 = quest:GetThingWithScriptName("TheGuildmaster")
                                        iVar8 = quest:AddNewConversation(r3, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_MAZE_HIT_OUT", r3, pCVar6, false)
                                    else
                                        if bVar4 then
                                            quest:DeregisterTimer(xStack_190)
                                            return
                                        end
                                        r4 = quest:GetThingWithScriptName("SkillApprentice")
                                        iVar8 = quest:AddNewConversation(r4, false, false)
                                        pCVar6 = quest:GetHero()
                                        quest:AddPersonToConversation(iVar8, pCVar6)
                                        pCVar6 = quest:GetHero()
                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_HIT_OUT", r4, pCVar6, false)
                                    end
                                else
                                    if bVar4 then
                                        quest:DeregisterTimer(xStack_190)
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
                                                quest:DeregisterTimer(xStack_190)
                                                return
                                            end
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                quest:DeregisterTimer(xStack_190)
                                                return
                                            end
                                            iVar5 = __native_entity_state:GetStateInt("DummyWorth") << 2
                                        else
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                quest:DeregisterTimer(xStack_190)
                                                return
                                            end
                                            iVar5 = __native_entity_state:GetStateInt("DummyWorth") << 1
                                        end
                                        piVar1 = (__native_entity_state:GetStateInt("self_0x18") + 0xa4)
                                        -- TODO(native): *piVar1 = *piVar1 + iVar5;
                                    else
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:DeregisterTimer(xStack_190)
                                            return
                                        end
                                        piVar1 = (__native_entity_state:GetStateInt("self_0x18") + 0xa4)
                                        -- TODO(native): *piVar1 = *piVar1 + *(int *)(this + 0x24);
                                    end
                                    ::LAB_00d42874::
                                    iVar5 = quest:GetTimer(xStack_190)
                                    if iVar5 < 1 then
                                        alive = not quest:IsActiveThreadTerminating()
                                        bVar3 = not alive
                                        if bVar3 then
                                            quest:DeregisterTimer(xStack_190)
                                            return
                                        end
                                        uVar9 = math.random(0, 32767)
                                        uVar9 = uVar9 & 0x80000003
                                        bVar3 = uVar9 == 0
                                        if uVar9 < 0 then
                                            bVar3 = (uVar9 - 1 | 0xfffffffc) == 0xffffffff
                                        end
                                        if bVar3 then
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar3 = not alive
                                            if bVar3 then
                                                quest:DeregisterTimer(xStack_190)
                                                return
                                            end
                                            quest:SetTimer(xStack_190, 8)
                                            bVar3 = quest:IsQuestActive("Q_GuildTrainingSkill")
                                            alive = not quest:IsActiveThreadTerminating()
                                            bVar4 = not alive
                                            if bVar3 then
                                                if bVar4 then
                                                    quest:DeregisterTimer(xStack_190)
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
                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_SKILL_EXCELLENT_HIT", r6, pCVar6, false)
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then
                                                                __cleanup_LAB_00d42ef8()
                                                                return
                                                            end
                                                            pCVar6 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_SKILL_GOOD_HIT", r6, pCVar6, false)
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then __cleanup_LAB_00d42ef8(); return end
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_SKILL_MEDIUM_HIT", r6, pCVar6, false)
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then __cleanup_LAB_00d42ef8(); return end
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_GUILDMASTER_SKILL_SOFT_HIT", r6, pCVar6, false)
                                                end
                                            else
                                                if bVar4 then
                                                    quest:DeregisterTimer(xStack_190)
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
                                                            if bVar3 then __cleanup_LAB_00d42efe(); return end
                                                            pCVar6 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_EXCELLENT_HIT", r7, pCVar6, false)
                                                        else
                                                            alive = not quest:IsActiveThreadTerminating()
                                                            bVar3 = not alive
                                                            if bVar3 then
                                                                __cleanup_LAB_00d42efe()
                                                                return
                                                            end
                                                            pCVar6 = quest:GetHero()
                                                            quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_GOOD_HIT", r7, pCVar6, false)
                                                        end
                                                    else
                                                        alive = not quest:IsActiveThreadTerminating()
                                                        bVar3 = not alive
                                                        if bVar3 then __cleanup_LAB_00d42efe(); return end
                                                        pCVar6 = quest:GetHero()
                                                        quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_MEDIUM_HIT", r7, pCVar6, false)
                                                    end
                                                else
                                                    alive = not quest:IsActiveThreadTerminating()
                                                    bVar3 = not alive
                                                    if bVar3 then __cleanup_LAB_00d42efe(); return end
                                                    pCVar6 = quest:GetHero()
                                                    quest:AddLineToConversation(iVar8, "TEXT_QST_028_APPRENTICE_SKILL_SOFT_HIT", r7, pCVar6, false)
                                                end
                                            end
                                        end
                                    end
                                    quest:EntityPlayObjectAnimation(me, "GET_HIT", false)
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:DeregisterTimer(xStack_190)
                                        return
                                    end
                                    alive = quest:NewScriptFrame(me)
                                    alive = not quest:IsActiveThreadTerminating()
                                    bVar3 = not alive
                                    if bVar3 then
                                        quest:DeregisterTimer(xStack_190)
                                        return
                                    end
                                end
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:DeregisterTimer(xStack_190)
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
                                quest:DeregisterTimer(xStack_190)
                                return
                            end
                            c_stk_16d = c_stk_16d == 0
                            i_stk_16c = 0
                        end
                        if quest:GetMasterGameState("SkillRepeatKnown") then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:DeregisterTimer(xStack_190)
                                return
                            end
                            if quest:GetMasterGameState("SkillRepeating") then
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:DeregisterTimer(xStack_190)
                                    return
                                end
                                c_stk_165 = 1
                            else
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:DeregisterTimer(xStack_190)
                                    return
                                end
                            end
                            quest:SetMasterGameState("SkillDummyReset", true)
                            cVar2 = quest:GetMasterGameState("SkillRepeatKnown")
                            while cVar2 do
                                alive = quest:NewScriptFrame(me)
                                alive = not quest:IsActiveThreadTerminating()
                                bVar3 = not alive
                                if bVar3 then
                                    quest:DeregisterTimer(xStack_190)
                                    return
                                end
                                cVar2 = quest:GetMasterGameState("SkillRepeatKnown")
                            end
                            alive = not quest:IsActiveThreadTerminating()
                            bVar3 = not alive
                            if bVar3 then
                                quest:DeregisterTimer(xStack_190)
                                return
                            end
                        end
                    until not (c_stk_165 == 0)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    if bVar3 then
                        -- LAB_00d42f0b: (native jump target)
                        quest:DeregisterTimer(xStack_190)
                        return
                    end
                    quest:DeregisterTimer(xStack_190)
                    alive = quest:NewScriptFrame(me)
                    alive = not quest:IsActiveThreadTerminating()
                    bVar3 = not alive
                    fVar12 = xStack_158
                until false
            end
        end
    end
    ::LAB_00d41f6f::
end

function Init(quest, me)
    local this_00 = me:GetDataString()
    local iVar1 = tonumber(this_00)
    __native_entity_state:SetStateInt("DummyNumber", iVar1)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

