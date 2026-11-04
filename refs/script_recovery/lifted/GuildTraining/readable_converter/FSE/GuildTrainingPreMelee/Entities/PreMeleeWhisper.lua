-- Readable native conversion: PreMeleeWhisper. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- PreMeleeWhisper.Main (retail 0x00d524a0)
function Main(quest, me)
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
    local dist, scratchValue6, p4, p5, getPos, scratchValue
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
    local hero = quest:GetHero()
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
            local scratchValue12 = resources:ScriptThing(resource)
            if 0.0 < quest:GetHealth(scratchValue12) then
                p5 = 0
                p4 = 1
                local p1 = "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CHAT"
                local hero2 = quest:GetHero()
                if not me:Speak(hero2, p1, 0, false, true, false) then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    quest:DeregisterTimer(timerId)
                    resources:ReleaseResource(resource)
                    return
                end
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
            local hero3 = quest:GetHero()
            me:FollowThing(hero3, 1.0, true)
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(movie)
        end
        local preMeleeChatMarker2 = quest:GetNearestWithScriptName(me, "PreMeleeChatMarker")
        local hero4 = quest:GetHero()
        if not (preMeleeChatMarker2 ~= nil and not preMeleeChatMarker2:IsNull()) then
            getPos = {x = 0, y = 0, z = 0}
        else
            getPos = preMeleeChatMarker2:GetPos()
        end
        local position2 = hero4:GetPos()
        local f_stk_94_1 = string.unpack("<f", string.pack("<f", getPos.z - position2.z))
        if quest:IsDistanceBetweenThingsUnder(me, preMeleeChatMarker2, 7.0) then
            dist = 7.0
            local hero5 = quest:GetHero()
            local sequence = not quest:IsDistanceBetweenThingsUnder(me, hero5, 7.0) or 5 < quest:GetTimer(timerId) or 1.0 < math.abs(f_stk_94_1)
            if sequence then goto LAB_00d52d56 end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            local dataString = preMeleeChatMarker2:GetDataString()
            local f_stk_94_2 = parseGameInteger(dataString)
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
                    local dataString2 = preMeleeChatMarker[scratchValue6 + 1]:GetDataString()
                    if parseGameInteger(dataString2) ~= f_stk_94_2 then scratchValue = scratchValue + 1; scratchValue6 = scratchValue6 + 1; goto continue_3 end
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        resources:ReleaseResource(resource)
                        return
                    end
                    quest:RemoveThing(preMeleeChatMarker[scratchValue6 + 1], false, true)
                    scratchValue = scratchValue + 1
                    scratchValue6 = scratchValue6 + 1
                    ::continue_3::
                until not (scratchValue < (#preMeleeChatMarker))
            end
            if quest:IsActiveThreadTerminating() then
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource)
                return
            end
            local conversationId = quest:AddNewConversation(me, false, false)
            local hero6 = quest:GetHero()
            quest:AddPersonToConversation(conversationId, hero6)
            quest:SetTimer(timerId, 10)
            local switch = f_stk_94_2
            repeat
                if switch == 2 then
                    local hero7 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_LIBRARY", me, hero7, false)
                    break
                elseif switch == 4 then
                    local hero8 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SHOP", me, hero8, false)
                    break
                elseif switch == 5 then
                    local hero9 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_CLOISTERS", me, hero9, false)
                    break
                elseif switch == 6 then
                    local hero10 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE1", me, hero10, false)
                    break
                elseif switch == 7 then
                    local hero11 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE2", me, hero11, false)
                    break
                elseif switch == 8 then
                    local hero12 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE3", me, hero12, false)
                    break
                elseif switch == 9 then
                    local hero13 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_GRAVE4", me, hero13, false)
                    break
                elseif switch == 10 then
                    local hero14 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAZE", me, hero14, false)
                    break
                elseif switch == 11 then
                    local hero15 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WILL", me, hero15, false)
                    break
                elseif switch == 13 then
                    local hero16 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_WOODS", me, hero16, false)
                    break
                elseif switch == 14 then
                    local hero17 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SKILL", me, hero17, false)
                    break
                elseif switch == 15 then
                    local hero18 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_SERVANTS", me, hero18, false)
                    break
                elseif switch == 16 then
                    local hero19 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_MAIN_DORM", me, hero19, false)
                    break
                elseif switch == 19 then
                    local hero20 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DOOR", me, hero20, false)
                    break
                elseif switch == 20 then
                    local hero21 = quest:GetHero()
                    quest:AddLineToConversation(conversationId, "TEXT_QST_028_TEEN_WHISPER_PRE_MELEE_GUIDE_DINING_ROOM", me, hero21, false)
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

