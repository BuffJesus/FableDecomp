-- Readable native conversion: SingingStone. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local self0X14, myStoneNumber, myParticleEmitter

-- SingingStone.Main (retail 0x00ed3370)
function Main(quest, me)
    local scratchValue, scratchValue2, scratchValue3, predicateResult, ctr_40, ctr_4c, scratchValue6
    local position3, scratchValue8
    local self_0x14
    local hero = quest:GetHero()
    scratchValue = 0
    local speakMarker = quest:GetNearestWithScriptName(me, "SpeakMarker")
    ctr_40 = 0
    if 0 < quest:GetStateInt("CurrentPlayListIndex") then
        ctr_4c = 108
        goto LAB_00ed33d0
    end
    goto FLOW_past_lab_00ed33d0
    ::LAB_00ed33d0::
    -- TODO(native): if *(self_0x14 + ctr_4c) ~= myStoneNumber then goto LAB_00ed34e2 end
    if not quest:IsActiveThreadTerminating() then
        repeat
            if quest:IsDistanceBetweenThingsUnder(me, hero, 35.0) then
                if quest:IsCameraPosOnScreen(me:GetPos()) then goto LAB_00ed344b end
            end
            if not quest:NewScriptFrame(me) then goto LAB_00ed3906 end
        until false
    end
    goto LAB_00ed38d9
    ::FLOW_past_lab_00ed33d0::
    ::LAB_00ed3504::
    while not quest:IsActiveThreadTerminating() do
        scratchValue6 = 0
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
            -- TODO(native): xStack_4d = '\0';
        end
        goto FLOW_past_lab_00ed3597
        ::LAB_00ed3597::
        -- TODO(native): xStack_4d = '\x01';
        ::FLOW_past_lab_00ed3597::
        if scratchValue2 & 4 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffb
            ctr_4c = scratchValue2
        end
        if scratchValue2 & 2 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffd
            ctr_4c = scratchValue2
        end
        if scratchValue2 & 1 ~= 0 then
            scratchValue2 = scratchValue2 & 0xfffffffe
            ctr_4c = scratchValue2
        end
        scratchValue = scratchValue2
        if scratchValue8 ~= 0 then
            if quest:IsActiveThreadTerminating() then goto LAB_00ed38d9 end
            predicateResult = false
            if 0 < quest:GetStateInt("CurrentPlayListIndex") then
                repeat
                    -- TODO(native): if *(self_0x14 + iVar8) == myStoneNumber then
                    scratchValue6 = scratchValue6 + 1; goto continue_1
                    if quest:IsActiveThreadTerminating() then goto LAB_00ed3906 end
                    predicateResult = true
                    scratchValue6 = scratchValue6 + 1
                    ::continue_1::
                until scratchValue6 >= quest:GetStateInt("CurrentPlayListIndex")
                scratchValue = ctr_4c
                if predicateResult then goto LAB_00ed3899 end
            end
            if quest:IsActiveThreadTerminating() then return end
            local conversationId = quest:AddNewConversation(speakMarker, true, true)
            quest:AddPersonToConversation(conversationId, hero)
            local switch = myStoneNumber
            repeat
                if myStoneNumber == 0 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_A", speakMarker, hero, false)
                    break
                elseif myStoneNumber == 1 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_B", speakMarker, hero, false)
                    break
                elseif myStoneNumber == 2 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_C", speakMarker, hero, false)
                    break
                elseif myStoneNumber == 3 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_060_NAME_D", speakMarker, hero, false)
                    break
                else
                    break
                end
            until true
            -- TODO(native): *(undefined4 *) (*(int *)(this + 0x14) + 0x6c + *(int *)(*(int *)(this + 0x14) + 0x48) * 4) = *(undefined4 *)(this + 0x1c);
            quest:SetStateInt("CurrentPlayListIndex", quest:GetStateInt("CurrentPlayListIndex") + 1)
            local position = me:GetPos()
            quest:CreateEffectAtPos("MARKTELEPORTER", {x = position.x, y = position.y, z = position.z + 1.0}, 0.0, false)
            -- TODO(native): p0 = *(self_0x14 + 0x80)
            local p0 = nil --[[unresolved native value]]
            -- TODO(native): if p0 == *(self_0x14 + 0x84) then
            if p0 ~= nil then
                -- TODO(native): p0[1] = *(undefined4 *)(pCVar3 + 0x4);
                -- TODO(native): piVar1 = *(pCVar3 + 0x8)
    --[[unresolved native value]]
                -- TODO(native): p0[2] = piVar1;
                if nil ~= nil then
                    -- TODO(native): *piVar1 = *piVar1 + 1;
                end
            end
            -- TODO(native): (quest:GetStateListCount("EffectList") * 0xc) = (quest:GetStateListCount("EffectList") * 0xc) + 0xc;
            scratchValue = ctr_4c
        end
        ::LAB_00ed3899::
        quest:NewScriptFrame(me)
    end
    scratchValue3 = nil == nil
    if not scratchValue3 then
        -- TODO(native): *xStack_24 = *xStack_24 - 1;
        -- TODO(native): __native_condition_1 = *r1 ~= 0
        scratchValue3 = nil --[[unresolved native value]]
    end
    if scratchValue3 then goto LAB_00ed3906 end
    (nil):GetName()
    ::LAB_00ed3906::
    do return end
    ::LAB_00ed344b::
    if quest:IsActiveThreadTerminating() then return end
    position3 = me:GetPos()
    -- TODO(native): xStack_18 = (int *)(pCVar4.z + 1.0);
    -- TODO(native): xStack_18._4_4_ = pCVar4.y;
    -- TODO(native): xStack_18._0_4_ = pCVar4.x;
    quest:StateListPush("EffectList", quest:CreateEffectAtPos("MARKTELEPORTER", position3, 0.0, false, hero))
    ::LAB_00ed34e2::
    ctr_4c = ctr_4c + 4
    ctr_40 = ctr_40 + 1
    if quest:GetStateInt("CurrentPlayListIndex") <= ctr_40 then goto LAB_00ed3504 end
    goto LAB_00ed33d0
    ::LAB_00ed38d9::
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
    myStoneNumber = parseGameInteger(me:GetDataString())
    myParticleEmitter = quest:GetThingWithScriptName("M_WispMarker" .. me:GetDataString())
    quest:MiniMapAddMarker(me, "HUD_ORB_GREEN_SMALL")
    local switch1 = myStoneNumber
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

