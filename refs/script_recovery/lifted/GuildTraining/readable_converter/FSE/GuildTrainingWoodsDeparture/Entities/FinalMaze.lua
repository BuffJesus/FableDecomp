-- Readable native conversion: FinalMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local HERO_ABILITY_LIGHTNING_SPELL = 11  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local notFighting, notBeaten, beenHit

-- FinalMaze.Main (retail 0x00d647f0)
function Main(quest, me)
    local resources = quest:RetailResources()
    local controlAcquired, predicateResult3, scratchValue5, predicateResult4, predicateResult7
    local predicateResult8, scratchValue12, predicateResult, predicateResult14, scratchValue19
    local controlAcquired6, getStateBool, addNewConversation, infoCounter, infoCounter2
    local infoCounter3, position, scratchValue28, line, pppuVar, scratchValue29, scratchValue30
    local scratchValue31, retailRandModulo, retailRandModulo2, scratchValue32, movie, actorMap
    local resource
    retailRandModulo = 0
    scratchValue32 = 0
    if not quest:NewScriptFrame(me) then return end
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then resources:ReleaseResource(resource2); return end
    end
    if quest:IsActiveThreadTerminating() then resources:ReleaseResource(resource2); return end
    notFighting = true
    notBeaten = true
    beenHit = 0
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    pppuVar = resource
    local hero2 = quest:GetHero()
    controlAcquired = resources:TryAcquire(pppuVar, hero2, 4)
    while not controlAcquired do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            resources:ReleaseResource(resource2)
            return
        end
        pppuVar = resource
        local hero3 = quest:GetHero()
        controlAcquired = resources:TryAcquire(pppuVar, hero3, 4)
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource)
        resources:ReleaseResource(resource2)
        return
    end
    actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    resources:SetActor(actorMap, "MAZE", resource2)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_START", actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    infoCounter3 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    resources:PrepareResource(resource2)
    quest:EntitySetBossPhase(me, 0)
    local hero4 = quest:GetHero()
    quest:EntitySetAsDamageable(hero4, false)
    quest:UpdateQuestInfoCounter(infoCounter3, beenHit, -1)
    local timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    position = me:GetPos()
    scratchValue29 = quest:EntityWillTeleportToArea(me, position.x, 5.0, 20.0)
    quest:CacheMusicSet(47)
    addNewConversation = 0
    while notBeaten do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
        retailRandModulo2 = retailRandModulo | 3
        scratchValue32 = retailRandModulo2
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            goto LAB_00d64cba
        else
            retailRandModulo2 = retailRandModulo | 15
            scratchValue32 = retailRandModulo2
            predicateResult3 = false
            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then goto LAB_00d64cba end
        end
        goto FLOW_past_lab_00d64cba
        ::LAB_00d64cba::
        predicateResult3 = true
        ::FLOW_past_lab_00d64cba::
        if (retailRandModulo2 & 8) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffffff7
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 4) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffffffb
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 2) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffffffd
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 1) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffffffe
            scratchValue32 = retailRandModulo2
        end
        if predicateResult3 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
            local getStateInt = beenHit + 1
            beenHit = getStateInt
            if getStateInt == 7 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                notBeaten = false
                quest:ModifyThingHealth(me, 1000.0, false)
            end
            quest:UpdateQuestInfoCounter(infoCounter3, beenHit, -1)
            retailRandModulo = retailRandModulo2
            local timeRemaining = quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation)
            if not timeRemaining then goto continue_1 end
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
            retailRandModulo = quest:RetailRandModulo(32768) & 0x80000001
            scratchValue5 = retailRandModulo == 0
            if retailRandModulo < 0 then
                scratchValue5 = (retailRandModulo - 1 | 0xfffffffe) == 0xffffffff
            end
            if scratchValue5 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                addNewConversation = quest:AddNewConversation(me, false, false)
                local hero5 = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, hero5)
                local hero6 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero6, false)
            else
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                addNewConversation = quest:AddNewConversation(me, false, false)
                local hero7 = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, hero7)
                local hero8 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero8, false)
            end
            quest:SetTimer(timerId, 5)
            retailRandModulo = scratchValue32
        else
            retailRandModulo = retailRandModulo2 | 48
            scratchValue32 = retailRandModulo
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                goto LAB_00d64f41
            else
                retailRandModulo = retailRandModulo2 | 240
                scratchValue32 = retailRandModulo
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then goto LAB_00d64f41 end
                goto LAB_00d64f56
            end
            goto FLOW_past_lab_00d64f41
            ::LAB_00d64f41::
            predicateResult4 = true
            if quest:IsConversationActive(addNewConversation) then goto LAB_00d64f56 end
            ::FLOW_past_lab_00d64f41::
            goto FLOW_past_lab_00d64f56
            ::LAB_00d64f56::
            predicateResult4 = false
            ::FLOW_past_lab_00d64f56::
            if (retailRandModulo & 128) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffffff7f
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 64) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffffffbf
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 32) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffffffdf
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 16) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffffffef
                scratchValue32 = retailRandModulo
            end
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                addNewConversation = quest:AddNewConversation(me, false, false)
                local hero9 = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, hero9)
                local hero10 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW", me, hero10, false)
                quest:ModifyThingHealth(me, 1000.0, false)
                retailRandModulo = scratchValue32
            elseif me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                if not quest:IsConversationActive(addNewConversation) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    local hero11 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero11)
                    local hero12 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING", me, hero12, false)
                    retailRandModulo = scratchValue32
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    quest:RemoveQuestInfoElement(infoCounter3)
    quest:DisplayQuestInfo(false)
    beenHit = 0
    notBeaten = true
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    resource = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    quest:Pause(0.5)
    local hero13 = quest:GetHero()
    quest:EntitySetAsDrawable(hero13, false)
    scratchValue28 = quest:GetThingWithScriptName("CAM_RC_MAZE")
    quest:CameraUseCameraPoint(scratchValue28, me, -1.0, 0, -1)
    scratchValue28 = resources:ScriptThing(resource2)
    if 0.0 < quest:GetHealth(scratchValue28) then
        line = "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST"
        local hero14 = quest:GetHero()
        if me:Speak(hero14, line, 0, false, true, false) then
            while me:IsPerformingScriptTask() do
                if not quest:NewScriptFrame(me) then goto LAB_00d65b9f end
            end
            if not (quest:IsActiveThreadTerminating()) then
                goto FLOW_past_lab_00d65b9f
            end
        end
        ::LAB_00d65b9f::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(resource)
        quest:DeregisterTimer(timerId)
        resources:ReleaseResource(resource2)
        do return end
        ::FLOW_past_lab_00d65b9f::
    end
    local hero15 = quest:GetHero()
    quest:EntitySetAsDrawable(hero15, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(resource)
    resources:PrepareResource(resource2)
    quest:EntitySetBossPhase(me, 1)
    infoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    infoCounter3 = infoCounter
    quest:DisplayQuestInfo(true)
    quest:UpdateQuestInfoCounter(infoCounter, beenHit, -1)
    position = me:GetPos()
    scratchValue30 = quest:EntityWillTeleportToArea(me, position.x, 5.0, 20.0)
    getStateBool = notBeaten
    retailRandModulo = scratchValue32
    while getStateBool do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
        retailRandModulo2 = retailRandModulo | 768
        scratchValue32 = retailRandModulo2
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            goto LAB_00d654d1
        else
            retailRandModulo2 = retailRandModulo | 3840
            scratchValue32 = retailRandModulo2
            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then goto LAB_00d654d1 end
            goto LAB_00d654e6
        end
        goto FLOW_past_lab_00d654d1
        ::LAB_00d654d1::
        predicateResult7 = true
        if quest:IsConversationActive(addNewConversation) then goto LAB_00d654e6 end
        ::FLOW_past_lab_00d654d1::
        goto FLOW_past_lab_00d654e6
        ::LAB_00d654e6::
        predicateResult7 = false
        ::FLOW_past_lab_00d654e6::
        if (retailRandModulo2 & 2048) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffff7ff
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 1024) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffffbff
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 512) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffffdff
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 256) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffffeff
            scratchValue32 = retailRandModulo2
        end
        if predicateResult7 then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
            addNewConversation = quest:AddNewConversation(me, false, false)
            local hero16 = quest:GetHero()
            quest:AddPersonToConversation(addNewConversation, hero16)
            local hero17 = quest:GetHero()
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE", me, hero17, false)
            quest:ModifyThingHealth(me, 1000.0, false)
            retailRandModulo = scratchValue32
        else
            retailRandModulo = retailRandModulo2 | 0x3000
            scratchValue32 = retailRandModulo
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                goto LAB_00d65670
            else
                retailRandModulo = retailRandModulo2 | 0xf000
                scratchValue32 = retailRandModulo
                predicateResult8 = false
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then goto LAB_00d65670 end
            end
            goto FLOW_past_lab_00d65670
            ::LAB_00d65670::
            predicateResult8 = true
            ::FLOW_past_lab_00d65670::
            if (retailRandModulo & 0x8000) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffff7fff
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 0x4000) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffffbfff
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 0x2000) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffffdfff
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 4096) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffffefff
                scratchValue32 = retailRandModulo
            end
            if predicateResult8 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                local getStateInt2 = beenHit + 1
                beenHit = getStateInt2
                if getStateInt2 == 7 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                    notBeaten = false
                    quest:ModifyThingHealth(me, 1000.0, false)
                end
                quest:UpdateQuestInfoCounter(infoCounter3, beenHit, -1)
                local timeRemaining2 = quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation)
                if not timeRemaining2 then infoCounter = infoCounter3; getStateBool = notBeaten; goto continue_2 end
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                retailRandModulo2 = quest:RetailRandModulo(32768) & 0x80000001
                scratchValue12 = retailRandModulo2 == 0
                if retailRandModulo2 < 0 then
                    scratchValue12 = (retailRandModulo2 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue12 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    local hero18 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero18)
                    local hero19 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero19, false)
                else
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    local hero20 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero20)
                    local hero21 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero21, false)
                end
                quest:SetTimer(timerId, 5)
            elseif me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                if not quest:IsConversationActive(addNewConversation) then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    local hero22 = quest:GetHero()
                    quest:AddPersonToConversation(addNewConversation, hero22)
                    local hero23 = quest:GetHero()
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING", me, hero23, false)
                    retailRandModulo = scratchValue32
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
        infoCounter = infoCounter3
        getStateBool = notBeaten
        ::continue_2::
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    quest:RemoveQuestInfoElement(infoCounter)
    quest:DisplayQuestInfo(false)
    beenHit = 0
    notBeaten = true
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    resource = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    quest:Pause(0.5)
    local hero24 = quest:GetHero()
    quest:EntitySetAsDrawable(hero24, false)
    scratchValue28 = quest:GetThingWithScriptName("CAM_RC_MAZE")
    quest:CameraUseCameraPoint(scratchValue28, me, -1.0, 0, -1)
    scratchValue28 = resources:ScriptThing(resource2)
    if 0.0 < quest:GetHealth(scratchValue28) then
        line = "TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST"
        local hero25 = quest:GetHero()
        if not me:Speak(hero25, line, 0, false, true, false) then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource2)
            return
        end
        while me:IsPerformingScriptTask() do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(resource)
                quest:DeregisterTimer(timerId)
                resources:ReleaseResource(resource2)
                do return end
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource)
            quest:DeregisterTimer(timerId)
            resources:ReleaseResource(resource2)
            return
        end
    end
    local hero26 = quest:GetHero()
    quest:EntitySetAsDrawable(hero26, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(resource)
    resources:PrepareResource(resource2)
    quest:EntitySetBossPhase(me, 2)
    infoCounter2 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    infoCounter3 = infoCounter2
    quest:DisplayQuestInfo(true)
    quest:UpdateQuestInfoCounter(infoCounter2, beenHit, -1)
    position = me:GetPos()
    scratchValue31 = quest:EntityWillTeleportToArea(me, position.x, 5.0, 20.0)
    getStateBool = notBeaten
    retailRandModulo = scratchValue32
    while getStateBool do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
        retailRandModulo2 = retailRandModulo | 0x30000
        scratchValue32 = retailRandModulo2
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            goto LAB_00d65d51
        else
            retailRandModulo2 = retailRandModulo | 0xf0000
            scratchValue32 = retailRandModulo2
            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then goto LAB_00d65d51 end
            goto LAB_00d65d66
        end
        goto FLOW_past_lab_00d65d51
        ::LAB_00d65d51::
        predicateResult = true
        if quest:IsConversationActive(addNewConversation) then goto LAB_00d65d66 end
        ::FLOW_past_lab_00d65d51::
        goto FLOW_past_lab_00d65d66
        ::LAB_00d65d66::
        predicateResult = false
        ::FLOW_past_lab_00d65d66::
        if (retailRandModulo2 & 0x80000) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfff7ffff
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 0x40000) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffbffff
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 0x20000) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffdffff
            scratchValue32 = retailRandModulo2
        end
        if (retailRandModulo2 & 0x10000) ~= 0 then
            retailRandModulo2 = retailRandModulo2 & 0xfffeffff
            scratchValue32 = retailRandModulo2
        end
        if predicateResult then
            if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
            addNewConversation = quest:AddNewConversation(me, false, false)
            local hero27 = quest:GetHero()
            quest:AddPersonToConversation(addNewConversation, hero27)
            local hero28 = quest:GetHero()
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE", me, hero28, false)
            goto LAB_00d65e4c
        else
            retailRandModulo = retailRandModulo2 | 0x300000
            scratchValue32 = retailRandModulo
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                goto LAB_00d65ef7
            else
                retailRandModulo = retailRandModulo2 | 0xf00000
                scratchValue32 = retailRandModulo
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then goto LAB_00d65ef7 end
                goto LAB_00d65f0c
            end
            goto FLOW_past_lab_00d65ef7
            ::LAB_00d65ef7::
            predicateResult14 = true
            if quest:IsConversationActive(addNewConversation) then goto LAB_00d65f0c end
            ::FLOW_past_lab_00d65ef7::
            goto FLOW_past_lab_00d65f0c
            ::LAB_00d65f0c::
            predicateResult14 = false
            ::FLOW_past_lab_00d65f0c::
            if (retailRandModulo & 0x800000) ~= 0 then
                retailRandModulo = retailRandModulo & 0xff7fffff
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 0x400000) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffbfffff
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 0x200000) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffdfffff
                scratchValue32 = retailRandModulo
            end
            if (retailRandModulo & 0x100000) ~= 0 then
                retailRandModulo = retailRandModulo & 0xffefffff
                scratchValue32 = retailRandModulo
            end
            if predicateResult14 then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                addNewConversation = quest:AddNewConversation(me, false, false)
                local hero29 = quest:GetHero()
                quest:AddPersonToConversation(addNewConversation, hero29)
                local hero30 = quest:GetHero()
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW", me, hero30, false)
                goto LAB_00d65e4c
            end
            if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
                if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                local getStateInt3 = beenHit + 1
                beenHit = getStateInt3
                if getStateInt3 == 7 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                    notBeaten = false
                    quest:ModifyThingHealth(me, 1000.0, false)
                end
                quest:UpdateQuestInfoCounter(infoCounter3, beenHit, -1)
                local timeRemaining3 = quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation)
                if timeRemaining3 then
                    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                    retailRandModulo2 = quest:RetailRandModulo(32768) & 0x80000001
                    scratchValue19 = retailRandModulo2 == 0
                    if retailRandModulo2 < 0 then
                        scratchValue19 = (retailRandModulo2 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if scratchValue19 then
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        local hero31 = quest:GetHero()
                        quest:AddPersonToConversation(addNewConversation, hero31)
                        local hero32 = quest:GetHero()
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero32, false)
                    else
                        if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        local hero33 = quest:GetHero()
                        quest:AddPersonToConversation(addNewConversation, hero33)
                        local hero34 = quest:GetHero()
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero34, false)
                    end
                    quest:SetTimer(timerId, 5)
                end
            end
        end
        goto FLOW_past_lab_00d65e4c
        ::LAB_00d65e4c::
        quest:ModifyThingHealth(me, 1000.0, false)
        retailRandModulo = scratchValue32
        ::FLOW_past_lab_00d65e4c::
        infoCounter2 = infoCounter3
        getStateBool = notBeaten
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    quest:RemoveQuestInfoElement(infoCounter2)
    quest:DisplayQuestInfo(false)
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    end
    if quest:IsActiveThreadTerminating() then quest:DeregisterTimer(timerId); resources:ReleaseResource(resource2); return end
    me:ClearCommands()
    quest:EntitySetInFaction(me, "FACTION_HERO")
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    pppuVar = resource
    local hero35 = quest:GetHero()
    controlAcquired6 = resources:TryAcquire(pppuVar, hero35, 4)
    while not controlAcquired6 do
        if not quest:NewScriptFrame(me) then goto LAB_00d6632b end
        pppuVar = resource
        local hero36 = quest:GetHero()
        controlAcquired6 = resources:TryAcquire(pppuVar, hero36, 4)
    end
    if quest:IsActiveThreadTerminating() then
        goto LAB_00d6632b
    else
        actorMap = resources:NewActorMap()
        resources:SetActor(actorMap, "HERO", resource)
        resources:SetActor(actorMap, "MAZE", resource2)
        movie = resources:StartMovie("")
        quest:PauseAllNonScriptedEntities(true)
        quest:FixMovieSequenceCamera(true)
        resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_WIN", actorMap, false, true)
        quest:FixMovieSequenceCamera(false)
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(movie)
        resources:DestroyActorMap(actorMap)
        resources:ReleaseResource(resource)
        quest:SetStateBool("MissionSucceeded", true)
        quest:ModifyThingHealth(me, 1000.0, false)
        quest:RemoveThing(me, false, true)
        local hero = quest:GetHero()
        quest:EntitySetAsDamageable(hero, true)
    end
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource2)
    do return end
    ::LAB_00d6632b::
    resources:ReleaseResource(resource)
    quest:DeregisterTimer(timerId)
    resources:ReleaseResource(resource2)
end

-- FinalMaze.Init (retail 0x00d62320)
function Init(quest, me)
end

-- FinalMaze.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, me, context)
end

-- FinalMaze.OnPredicateFail (retail 0x00d62330)
function OnPredicateFail(quest, me)
end

