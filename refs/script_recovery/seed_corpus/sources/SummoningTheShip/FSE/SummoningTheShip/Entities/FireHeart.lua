-- FireHeart.lua
-- Entity script for the Fire Heart artifact placed in the lighthouse
-- Functional parity port of NScript::CQ_SummoningTheShipScript::CFireHeart

local HERO_ABILITY_HEAL_LIFE_SPELL = 14

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
    quest:Log("FireHeart: Init() started.")

    quest:EntitySetAsKillable(me, false)
    quest:EntitySetAsDamageable(me, false)

    quest:Log("FireHeart: Init() completed.")
end

function Main(quest, me)
    quest:Log("FireHeart: Main() started.")

    quest:EntitySetAlpha(me, 0.0)

    -- Wait until Hero activates lighthouse
    while not quest:GetStateBool("LighthouseStarted") do
        if not quest:NewScriptFrame(me) then return end
    end

    quest:MiniMapRemoveMarker(me)

    -- Wait until Summoner attacks start
    while not quest:GetStateBool("SummonerAttacksStarted") do
        if not quest:NewScriptFrame(me) then return end
    end

    quest:Log("FireHeart: Attacks initiated. Becoming active and damageable...")

    quest:EntitySetAsDamageable(me, true)
    quest:DisplayQuestInfo(true)
    quest:EntitySetAlpha(me, 1.0)

    local commentTimer = quest:RegisterTimer()
    quest:AddQuestInfoBarHealth(me, { red = 0, green = 255, blue = 0 }, "HUD_ICON_FIRE_HEART", 1.0)

    local heartBeatTimer = quest:RegisterTimer()
    quest:SetTimer(heartBeatTimer, 3)

    -- Health monitoring loop
    while true do
        local hp = quest:GetHealth(me)
        if hp <= 1.0 then
            quest:Log("FireHeart: Health dropped to <= 1.0! Mission failed.")
            quest:SetStateBool("MissionFailed", true)
            break
        end

        if not quest:NewScriptFrame(me) then break end

        -- Heartbeat sound intervals based on health remaining
        if quest:GetTimer(heartBeatTimer) <= 0 then
            hp = quest:GetHealth(me)
            if hp <= 750.0 then
                quest:Play2DSound("SND_MM_FIREHEART_BEAT_FAST")
                quest:SetTimer(heartBeatTimer, 1)
            elseif hp > 1500.0 then
                quest:Play2DSound("SND_MM_FIREHEART_BEAT_SLOW")
                quest:SetTimer(heartBeatTimer, 3)
            else
                quest:Play2DSound("SND_MM_FIREHEART_BEAT_MEDIUM")
                quest:SetTimer(heartBeatTimer, 2)
            end
        end

        -- Check if attacked by Summoners
        local hitBySummoner = me:MsgIsHitBy("SummonerAttacker")
            or (me:MsgIsHitByAnySpecialAbilityFrom("SummonerAttacker")
                and not me:MsgIsHitBySpecialAbilityFrom(HERO_ABILITY_HEAL_LIFE_SPELL, "SCRIPT_NAME_HERO"))

        if hitBySummoner then
            if quest:GetTimer(commentTimer) == 0 then
                if MakeBriarRoseComment(quest, "FIREHEART") then
                    quest:SetTimer(commentTimer, 20)
                end
            end
        end

        if quest:GetHealth(me) <= 1.0 then
            quest:Log("FireHeart: Health depleted during attack. Mission failed.")
            quest:SetStateBool("MissionFailed", true)
            break
        end
    end

    quest:DeregisterTimer(heartBeatTimer)
    quest:DeregisterTimer(commentTimer)
    quest:Log("FireHeart: Main() finished.")
end
