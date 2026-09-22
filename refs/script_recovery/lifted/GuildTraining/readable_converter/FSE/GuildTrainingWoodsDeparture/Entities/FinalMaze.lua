-- Readable native conversion: FinalMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_LIGHTNING_SPELL = 11  -- EHeroAbility (Ego_r.pdb)

-- per-entity fields (native class members; one Lua state per entity instance)
local notFighting, notBeaten, beenHit

-- FinalMaze.Main (retail 0x00d647f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult3, scratchValue5, predicateResult4, predicateResult7, predicateResult8
    local scratchValue, predicateResult, predicateResult14, scratchValue19, addNewConversation
    local infoCounter, infoCounter2, infoCounter3, scratchValue32, scratchValue33, movie, actorMap2
    local resource, timerId
    scratchValue32 = 0
    if not quest:NewScriptFrame(me) then return end
    local resource2 = resources:NewResource()
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b9 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b9 end
    notFighting = true
    notBeaten = true
    beenHit = 0
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, hero, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            resources:ReleaseResource(resource2)
            do return end
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(resource)
        resources:ReleaseResource(resource2)
        return
    end
    actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "HERO", resource)
    resources:SetActor(actorMap2, "MAZE", resource2)
    movie = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_START", actorMap2, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(resource)
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    infoCounter3 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    resources:PrepareResource(resource2)
    quest:EntitySetBossPhase(me, 0)
    quest:EntitySetAsDamageable(hero, false)
    quest:UpdateQuestInfoCounter(infoCounter3, beenHit, -1)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    quest:CacheMusicSet(47)
    addNewConversation = 0
    while notBeaten do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue33 = scratchValue32 | 3
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            goto LAB_00d64cba
        else
            scratchValue33 = scratchValue32 | 15
            predicateResult3 = false
            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then goto LAB_00d64cba end
        end
        goto FLOW_past_lab_00d64cba
        ::LAB_00d64cba::
        predicateResult3 = true
        ::FLOW_past_lab_00d64cba::
        if scratchValue33 & 8 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffffff7
        end
        if scratchValue33 & 4 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffffffb
        end
        if scratchValue33 & 2 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffffffd
        end
        if scratchValue33 & 1 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffffffe
        end
        if predicateResult3 then
            local getStateInt = beenHit + 1
            beenHit = getStateInt
            if getStateInt == 7 then
                notBeaten = false
                quest:ModifyThingHealth(me, 1000.0, false)
            end
            quest:UpdateQuestInfoCounter(infoCounter3, beenHit, -1)
            scratchValue32 = scratchValue33
            if not (quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation)) then goto continue_1 end
            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
            scratchValue32 = math.random(0, 32767) & 0x80000001
            scratchValue5 = scratchValue32 == 0
            if scratchValue32 < 0 then
                scratchValue5 = (scratchValue32 - 1 | 0xfffffffe) == 0xffffffff
            end
            if scratchValue5 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
            end
            quest:SetTimer(timerId, 5)
        else
            scratchValue32 = scratchValue33 | 48
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                goto LAB_00d64f41
            else
                scratchValue32 = scratchValue33 | 240
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
            if scratchValue32 < 0 then
                scratchValue32 = scratchValue32 & 0xffffff7f
            end
            if scratchValue32 & 64 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffffffbf
            end
            if scratchValue32 & 32 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffffffdf
            end
            if scratchValue32 & 16 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffffffef
            end
            if predicateResult4 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW", me, hero, false)
                quest:ModifyThingHealth(me, 1000.0, false)
            elseif me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                if not quest:IsConversationActive(addNewConversation) then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING", me, hero, false)
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
        ::continue_1::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    quest:RemoveQuestInfoElement(infoCounter3)
    quest:DisplayQuestInfo(false)
    beenHit = 0
    notBeaten = true
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    resource = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    quest:Pause(0.5)
    quest:EntitySetAsDrawable(hero, false)
    quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
        if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST", GROUP_SELECT_FIRST, false, true, false) then goto LAB_00d65b9f end
        if quest:IsActiveThreadTerminating() then goto LAB_00d65b9f end
        goto FLOW_past_lab_00d65b9f
        ::LAB_00d65b9f::
        quest:PauseAllNonScriptedEntities(false)
        resources:DestroyMovie(resource)
        goto LAB_00d664b0
        ::FLOW_past_lab_00d65b9f::
    end
    quest:EntitySetAsDrawable(hero, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(resource)
    resources:PrepareResource(resource2)
    quest:EntitySetBossPhase(me, 1)
    infoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    quest:UpdateQuestInfoCounter(infoCounter, beenHit, -1)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    while notBeaten do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue33 = scratchValue32 | 768
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            goto LAB_00d654d1
        else
            scratchValue33 = scratchValue32 | 3840
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
        if scratchValue33 & 2048 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffff7ff
        end
        if scratchValue33 & 1024 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffffbff
        end
        if scratchValue33 & 512 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffffdff
        end
        if scratchValue33 & 256 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffffeff
        end
        if predicateResult7 then
            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE", me, hero, false)
            quest:ModifyThingHealth(me, 1000.0, false)
        else
            scratchValue32 = scratchValue33 | 0x3000
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                goto LAB_00d65670
            else
                scratchValue32 = scratchValue33 | 0xf000
                predicateResult8 = false
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then goto LAB_00d65670 end
            end
            goto FLOW_past_lab_00d65670
            ::LAB_00d65670::
            predicateResult8 = true
            ::FLOW_past_lab_00d65670::
            if scratchValue32 >> 8 < 0 then
                scratchValue32 = scratchValue32 & 0xffff7fff
            end
            if scratchValue32 & 0x4000 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffffbfff
            end
            if scratchValue32 & 0x2000 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffffdfff
            end
            if scratchValue32 & 4096 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffffefff
            end
            if predicateResult8 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                local getStateInt2 = beenHit + 1
                beenHit = getStateInt2
                if getStateInt2 == 7 then
                    notBeaten = false
                    quest:ModifyThingHealth(me, 1000.0, false)
                end
                quest:UpdateQuestInfoCounter(infoCounter, beenHit, -1)
                if not (quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation)) then goto continue_2 end
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                scratchValue33 = math.random(0, 32767) & 0x80000001
                scratchValue = scratchValue33 == 0
                if scratchValue33 < 0 then
                    scratchValue = (scratchValue33 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
                else
                    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
                end
                quest:SetTimer(timerId, 5)
            elseif me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                if not quest:IsConversationActive(addNewConversation) then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING", me, hero, false)
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
        ::continue_2::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    quest:RemoveQuestInfoElement(infoCounter)
    quest:DisplayQuestInfo(false)
    beenHit = 0
    notBeaten = true
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    resource = resources:StartMovie("")
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    quest:Pause(0.5)
    quest:EntitySetAsDrawable(hero, false)
    quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
    if 0.0 < quest:GetHealth(resources:ScriptThing(resource2)) then
        me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST", GROUP_SELECT_FIRST, false, true, false)
        while me:IsPerformingScriptTask() do
            quest:NewScriptFrame(me)
            if quest:IsActiveThreadTerminating() then
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(resource)
                goto LAB_00d664b0
            end
        end
        if quest:IsActiveThreadTerminating() then
            quest:PauseAllNonScriptedEntities(false)
            resources:DestroyMovie(resource)
            goto LAB_00d664b0
        end
    end
    quest:EntitySetAsDrawable(hero, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(resource)
    resources:PrepareResource(resource2)
    quest:EntitySetBossPhase(me, 2)
    infoCounter2 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    quest:UpdateQuestInfoCounter(infoCounter2, beenHit, -1)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    while notBeaten do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue33 = scratchValue32 | 0x30000
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            goto LAB_00d65d51
        else
            scratchValue33 = scratchValue32 | 0xf0000
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
        if scratchValue33 & 0x80000 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfff7ffff
        end
        if scratchValue33 & 0x40000 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffbffff
        end
        if scratchValue33 & 0x20000 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffdffff
        end
        if scratchValue33 & 0x10000 ~= 0 then
            scratchValue33 = scratchValue33 & 0xfffeffff
        end
        if predicateResult then
            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
            addNewConversation = quest:AddNewConversation(me, false, false)
            quest:AddPersonToConversation(addNewConversation, hero)
            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE", me, hero, false)
            goto LAB_00d65e4c
        else
            scratchValue32 = scratchValue33 | 0x300000
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                goto LAB_00d65ef7
            else
                scratchValue32 = scratchValue33 | 0xf00000
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
            if scratchValue32 & 0x800000 ~= 0 then
                scratchValue32 = scratchValue32 & 0xff7fffff
            end
            if scratchValue32 & 0x400000 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffbfffff
            end
            if scratchValue32 & 0x200000 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffdfffff
            end
            if scratchValue32 & 0x100000 ~= 0 then
                scratchValue32 = scratchValue32 & 0xffefffff
            end
            if predicateResult14 then
                if not quest:IsActiveThreadTerminating() then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW", me, hero, false)
                    goto LAB_00d65e4c
                end
                goto LAB_00d664b0
            end
            if me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_LIGHTNING_SPELL) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                local getStateInt3 = beenHit + 1
                beenHit = getStateInt3
                if getStateInt3 == 7 then
                    notBeaten = false
                    quest:ModifyThingHealth(me, 1000.0, false)
                end
                quest:UpdateQuestInfoCounter(infoCounter2, beenHit, -1)
                if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                    scratchValue33 = math.random(0, 32767) & 0x80000001
                    scratchValue19 = scratchValue33 == 0
                    if scratchValue33 < 0 then
                        scratchValue19 = (scratchValue33 - 1 | 0xfffffffe) == 0xffffffff
                    end
                    if scratchValue19 then
                        if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
                    else
                        if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                        addNewConversation = quest:AddNewConversation(me, false, false)
                        quest:AddPersonToConversation(addNewConversation, hero)
                        quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
                    end
                    quest:SetTimer(timerId, 5)
                end
            end
        end
        goto FLOW_past_lab_00d65e4c
        ::LAB_00d65e4c::
        quest:ModifyThingHealth(me, 1000.0, false)
        ::FLOW_past_lab_00d65e4c::
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    quest:RemoveQuestInfoElement(infoCounter2)
    quest:DisplayQuestInfo(false)
    resources:PrepareResource(resource2)
    while not resources:TryAcquire(resource2, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
    me:ClearCommands()
    quest:EntitySetInFaction(me, "FACTION_HERO")
    resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, hero, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d6632b end
    end
    if quest:IsActiveThreadTerminating() then
        goto LAB_00d6632b
    else
        local actorMap = resources:NewActorMap()
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
        quest:EntitySetAsDamageable(hero, true)
    end
    goto FLOW_past_lab_00d6632b
    ::LAB_00d6632b::
    resources:ReleaseResource(resource)
    ::FLOW_past_lab_00d6632b::
    ::LAB_00d664b0::
    quest:DeregisterTimer(timerId)
    ::LAB_00d664b9::
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

