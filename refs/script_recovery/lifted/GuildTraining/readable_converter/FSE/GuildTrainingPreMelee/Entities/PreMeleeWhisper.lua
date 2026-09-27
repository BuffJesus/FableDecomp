-- Readable native conversion: PreMeleeWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

-- PreMeleeWhisper.Main (retail 0x00d524a0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
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
    local scratchValue6, switch, hero4, getPos, scratchValue
    quest:EntitySetAsKillable(me, false, true)
    me:SetFriendsWithEverythingFlag(true)
    while not quest:GetStateBool("WhisperCutsceneFinished") do
        if not quest:NewScriptFrame(me) then return end
    end
    if quest:IsActiveThreadTerminating() then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource); return end
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    me:FollowThing(hero, 1.0, true)
    while not quest:GetStateBool("WhisperStopFollowing") do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource)
            return
        end
        if me:IsTalkedToByHero() then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
            local movie = resources:StartMovie("")
            quest:PauseAllNonScriptedEntities(true)
            me:ClearCommands()
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                me:Speak(hero, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CHAT", GROUP_SELECT_FIRST, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        do return end
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
            end
            me:FollowThing(hero, 1.0, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        local preMeleeChatMarker2 = quest:GetNearestWithScriptName(me, "PreMeleeChatMarker")
        hero4 = hero
        if not (preMeleeChatMarker2 ~= nil and not preMeleeChatMarker2:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = preMeleeChatMarker2:GetPos()
        end
        local f_stk_94_1 = getPos.z - hero:GetPos().z
        if quest:IsDistanceBetweenThingsUnder(me, preMeleeChatMarker2, 7.0) then
            local sequence = not quest:IsDistanceBetweenThingsUnder(me, hero, 7.0) or 5 < quest:GetTimer(timerId) or 1.0 < math.abs(f_stk_94_1)
            if sequence then goto LAB_00d52d56 end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            local f_stk_94_2 = parseGameInteger(preMeleeChatMarker2:GetDataString())
            scratchValue = 0
            local preMeleeChatMarker = quest:GetAllThingsWithScriptName("PreMeleeChatMarker")
            if #preMeleeChatMarker ~= 0 then
                scratchValue6 = 0
                repeat
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    if parseGameInteger(preMeleeChatMarker[scratchValue6 + 1]:GetDataString()) ~= f_stk_94_2 then scratchValue = scratchValue + 1; scratchValue6 = scratchValue6 + 1; goto continue_3 end
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:RemoveThing(preMeleeChatMarker[scratchValue6 + 1], false, true)
                    scratchValue = scratchValue + 1
                    scratchValue6 = scratchValue6 + 1
                    ::continue_3::
                until scratchValue >= #preMeleeChatMarker
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            local conversationId = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(conversationId, hero)
            quest:SetTimer(timerId, 10)
            switch = f_stk_94_2
            repeat
                if f_stk_94_2 == 2 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_LIBRARY", me, hero, false)
                    break
                elseif f_stk_94_2 == 4 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SHOP", me, hero, false)
                    break
                elseif f_stk_94_2 == 5 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CLOISTERS", me, hero, false)
                    break
                elseif f_stk_94_2 == 6 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE1", me, hero, false)
                    break
                elseif f_stk_94_2 == 7 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE2", me, hero, false)
                    break
                elseif f_stk_94_2 == 8 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE3", me, hero, false)
                    break
                elseif f_stk_94_2 == 9 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE4", me, hero, false)
                    break
                elseif f_stk_94_2 == 10 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAZE", me, hero, false)
                    break
                elseif f_stk_94_2 == 11 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WILL", me, hero, false)
                    break
                elseif f_stk_94_2 == 13 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WOODS", me, hero, false)
                    break
                elseif f_stk_94_2 == 14 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SKILL", me, hero, false)
                    break
                elseif f_stk_94_2 == 15 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SERVANTS", me, hero, false)
                    break
                elseif f_stk_94_2 == 16 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAIN_DORM", me, hero, false)
                    break
                elseif f_stk_94_2 == 19 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DOOR", me, hero, false)
                    break
                elseif f_stk_94_2 == 20 then
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DINING_ROOM", me, hero, false)
                else
                    break
                end
            until true
        end
        ::LAB_00d52d56::
    end
    if not quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource); return end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
    do return end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource)
end

-- PreMeleeWhisper.Init (retail 0x00d522e0)
function Init(quest, me)
end

-- PreMeleeWhisper.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- PreMeleeWhisper.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

