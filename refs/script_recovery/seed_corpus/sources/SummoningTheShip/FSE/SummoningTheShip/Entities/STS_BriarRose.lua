-- STS_BriarRose.lua
-- Entity script for Briar Rose during Summoning The Ship
-- Functional parity port of NScript::CQ_SummoningTheShipScript::CSTS_BriarRose

local HERO_ABILITY_HEAL_LIFE_SPELL = 14
local AI_STATE_FIGHTING = 1
local AI_STATE_FOLLOWING = 2

local function MakeBriarRoseComment(quest, comment)
    local convoID = quest:GetStateInt("ConversationIndex") or -1
    if convoID ~= -1 and quest:IsConversationActive(convoID) then
        return false
    end

    local commTimer = quest:GetStateInt("CommentaryTimer")
    if commTimer and commTimer >= 0 then
        quest:SetTimer(commTimer, 15)
    end

    local briarRose = quest:GetThingWithScriptName("STS_BriarRose")
    if not briarRose or briarRose:IsDead() or not briarRose:IsAlive() then
        return false
    end

    local hero = quest:GetHero()
    if not hero then
        return false
    end

    local newConvoID = quest:StartAmbientConversation(briarRose, hero, false, false)
    if not newConvoID or newConvoID == -1 then
        return false
    end

    quest:SetStateInt("ConversationIndex", newConvoID)
    local lineKey = "TEXT_QST_B02_BRIARROSE_" .. comment
    quest:AddLineToConversation(newConvoID, lineKey, briarRose, hero, false)
    return true
end

function Init(quest, me)
    quest:Log("STS_BriarRose: Init() started.")

    me:MakeBehavioral()

    local hero = quest:GetHero()
    quest:EntitySetAsRespondingToFollowAndWaitExpressions(me, false)
    quest:SetThingPersistent(me, true)
    quest:EntitySetAsAllowedToFollowHero(me, true)
    quest:SetThingHasInformation(me, false, true, false)
    quest:EntitySheatheWeapons(me, false)

    if hero then
        quest:EntitySetThingAsAllyOfThing(me, hero)
    end

    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)

    quest:Log("STS_BriarRose: Init() completed.")
end

function Main(quest, me)
    quest:Log("STS_BriarRose: Main() started.")

    local hero = quest:GetHero()

    -- Initial yield frame
    if not quest:NewScriptFrame(me) then return end

    -- Wait until SummonerAttacksStarted
    while not quest:GetStateBool("SummonerAttacksStarted") do
        if not quest:NewScriptFrame(me) then return end
    end

    quest:Log("STS_BriarRose: Attacks started. Entering combat behavior...")

    local attackedByEnemyTimer = quest:RegisterTimer()
    local previousAIState = 0
    local summonerCountComment = 0
    local activeQuestName = quest:GetActiveQuestName()

    quest:SetQuestCardObjective(activeQuestName, "TEXT_QUEST_SUMMONING_SHIP_OBJECTIVE_03", "HookCoast", "HookCoast")

    quest:ClearThingBestEnemyTarget(me)
    if hero then
        quest:EntityFollowThing(me, hero, 1.0, true)
    end

    local briarMode = 0
    local enemy = nil

    -- Main battle loop
    while true do
        local curWave = quest:GetStateInt("CurrentAttackWave") or 1

        -- 1. Dynamic target selection
        if curWave == 1 then
            if briarMode ~= 1 then
                enemy = quest:GetNearestWithScriptName(me, "SummonerAttacker")
                if enemy and enemy:IsAlive() then
                    quest:GiveThingBestEnemyTarget(me, enemy)
                end
                briarMode = 1
            end
        else
            -- Wave 2+: Target whichever enemy (Summoner or Minion) is closest
            if briarMode == 1 and (not enemy or enemy:IsDead() or not enemy:IsAlive()) then
                local nearestMinion = quest:GetNearestWithScriptName(me, "SummonerMinion")
                local nearestSummoner = quest:GetNearestWithScriptName(me, "SummonerAttacker")

                if nearestMinion and nearestSummoner then
                    local dMinion = quest:GetDistanceBetweenThings(me, nearestMinion)
                    local dSummoner = quest:GetDistanceBetweenThings(me, nearestSummoner)
                    if dSummoner >= dMinion then
                        enemy = nearestMinion
                    else
                        enemy = nearestSummoner
                    end
                elseif nearestMinion then
                    enemy = nearestMinion
                elseif nearestSummoner then
                    enemy = nearestSummoner
                else
                    enemy = nil
                end

                if enemy and enemy:IsAlive() then
                    quest:GiveThingBestEnemyTarget(me, enemy)
                end
            end
        end

        -- 2. Commentary: Summoners remaining count
        local summonersAlive = quest:GetStateInt("SummonersAlive") or 0
        if summonerCountComment ~= summonersAlive then
            local commentKey = nil
            if summonersAlive == 1 then
                commentKey = "SUMMONER_ONE_LEFT"
            elseif summonersAlive == 2 then
                commentKey = "SUMMONER_TWO_LEFT"
            elseif summonersAlive == 3 then
                commentKey = "SUMMONER_THREE_LEFT"
            end

            if commentKey and summonersAlive > 0 and summonersAlive <= 3 then
                if MakeBriarRoseComment(quest, commentKey) then
                    summonerCountComment = summonersAlive
                end
            end
        end

        -- 3. Commentary: AI state transitions (Following vs Attacking)
        local currentStateGroup = me:GetCurrentStateGroupType()
        if previousAIState ~= currentStateGroup then
            if currentStateGroup == AI_STATE_FOLLOWING then
                MakeBriarRoseComment(quest, "FOLLOWING")
            elseif currentStateGroup == AI_STATE_FIGHTING then
                MakeBriarRoseComment(quest, "ATTACKING")
            end
            previousAIState = currentStateGroup
        end

        -- 4. Commentary: Attacked by enemies (30s cooldown)
        if quest:GetTimer(attackedByEnemyTimer) == 0 then
            local hitBySummoner = me:MsgIsHitBy("SummonerAttacker")
                or (me:MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker")
                    and not me:MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))

            if hitBySummoner then
                if MakeBriarRoseComment(quest, "SUMMONER_ATTACKED") then
                    quest:SetTimer(attackedByEnemyTimer, 30)
                end
            else
                local hitByMinion = me:MsgIsHitBy("SummonerMinion")
                    or (me:MsgIsHitByAnySpecialAbilityFrom("SummonerMinion")
                        and not me:MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))

                if hitByMinion then
                    if MakeBriarRoseComment(quest, "MINION_ATTACKED") then
                        quest:SetTimer(attackedByEnemyTimer, 30)
                    end
                end
            end
        end

        -- 5. Commentary: Attacked by Hero
        local hitByHero = me:MsgIsHitBy("SCRIPT_NAME_HERO")
            or (me:MsgIsHitByAnySpecialAbilityFrom("SCRIPT_NAME_HERO")
                and not me:MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))

        if hitByHero then
            MakeBriarRoseComment(quest, "ATTACKED")
        end

        if not quest:NewScriptFrame(me) then break end
        if me:IsDead() then break end
    end

    quest:DeregisterTimer(attackedByEnemyTimer)
    quest:Log("STS_BriarRose: Main() finished.")
end
