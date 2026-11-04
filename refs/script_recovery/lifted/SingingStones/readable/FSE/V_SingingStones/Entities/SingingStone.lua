-- Readable native conversion: SingingStone. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local myStoneNumber_, myParticleEmitter

-- SingingStone.Main (retail 0x00ed3370)
function Main(quest, me)
    local scratchValue, scratchValue2, predicateResult, ctr_40, ctr_4c, dist, scratchValue4
    local scratchValue5, createEffectAtPos, scratchValue6, position3, speakMarker, scratchValue7
    local myStoneNumber = myStoneNumber_
    scratchValue = 0
    speakMarker = quest:GetNearestWithScriptName(me, "SpeakMarker")
    ctr_40 = 0
    if 0 < quest:GetStateInt("CurrentPlayListIndex") then
        ctr_4c = 108
    else
        goto FLOW_past_lab_00ed33d0
    end
    ::LAB_00ed33d0::
    if quest:GetStateInt("YourPersonalPlayList_" .. ctr_40) ~= myStoneNumber then goto LAB_00ed34e2 end
    if not quest:IsActiveThreadTerminating() then
        repeat
            dist = 35.0
            local hero = quest:GetHero()
            if quest:IsDistanceBetweenThingsUnder(me, hero, 35.0) then
                local position = me:GetPos()
                if quest:IsCameraPosOnScreen(position) then goto LAB_00ed344b end
            end
            if not quest:NewScriptFrame(me) then goto LAB_00ed3906 end
        until false
    end
    goto LAB_00ed38d9
    ::FLOW_past_lab_00ed33d0::
    while not quest:IsActiveThreadTerminating() do
        scratchValue5 = 0
        scratchValue2 = scratchValue | 1
        ctr_4c = scratchValue2
        if me:MsgIsHitByHero() then
            goto LAB_00ed3597
        else
            scratchValue2 = scratchValue | 3
            ctr_4c = scratchValue2
            if me:MsgIsHitByAnySpecialAbilityFromHero() then
                scratchValue2 = scratchValue | 7
                ctr_4c = scratchValue2
                if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00ed3597 end
            end
            scratchValue7 = 0
        end
        goto FLOW_past_lab_00ed3597
        ::LAB_00ed3597::
        scratchValue7 = 1
        ::FLOW_past_lab_00ed3597::
        if (scratchValue2 & 4) ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffb
            ctr_4c = scratchValue2
        end
        if (scratchValue2 & 2) ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffd
            ctr_4c = scratchValue2
        end
        if (scratchValue2 & 1) ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffe
            ctr_4c = scratchValue2
        end
        scratchValue = scratchValue2
        if scratchValue7 ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00ed38d9 end
            predicateResult = false
            if 0 < quest:GetStateInt("CurrentPlayListIndex") then
                scratchValue4 = 108
                repeat
                    if quest:GetStateInt("YourPersonalPlayList_" .. scratchValue5) ~= myStoneNumber then scratchValue5 = scratchValue5 + 1; scratchValue4 = scratchValue4 + 4; goto continue_1 end
                    if quest:IsActiveThreadTerminating() then speakMarker = nil; goto LAB_00ed3906 end
                    predicateResult = true
                    scratchValue5 = scratchValue5 + 1
                    scratchValue4 = scratchValue4 + 4
                    ::continue_1::
                until not (scratchValue5 < quest:GetStateInt("CurrentPlayListIndex"))
                scratchValue = ctr_4c
                if predicateResult then goto LAB_00ed3899 end
            end
            if quest:IsActiveThreadTerminating() then speakMarker = nil; return end
            local conversationId = quest:AddNewConversation(speakMarker, true, true)
            local hero2 = quest:GetHero()
            quest:AddPersonToConversation(conversationId, hero2)
            local switch = myStoneNumber
            repeat
                if switch == 0 then
                    local hero3 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_A", speakMarker, hero3, false)
                    break
                elseif switch == 1 then
                    local hero4 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_B", speakMarker, hero4, false)
                    break
                elseif switch == 2 then
                    local hero5 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_C", speakMarker, hero5, false)
                    break
                elseif switch == 3 then
                    local hero6 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_D", speakMarker, hero6, false)
                    break
                else
                    break
                end
            until true
            quest:SetStateInt("YourPersonalPlayList_" .. quest:GetStateInt("CurrentPlayListIndex"), myStoneNumber)
            quest:SetStateInt("CurrentPlayListIndex", quest:GetStateInt("CurrentPlayListIndex") + 1)
            local position2 = me:GetPos()
            local scratchValue8 = {x = position2.x, y = position2.y, z = position2.z + 1.0}
            createEffectAtPos = quest:CreateEffectAtPos("MARKTELEPORTER", scratchValue8, 0.0, false, false, "StonesEffect")
            quest:StateListPush("EffectList", createEffectAtPos)
            createEffectAtPos = nil
            scratchValue = ctr_4c
        end
        ::LAB_00ed3899::
        quest:NewScriptFrame(me)
    end
    ::LAB_00ed3906::
    do return end
    ::LAB_00ed344b::
    if quest:IsActiveThreadTerminating() then return end
    position3 = me:GetPos()
    scratchValue6 = quest:CreateEffectAtPos("MARKTELEPORTER", {x = position3.x, y = position3.y, z = position3.z + 1.0}, 0.0, false, false, "StonesEffect")
    quest:StateListPush("EffectList", scratchValue6)
    ::LAB_00ed34e2::
    ctr_4c = ctr_4c + 4
    ctr_40 = ctr_40 + 1
    if quest:GetStateInt("CurrentPlayListIndex") <= ctr_40 then goto FLOW_past_lab_00ed33d0 end
    goto LAB_00ed33d0
    ::LAB_00ed38d9::
    speakMarker = nil
    goto LAB_00ed3906
end

-- SingingStone.Init (retail 0x00ed14e0)
function Init(quest, me)
    local function parseGameInteger(text)
        local value, negative = 0, false
        for position = 1, #text do
            local character = text:sub(position, position)
            if character == "." then break end
            if character == "-" then
                negative = true
            elseif character >= "0" and character <= "9" then
                value = (value * 10 + tonumber(character)) % 4294967296
            end
        end
        if negative then value = (-value) % 4294967296 end
        -- Match the game's signed 32-bit result, including overflow.
        if value >= 2147483648 then value = value - 4294967296 end
        return value
    end
    local this_00 = me:GetDataString()
    myStoneNumber_ = parseGameInteger(this_00)
    local getDataString = "M_WispMarker" .. me:GetDataString()
    myParticleEmitter = quest:GetThingWithScriptName(getDataString)
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    local switch1 = myStoneNumber_
    repeat
        if switch1 == 0 then
            quest:SetReadableObjectTextTag(me, "TEXT_QST_060_NAME_A")
            return
        elseif switch1 == 1 then
            quest:SetReadableObjectTextTag(me, "TEXT_QST_060_NAME_B")
            return
        elseif switch1 == 2 then
            quest:SetReadableObjectTextTag(me, "TEXT_QST_060_NAME_C")
            return
        elseif switch1 == 3 then
            quest:SetReadableObjectTextTag(me, "TEXT_QST_060_NAME_D")
        end
    until true
end

-- SingingStone.OnPersist (retail 0x00ed14d0)
function OnPersist(quest, me, context)
end

-- SingingStone.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

