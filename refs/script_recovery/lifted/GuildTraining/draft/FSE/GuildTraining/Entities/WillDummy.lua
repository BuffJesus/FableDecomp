-- Generated native draft: WillDummy. Review coverage report before use.
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
    local cVar2, fVar11, iVar3, pCStack_14c, pCVar1, pCVar12, pCVar13, piVar4, ppVar14, ppVar5, r1, r2, r3, r4, uVar10, uVar6, uVar9
    local alive = true
    fVar11 = me:GetAngleXY()
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    if alive then
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if alive then
            quest:SetTimer(0, nil --[[missing]])
            cVar2 = quest:GetMasterGameState("WillTrainingStarted")
            while cVar2 ~= '\x01' do
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                cVar2 = quest:GetMasterGameState("WillTrainingStarted")
            end
            alive = not quest:IsActiveThreadTerminating()
            if alive then
                alive = not quest:IsActiveThreadTerminating()
                uVar10 = 0
                cVar2 = extraout_AL_03
                -- TODO(native): joined_r0x00d434fc:
                if cVar2 == 0 then
                    repeat
                        uVar9 = uVar10 | 1
                        ppVar14 = 0xb
                        cVar2 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
                        if not cVar2 then
                            uVar9 = uVar10 | 3
                            cVar2 = me:MsgIsHitByHero()
                            -- TODO(native): uStack_ec = (undefined4 *)CONCAT13(1,(undefined3)uStack_ec);
                            uVar10 = uVar9
                            if cVar2 then return end  -- TODO(native): goto LAB_00d43568
                        else
                            -- LAB_00d43568: (native jump target)
                            -- TODO(native): uStack_ec = (undefined4 *)((uint)uStack_ec & 0xffffff);
                            uVar10 = uVar9
                        end
                        if (uVar10 & 2) ~= 0 then
                            uVar10 = uVar10 & 0xfffffffd
                        end
                        if (uVar10 & 1) ~= 0 then
                            uVar10 = uVar10 & 0xfffffffe
                        end
                        if uStack_ec._3_1_ == 0 then goto LAB_00d435c1 end
                        alive = quest:NewScriptFrame(me)
                        alive = not quest:IsActiveThreadTerminating()
                        if not alive then
                            return
                        end
                    until false
                end
            end
        end
    end
    do return end
    ::LAB_00d435c1::
    alive = not quest:IsActiveThreadTerminating()
    if not alive then
        return
    end
    pCVar13 = "SCRIPT_NAME_HERO"
    ppVar14 = 0xb
    cVar2 = me:MsgIsHitByHeroSpecialAbility(nil --[[missing]])
    if not cVar2 then
        cVar2 = me:MsgIsHitByHero()
        if cVar2 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                return
            end
            quest:EntityPlayObjectAnimation(nil --[[missing]], "GET_HIT")
            iVar3 = quest:GetTimer(ppVar14)
            if iVar3 < 1 then
                alive = not quest:IsActiveThreadTerminating()
                if not alive then
                    return
                end
                piVar4 = quest:GetThingWithScriptName("WillApprentice")
                cVar2 = (piVar4 ~= nil and piVar4:IsAlive())
                alive = not quest:IsActiveThreadTerminating()
                if not cVar2 then
                    if not alive then
                        return
                    end
                    -- TODO(native): pCStack_170 = aCStack_98;
                    ppVar5 = quest:GetThingWithScriptName("TheRealGuildmaster")
                    ppVar14 = quest:AddNewConversation(ppVar5, nil --[[missing]], nil --[[missing]])
                    uVar6 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar14, uVar6)
                    iVar3 = *piVar4
                    uVar6 = quest:GetHero()
                    uVar6 = quest:GetThingWithScriptName("TEXT_QST_028_GUILDMASTER_WILL_TROUBLE")
                    quest:AddLineToConversation(ppVar14, "TheRealGuildmaster", uVar6, nil --[[missing]], false)
                else
                    if not alive then
                        return
                    end
                    -- TODO(native): pCStack_170 = aCStack_c8;
                    ppVar5 = quest:GetThingWithScriptName("WillApprentice")
                    ppVar14 = quest:AddNewConversation(ppVar5, nil --[[missing]], nil --[[missing]])
                    uVar6 = quest:GetHero()
                    quest:AddPersonToConversation(ppVar14, uVar6)
                    iVar3 = *piVar4
                    uVar6 = quest:GetHero()
                    uVar6 = quest:GetThingWithScriptName("TEXT_QST_028_APPRENTICE_WILL_TROUBLE")
                    quest:AddLineToConversation(ppVar14, "WillApprentice", uVar6, nil --[[missing]], false)
                end
                quest:SetTimer(quest:GetStateInt("WillHelpTimer"), 7)
            end
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        quest:EntityPlayObjectAnimation(nil --[[missing]], "GET_HIT_SPIN")
        -- TODO(native): fStack_118 = unaff_EBP + (float)_DAT_01238010;
        -- TODO(native): fStack_c4 = fStack_118;
        quest:EntitySetFacingAngle(me, 1)
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        -- TODO(native): fStack_124 = (float)pCVar13 + (float)_DAT_012316f0;
        quest:EntitySetFacingAngle(me, 1)
        quest:SetMasterGameState("WillScore", quest:GetMasterGameState("WillScore") + 1)
        iVar3 = quest:GetTimer(nil --[[missing]])
        if iVar3 < 1 then
            alive = not quest:IsActiveThreadTerminating()
            if not alive then
                return
            end
            piVar4 = quest:GetThingWithScriptName("WillApprentice")
            cVar2 = (piVar4 ~= nil and piVar4:IsAlive())
            alive = not quest:IsActiveThreadTerminating()
            if not cVar2 then
                if not alive then
                    return
                end
                -- TODO(native): pCStack_144 = (CCreatureAction_TrollWhackGroundBase *)aCStack_90;
                ppVar5 = quest:GetThingWithScriptName("TheRealGuildmaster")
                ppVar14 = quest:AddNewConversation(ppVar5, nil --[[missing]], nil --[[missing]])
                pCStack_14c = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], pCStack_14c)
                iVar3 = *piVar4
                r1 = quest:GetHero()
                r2 = quest:GetThingWithScriptName("TEXT_QST_028_GUILDMASTER_WILL_GOOD_HIT")
                quest:AddLineToConversation(nil --[[missing]], "TheRealGuildmaster", r2, r1)
            else
                if not alive then
                    return
                end
                -- TODO(native): pCStack_144 = aCStack_a8;
                ppVar5 = quest:GetThingWithScriptName("WillApprentice")
                ppVar14 = quest:AddNewConversation(ppVar5, nil --[[missing]], nil --[[missing]])
                pCStack_14c = quest:GetHero()
                quest:AddPersonToConversation(nil --[[missing]], pCStack_14c)
                iVar3 = *piVar4
                r3 = quest:GetHero()
                r4 = quest:GetThingWithScriptName("TEXT_QST_028_APPRENTICE_WILL_GOOD_HIT")
                quest:AddLineToConversation(nil --[[missing]], "WillApprentice", r4, r3)
            end
            quest:SetTimer(nil --[[missing]], nil --[[missing]])
        end
        quest:EntitySetTargetable(nil --[[missing]], nil --[[missing]])
        quest:Pause(nil --[[missing]])
        -- TODO(native): puStack_140 = uStack_ec;
        quest:EntitySetFacingAngle(me, nil --[[missing]])
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        if not alive then
            return
        end
        -- TODO(native): pCStack_14c = pCStack_128;
        quest:EntitySetFacingAngle(nil --[[missing]], nil --[[missing]])
        quest:EntitySetTargetable(nil --[[missing]], nil --[[missing]])
    end
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    cVar2 = extraout_AL_16
    -- TODO(native): goto joined_r0x00d434fc;
end

function Init(quest, me)
end

function OnPersist(quest, context)
end

function OnPredicateFail(quest, me)
end

