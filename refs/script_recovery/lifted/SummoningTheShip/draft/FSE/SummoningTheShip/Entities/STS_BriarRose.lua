-- Generated native draft: STS_BriarRose. Review coverage report before use.
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
    local bVar2, bVar3, bVar4, bVar5, bVar6, bVar7, cVar1, fVar11, f_stk_40, iVar9, i_stk_2c, i_stk_38, i_stk_90, native_arg_sequence_1, native_arg_sequence_2, p0, pCVar8, pOther, pQuestName, r1, r2, xStack_8c, x_stk_7c
    local alive = true
    bVar4 = false
    bVar7 = false
    bVar3 = false
    bVar6 = false
    bVar2 = false
    f_stk_40 = 0.0
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if not bVar5 then
        cVar1 = quest:GetStateBool("SummonerAttacksStarted")
        while not cVar1 do
            alive = quest:NewScriptFrame(me)
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            cVar1 = quest:GetStateBool("SummonerAttacksStarted")
        end
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if not bVar5 then
            i_stk_2c = 0
            i_stk_90 = quest:RegisterTimer()
            i_stk_38 = 0
            __native_entity_state:SetStateInt("BrainState", 2)
            pQuestName = quest:GetActiveQuestName()
            quest:SetQuestCardObjective(pQuestName, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_03", "HookCoast", "HookCoast")
            helper_DF2090(quest, me, 1)
            x_stk_7c = 0
            xStack_8c = nil
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            while not bVar5 do
                if quest:GetStateInt("CurrentAttackWave") == 1 then
                    if x_stk_7c ~= 1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then break end
                        pCVar8 = quest:GetNearestWithScriptName(me, "SummonerAttacker")
                        xStack_8c = pCVar8
                        iVar9 = (xStack_8c ~= nil and xStack_8c:IsAlive())
                        if iVar9 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:DeregisterTimer(i_stk_90)
                                return
                            end
                            quest:GiveThingBestEnemyTarget(me, xStack_8c)
                        end
                        x_stk_7c = 1
                    end
                    -- TODO(native): } else {
                    native_arg_sequence_1 = false
                    if x_stk_7c == 1 then
                        native_arg_sequence_1 = true
                    else
                        native_arg_sequence_1 = false
                    end
                    if native_arg_sequence_1 then
                        bVar5 = (xStack_8c ~= nil and xStack_8c:IsAlive())
                        if not bVar5 then
                            native_arg_sequence_1 = true
                        else
                            native_arg_sequence_1 = false
                        end
                    end
                    if native_arg_sequence_1 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:DeregisterTimer(i_stk_90)
                            return
                        end
                        r1 = quest:GetNearestWithScriptName(me, "SummonerAttacker")
                        r2 = quest:GetNearestWithScriptName(me, "SummonerMinion")
                        f_stk_40 = (quest:GetDistanceBetweenThings(me, r1) ^ 2)
                        fVar11 = (quest:GetDistanceBetweenThings(me, r2) ^ 2)
                        if f_stk_40 <= fVar11 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                pCVar8 = r1
                                goto LAB_00df1b40
                            end
                        else
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if not bVar5 then
                                pCVar8 = r2
                                goto LAB_00df1b40
                            end
                        end
                        goto FLOW_past_lab_00df1b40
                        ::LAB_00df1b40::
                        xStack_8c = pCVar8
                        iVar9 = (xStack_8c ~= nil and xStack_8c:IsAlive())
                        if iVar9 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then goto LAB_00df204e end
                            quest:GiveThingBestEnemyTarget(me, xStack_8c)
                        end
                        goto LAB_00df1b8b
                        ::FLOW_past_lab_00df1b40::
                        ::LAB_00df204e::
                        quest:DeregisterTimer(i_stk_90)
                        return
                    end
                end
                ::LAB_00df1b8b::
                if i_stk_38 ~= quest:GetStateInt("SummonersAlive") then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    iVar9 = quest:GetStateInt("SummonersAlive")
                    if iVar9 == 1 then
                        pOther = "SUMMONER_ONE_LEFT"
                        goto LAB_00df1bd5
                    else
                        if iVar9 == 2 then
                            pOther = "SUMMONER_TWO_LEFT"
                            goto LAB_00df1bd5
                        end
                        if iVar9 == 3 then
                            pOther = "SUMMONER_THREE_LEFT"
                            goto LAB_00df1bd5
                        end
                    end
                    goto FLOW_past_lab_00df1bd5
                    ::LAB_00df1bd5::
                    ::FLOW_past_lab_00df1bd5::
                    -- TODO(native): iVar9 = *(*(this + 0x14) + 0x48)
                    iVar9 = nil --[[unresolved native value]]
                    native_arg_sequence_2 = false
                    if 0 < iVar9 then
                        native_arg_sequence_2 = true
                    else
                        native_arg_sequence_2 = false
                    end
                    if native_arg_sequence_2 then
                        if iVar9 < 4 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                    end
                    if native_arg_sequence_2 then
                        -- TODO(native): bVar5 = require("SummoningTheShip.native_quest_helpers").MakeBriarRoseComment(quest, me, &pOther)
                        bVar5 = nil --[[unresolved native value]]
                        if bVar5 then
                            native_arg_sequence_2 = true
                        else
                            native_arg_sequence_2 = false
                        end
                    end
                    if native_arg_sequence_2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:DeregisterTimer(i_stk_90)
                            return
                        end
                        i_stk_38 = quest:GetStateInt("SummonersAlive")
                    end
                end
                iVar9 = me:GetCurrentStateGroupType()
                if i_stk_2c ~= iVar9 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then break end
                    if iVar9 == 2 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:DeregisterTimer(i_stk_90)
                            return
                        end
                        require("SummoningTheShip.native_quest_helpers").MakeBriarRoseComment(quest, me, "FOLLOWING")
                    else
                        if iVar9 ~= 1 then goto LAB_00df1cb0 end
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:DeregisterTimer(i_stk_90)
                            return
                        end
                        require("SummoningTheShip.native_quest_helpers").MakeBriarRoseComment(quest, me, "ATTACKING")
                    end
                end
                ::LAB_00df1cb0::
                i_stk_2c = iVar9
                iVar9 = quest:GetTimer(i_stk_90)
                if iVar9 == 0 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        quest:DeregisterTimer(i_stk_90)
                        return
                    end
                    bVar5 = me:MsgIsHitBy("SummonerAttacker")
                    if bVar5 then
                        goto LAB_00df1d56
                    else
                        bVar6 = me:MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker")
                        if bVar6 then
                            bVar6 = true
                            bVar2 = true
                            bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar5 then goto LAB_00df1d56 end
                        end
                        bVar6 = true
                        bVar5 = false
                    end
                    goto FLOW_past_lab_00df1d56
                    ::LAB_00df1d56::
                    bVar5 = true
                    ::FLOW_past_lab_00df1d56::
                    if bVar2 then
                        bVar2 = false
                    end
                    if bVar6 then
                        bVar6 = false
                    end
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:DeregisterTimer(i_stk_90)
                            return
                        end
                        bVar5 = require("SummoningTheShip.native_quest_helpers").MakeBriarRoseComment(quest, me, "SUMMONER_ATTACKED")
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:DeregisterTimer(i_stk_90)
                                return
                            end
                            quest:SetTimer(i_stk_90, 0x1e)
                        end
                    end
                    bVar5 = me:MsgIsHitBy("SummonerMinion")
                    if bVar5 then
                        goto LAB_00df1e73
                    else
                        bVar7 = me:MsgIsHitByAnySpecialAbilityFrom("SummonerMinion")
                        if bVar7 then
                            bVar7 = true
                            bVar3 = true
                            bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                            if not bVar5 then goto LAB_00df1e73 end
                        end
                        bVar7 = true
                        bVar5 = false
                    end
                    goto FLOW_past_lab_00df1e73
                    ::LAB_00df1e73::
                    bVar5 = true
                    ::FLOW_past_lab_00df1e73::
                    if bVar3 then
                        bVar3 = false
                    end
                    if bVar7 then
                        bVar7 = false
                    end
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            quest:DeregisterTimer(i_stk_90)
                            return
                        end
                        bVar5 = require("SummoningTheShip.native_quest_helpers").MakeBriarRoseComment(quest, me, "MINION_ATTACKED")
                        if bVar5 then
                            alive = not quest:IsActiveThreadTerminating()
                            bVar5 = not alive
                            if bVar5 then
                                quest:DeregisterTimer(i_stk_90)
                                return
                            end
                            quest:SetTimer(i_stk_90, 0x1e)
                        end
                    end
                end
                bVar5 = me:MsgIsHitByHero()
                if bVar5 then
                    goto LAB_00df1f96
                else
                    bVar5 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar5 then
                        bVar4 = true
                        bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar5 then goto LAB_00df1f96 end
                    end
                    bVar5 = false
                end
                goto FLOW_past_lab_00df1f96
                ::LAB_00df1f96::
                bVar5 = true
                ::FLOW_past_lab_00df1f96::
                if bVar4 then
                    bVar4 = false
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        -- LAB_00df206e: (native jump target)
                        quest:DeregisterTimer(i_stk_90)
                        return
                    end
                    require("SummoningTheShip.native_quest_helpers").MakeBriarRoseComment(quest, me, pQuestName)
                end
                alive = quest:NewScriptFrame(me)
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
            end
            quest:DeregisterTimer(i_stk_90)
            return
        end
    end
end

function Init(quest, me)
    __native_entity_state:SetStateInt("BrainState", 1)
    quest:EntitySetAsRespondingToFollowAndWaitExpressions(me, false)
    quest:SetThingPersistent(me, true)
    quest:EntitySetAsAllowedToFollowHero(me, true)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySheatheWeapons(me, false)
    local pThing2 = quest:GetHero()
    quest:EntitySetThingAsAllyOfThing(me, pThing2)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
end

function helper_DF2090(quest, me, native_arg_param_1)
    local pTarget
    if native_arg_param_1 ~= 0 then
        quest:ClearThingBestEnemyTarget(me)
        pTarget = quest:GetHero()
        quest:EntityFollowThing(me, pTarget, 1.0, true)
        return
    end
    quest:EntityStopFollowing(me)
end

