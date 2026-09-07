-- SingingStone.lua
-- Entity script for SingingStone in Witchwood Stones (Witchwood2)
-- Functional parity port of NScript::CV_SingingStonesScript::CSingingStone

local STONE_TEXT = {
    [0] = "TEXT_QST_060_NAME_A",
    [1] = "TEXT_QST_060_NAME_B",
    [2] = "TEXT_QST_060_NAME_C",
    [3] = "TEXT_QST_060_NAME_D",
}

local myStoneNumber = 0

function Init(quest, me)
    local dataStr = me:GetDataString()
    myStoneNumber = tonumber(dataStr) or 0

    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")

    local textTag = STONE_TEXT[myStoneNumber]
    if textTag then
        me:SetReadableText(textTag)
    end
end

function Main(quest, me)
    local dataStr = me:GetDataString()
    myStoneNumber = tonumber(dataStr) or 0

    local hero = quest:GetHero()
    if not hero then
        quest:Log("!!! ERROR: SingingStone Main - could not get Hero. Aborting.")
        return
    end

    local speakMarker = quest:GetNearestWithScriptName(me, "SpeakMarker")
    local myEffect = nil

    -- Reconnection check: if game loaded while a tune was partially entered
    local currentIdx = quest:GetStateInt("CurrentPlayListIndex") or 0
    if currentIdx > 0 then
        local alreadyInList = false
        for i = 0, currentIdx - 1 do
            if quest:GetStateInt("PersonalPlayList_" .. i) == myStoneNumber then
                alreadyInList = true
                break
            end
        end

        if alreadyInList then
            while true do
                if quest:IsDistanceBetweenThingsUnder(me, hero, 35.0) and quest:IsCameraPosOnScreen(me:GetPos()) then
                    break
                end
                if not quest:NewScriptFrame(me) then return end
            end

            local pos = me:GetPos()
            myEffect = quest:CreateEffectAtPos("MARKTELEPORTER", { x = pos.x, y = pos.y, z = pos.z + 1.0 })
        end
    end

    -- State machine loop
    while true do
        -- Check if effects should be cleared (reset or region left, unless puzzle already completed)
        local shouldClear = quest:GetStateBool("ClearEffects")
            or (quest:GetStateInt("CurrentPlayListIndex") == 0 and not quest:GetStateBool("PuzzleWon"))

        if shouldClear and myEffect then
            quest:RemoveThing(myEffect)
            myEffect = nil
        end

        -- Check if struck by Hero
        local wasHit = me:MsgIsHitByHero()
            or (me:MsgIsHitByAnySpecialAbilityFromHero() and not me:MsgIsHitByHealLifeFromHero())

        if wasHit then
            local activeIdx = quest:GetStateInt("CurrentPlayListIndex") or 0
            local alreadyHit = false

            for i = 0, activeIdx - 1 do
                if quest:GetStateInt("PersonalPlayList_" .. i) == myStoneNumber then
                    alreadyHit = true
                    break
                end
            end

            if not alreadyHit and activeIdx < 4 and not quest:GetStateBool("PuzzleWon") then
                -- Play stone tone via SpeakMarker
                if speakMarker then
                    local convoID = quest:StartAmbientConversation(speakMarker, hero, true, true)
                    if convoID and convoID ~= -1 then
                        quest:AddLineToConversation(convoID, STONE_TEXT[myStoneNumber], speakMarker, hero, false)
                    end
                end

                -- Append stone to playlist in quest state
                quest:SetStateInt("PersonalPlayList_" .. activeIdx, myStoneNumber)
                quest:SetStateInt("CurrentPlayListIndex", activeIdx + 1)

                -- Spawn glowing teleporter light effect on the stone
                if not myEffect then
                    local pos = me:GetPos()
                    myEffect = quest:CreateEffectAtPos("MARKTELEPORTER", { x = pos.x, y = pos.y, z = pos.z + 1.0 })
                end
            end
        end

        if not quest:NewScriptFrame(me) then
            if myEffect then
                quest:RemoveThing(myEffect)
                myEffect = nil
            end
            return
        end
    end
end
