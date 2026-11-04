-- Generated native draft: ArenaEnemy. Review coverage report before use.
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
    local bVar3, bVar4, bVar5, bVar6, bVar7, c_stk_55, ctr_50, iVar11, iVar9, p0, pCVar10, pCVar8, pcVar13, r1, r2
    local alive = true
    iVar11 = 0
    bVar4 = false
    bVar6 = false
    bVar3 = false
    alive = quest:NewScriptFrame(me)
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    __native_entity_state:SetStateInt("MyCreatureType", 0)
    ctr_50 = 0
    if 0 < quest:GetStateInt(("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_NumWaveCreatures")) then
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            pCVar8 = me:GetDefName()
            iVar9 = ((pCVar8 == quest:GetStateString(("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. ctr_50 .. "_CreatureType"))) and 0 or 1)
            c_stk_55 = not (iVar9 ~= 0)
            if c_stk_55 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                __native_entity_state:SetStateInt("MyCreatureType", ctr_50)
            end
            ctr_50 = ctr_50 + 1
        until not (ctr_50 < quest:GetStateInt(("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_NumWaveCreatures")))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    if bVar5 then
        return
    end
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    __native_entity_state:SetStateBool("BigCreature", false)
    pCVar8 = me:GetDefName()
    iVar11 = ((pCVar8 == "CREATURE_EARTH_TROLL_START_STANDING") and 0 or 1)
    c_stk_55 = not (iVar11 ~= 0)
    if not c_stk_55 then
        pCVar8 = me:GetDefName()
        iVar11 = ((pCVar8 == "CREATURE_ROCK_TROLL_START_STANDING") and 0 or 1)
        c_stk_55 = not (iVar11 ~= 0)
        if not c_stk_55 then
            pCVar8 = me:GetDefName()
            iVar11 = ((pCVar8 == "CREATURE_SCORPION_KING") and 0 or 1)
            c_stk_55 = not (iVar11 ~= 0)
            if not c_stk_55 then goto LAB_00f1acb5 end
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            pcVar13 = "HUD_QUEST_ICON_SCORPION_KING"
        else
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            pcVar13 = "HUD_QUEST_ICON_ROCK_TROLL"
        end
    else
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
        if bVar5 then
            return
        end
        pcVar13 = "HUD_QUEST_ICON_EARTH_TROLL"
    end
    iVar11 = quest:AddQuestInfoBarHealth(me, {R = 255, G = 255, B = 0, A = 255}, pcVar13, 1.0)
    __native_entity_state:SetStateInt("HealthBarIndex", iVar11)
    __native_entity_state:SetStateBool("BigCreature", true)
    ::LAB_00f1acb5::
    alive = not quest:IsActiveThreadTerminating()
    bVar5 = not alive
    repeat
        if bVar5 then
            return
        end
        bVar5 = me:MsgIsHitByHeroWithFlourish()
        if bVar5 then
            alive = not quest:IsActiveThreadTerminating()
            bVar5 = not alive
            if bVar5 then
                return
            end
            iVar11 = quest:GetTimer(quest:GetStateInt("GlobalCrowdTimer"))
            if iVar11 < 1 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                pCVar8 = quest:GetStateString(("CrowdLoopTags_" .. quest:GetStateInt("NewCrowdBaseLevel") .. "_" .. 3))
                pCVar10 = quest:GetHero()
                pCVar10 = quest:GetNearestWithScriptName(pCVar10, "ArenaSpawn")
                r1 = quest:PlayCriteriaSoundOnThing(pCVar10, pCVar8)
                quest:SetTimer(quest:GetStateInt("GlobalCrowdTimer"), 10)
                quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 5)
            end
        else
            bVar5 = me:MsgIsHitByHeroWithDecapitate()
            if bVar5 then
                alive = not quest:IsActiveThreadTerminating()
                bVar5 = not alive
                if bVar5 then
                    return
                end
                iVar11 = quest:GetTimer(quest:GetStateInt("GlobalCrowdTimer"))
                if iVar11 < 9 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    pCVar8 = quest:GetStateString(("CrowdLoopTags_" .. quest:GetStateInt("NewCrowdBaseLevel") .. "_" .. 3))
                    pCVar10 = quest:GetHero()
                    pCVar10 = quest:GetNearestWithScriptName(pCVar10, "ArenaSpawn")
                    r2 = quest:PlayCriteriaSoundOnThing(pCVar10, pCVar8)
                    quest:SetTimer(quest:GetStateInt("GlobalCrowdTimer"), 10)
                    quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 7)
                end
            else
                bVar5 = me:MsgIsHitByHero()
                if bVar5 then
                    goto LAB_00f1af52
                else
                    bVar6 = me:MsgIsHitByAnySpecialAbilityFromHero()
                    if bVar6 then
                        bVar6 = true
                        bVar3 = true
                        bVar5 = me:MsgIsHitByHeroSpecialAbility(0xe)
                        if not bVar5 then goto LAB_00f1af52 end
                    end
                    bVar6 = true
                    bVar5 = false
                end
                goto FLOW_past_lab_00f1af52
                ::LAB_00f1af52::
                bVar5 = true
                ::FLOW_past_lab_00f1af52::
                if bVar3 then
                    bVar3 = false
                end
                if bVar6 then
                    bVar6 = false
                end
                if bVar5 then
                    alive = not quest:IsActiveThreadTerminating()
                    bVar5 = not alive
                    if bVar5 then
                        return
                    end
                    quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + 1)
                    pCVar10 = quest:GetHero()
                    quest:GiveThingBestEnemyTarget(me, pCVar10)
                else
                    bVar5 = me:MsgIsHitBy("WhisperAlly")
                    if bVar5 then
                        goto LAB_00f1b041
                    else
                        bVar4 = true
                        bVar7 = me:MsgIsHitByAnySpecialAbilityFrom("WhisperAlly")
                        bVar5 = false
                        if bVar7 then goto LAB_00f1b041 end
                    end
                    goto FLOW_past_lab_00f1b041
                    ::LAB_00f1b041::
                    bVar5 = true
                    ::FLOW_past_lab_00f1b041::
                    if bVar4 then
                        bVar4 = false
                    end
                    if bVar5 then
                        alive = not quest:IsActiveThreadTerminating()
                        bVar5 = not alive
                        if bVar5 then
                            return
                        end
                        pCVar10 = quest:GetThingWithScriptName("WhisperAlly")
                        quest:GiveThingBestEnemyTarget(me, pCVar10)
                    end
                end
            end
        end
        alive = quest:NewScriptFrame(me)
        alive = not quest:IsActiveThreadTerminating()
        bVar5 = not alive
    until false
end

function Init(quest, me)
end

function OnPersist(quest, me, context)
end

function OnPredicateFail(quest, me)
    quest:SetStateInt(("TotalCreatures_" .. __native_entity_state:GetStateInt("MyCreatureType")), quest:GetStateInt(("TotalCreatures_" .. __native_entity_state:GetStateInt("MyCreatureType"))) - 1)
    local cVar3 = me:MsgIsKilledBy("SCRIPT_NAME_HERO")
    if cVar3 then
        quest:SetStateInt("NewCrowdPoints", quest:GetStateInt("NewCrowdPoints") + quest:GetStateInt(("Rounds_" .. quest:GetStateInt("ArenaRound") .. "_Waves_" .. quest:GetStateInt("ArenaRoundWave") .. "_Creatures_" .. __native_entity_state:GetStateInt("MyCreatureType") .. "_DeathScore")))
    end
    if __native_entity_state:GetStateBool("BigCreature") then
        quest:RemoveQuestInfoElement(__native_entity_state:GetStateInt("HealthBarIndex"))
    end
end

