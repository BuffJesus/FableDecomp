-- Readable native conversion: FinalMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local state = {}  -- per-entity script state (__native_entity_state)
do
    local fields = {}
    for _, kind in ipairs({"Bool", "Int", "Float", "String", "Thing"}) do
        state["Get" .. kind] = function(_, name) return fields[name] end
        state["Set" .. kind] = function(_, name, value) fields[name] = value end
    end
end

-- FinalMaze.Main (retail 0x00d647f0)
function Main(quest, me)
    local hero = quest:GetHero()
    local resources = quest:RetailResources()
    local predicateResult3, scratchValue5, predicateResult4, predicateResult9, predicateResult
    local scratchValue, predicateResult15, predicateResult16, scratchValue19, addNewConversation
    local getStateInt, infoCounter, getStateInt2, infoCounter2, getStateInt3, actorMap
    local scratchValue27, scratchValue28, scratchValue29, scratchValue30, scratchValue31
    local scratchValue32, movie, movie2, actorMap2, scratchValue33, infoCounter3, resource, timerId
    scratchValue27 = 0
    if not quest:NewScriptFrame(me) then return end
    resource = resources:NewResource()
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b9 end
    end
    state:SetBool("NotFighting", true)
    state:SetBool("NotBeaten", true)
    state:SetInt("BeenHit", 0)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    scratchValue33 = resources:NewResource()
    while not resources:TryAcquire(scratchValue33, hero, 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(scratchValue33)
            resources:ReleaseResource(resource)
            return
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue33)
        resources:ReleaseResource(resource)
        return
    end
    actorMap2 = resources:NewActorMap()
    resources:SetActor(actorMap2, "HERO", scratchValue33)
    resources:SetActor(actorMap2, "MAZE", resource)
    movie = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_START", actorMap2, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap2)
    resources:ReleaseResource(scratchValue33)
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    infoCounter3 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    quest:EntitySetBossPhase(me, 0)
    quest:EntitySetAsDamageable(hero, false)
    quest:UpdateQuestInfoCounter(infoCounter3, state:GetInt("BeenHit"), -1)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    quest:CacheMusicSet(47)
    addNewConversation = 0
    while state:GetBool("NotBeaten") do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue28 = scratchValue27 | 3
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            predicateResult3 = true
        else
            scratchValue28 = scratchValue27 | 15
            predicateResult3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
        end
        if scratchValue28 & 8 ~= 0 then
            scratchValue28 = scratchValue28 & 0xfffffff7
        end
        if scratchValue28 & 4 ~= 0 then
            scratchValue28 = scratchValue28 & 0xfffffffb
        end
        if scratchValue28 & 2 ~= 0 then
            scratchValue28 = scratchValue28 & 0xfffffffd
        end
        if scratchValue28 & 1 ~= 0 then
            scratchValue28 = scratchValue28 & 0xfffffffe
        end
        if predicateResult3 then
            getStateInt = state:GetInt("BeenHit") + 1
            state:SetInt("BeenHit", getStateInt)
            if getStateInt == 7 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                state:SetBool("NotBeaten", false)
                quest:ModifyThingHealth(me, 1000.0, false)
            end
            quest:UpdateQuestInfoCounter(infoCounter3, state:GetInt("BeenHit"), -1)
            if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                scratchValue27 = scratchValue27 & 0x80000001
                scratchValue5 = scratchValue27 == 0
                if scratchValue27 < 0 then
                    scratchValue5 = (scratchValue27 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue5 then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
                else
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
                end
                quest:SetTimer(timerId, 5)
            end
        else
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                predicateResult4 = true
                if quest:IsConversationActive(addNewConversation) then
                    predicateResult4 = false
                    goto FLOW_after_lab_00d64f56
                end
            else
                scratchValue27 = scratchValue28 | 240
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then
                    predicateResult4 = true
                    if not quest:IsConversationActive(addNewConversation) then
                        goto FLOW_after_lab_00d64f56
                    end
                end
                predicateResult4 = false
            end
            ::FLOW_after_lab_00d64f56::
            if scratchValue27 < 0 then
                scratchValue27 = scratchValue27 & 0xffffff7f
            end
            if scratchValue27 & 64 ~= 0 then
                scratchValue27 = scratchValue27 & 0xffffffbf
            end
            if scratchValue27 & 32 ~= 0 then
                scratchValue27 = scratchValue27 & 0xffffffdf
            end
            if scratchValue27 & 16 ~= 0 then
                scratchValue27 = scratchValue27 & 0xffffffef
            end
            if predicateResult4 then
                addNewConversation = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(addNewConversation, hero)
                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW", me, hero, false)
                quest:ModifyThingHealth(me, 1000.0, false)
            elseif me:MsgIsHitByHeroSpecialAbility(me) then
                if not quest:IsConversationActive(addNewConversation) then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING", me, hero, false)
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveQuestInfoElement(infoCounter3)
        quest:DisplayQuestInfo(false)
        state:SetInt("BeenHit", 0)
        state:SetBool("NotBeaten", true)
        while not resources:TryAcquire(resource, me, 4) do
            if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue33 = resources:StartMovie("")
            quest:StartMovieSequence()
            -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
            quest:FixMovieSequenceCamera(true)
            quest:Pause(0.5)
            quest:EntitySetAsDrawable(hero, false)
            quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
            if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST", GROUP_SELECT_FIRST, false, true, false) then
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                    resources:DestroyMovie(scratchValue33)
                    goto LAB_00d664b0
                end
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                    resources:DestroyMovie(scratchValue33)
                    goto LAB_00d664b0
                end
            end
            quest:EntitySetAsDrawable(hero, true)
            quest:FixMovieSequenceCamera(false)
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
            resources:DestroyMovie(scratchValue33)
            quest:EntitySetBossPhase(me, 1)
            infoCounter = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
            quest:DisplayQuestInfo(true)
            quest:UpdateQuestInfoCounter(infoCounter, state:GetInt("BeenHit"), -1)
            quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
            while state:GetBool("NotBeaten") do
                if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                scratchValue29 = scratchValue27 | 768
                if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
                    predicateResult9 = true
                    if quest:IsConversationActive(addNewConversation) then
                        predicateResult9 = false
                        goto FLOW_after_lab_00d654e6
                    end
                else
                    scratchValue29 = scratchValue27 | 3840
                    if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then
                        predicateResult9 = true
                        if not quest:IsConversationActive(addNewConversation) then
                            goto FLOW_after_lab_00d654e6
                        end
                    end
                    predicateResult9 = false
                end
                ::FLOW_after_lab_00d654e6::
                if scratchValue29 & 2048 ~= 0 then
                    scratchValue29 = scratchValue29 & 0xfffff7ff
                end
                if scratchValue29 & 1024 ~= 0 then
                    scratchValue29 = scratchValue29 & 0xfffffbff
                end
                if scratchValue29 & 512 ~= 0 then
                    scratchValue29 = scratchValue29 & 0xfffffdff
                end
                if scratchValue29 & 256 ~= 0 then
                    scratchValue29 = scratchValue29 & 0xfffffeff
                end
                if predicateResult9 then
                    addNewConversation = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(addNewConversation, hero)
                    quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE", me, hero, false)
                    quest:ModifyThingHealth(me, 1000.0, false)
                else
                    if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                        predicateResult = true
                    else
                        scratchValue27 = scratchValue29 | 0xf000
                        predicateResult = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                    end
                    if scratchValue27 >> 8 < 0 then
                        scratchValue27 = scratchValue27 & 0xffff7fff
                    end
                    if scratchValue27 & 0x4000 ~= 0 then
                        scratchValue27 = scratchValue27 & 0xffffbfff
                    end
                    if scratchValue27 & 0x2000 ~= 0 then
                        scratchValue27 = scratchValue27 & 0xffffdfff
                    end
                    if scratchValue27 & 4096 ~= 0 then
                        scratchValue27 = scratchValue27 & 0xffffefff
                    end
                    if predicateResult then
                        getStateInt2 = state:GetInt("BeenHit") + 1
                        state:SetInt("BeenHit", getStateInt2)
                        if getStateInt2 == 7 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                            state:SetBool("NotBeaten", false)
                            quest:ModifyThingHealth(me, 1000.0, false)
                        end
                        quest:UpdateQuestInfoCounter(infoCounter, state:GetInt("BeenHit"), -1)
                        if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                            scratchValue30 = scratchValue29 & 0x80000001
                            scratchValue = scratchValue30 == 0
                            if scratchValue30 < 0 then
                                scratchValue = (scratchValue30 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if scratchValue then
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
                            else
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
                            end
                            quest:SetTimer(timerId, 5)
                        end
                    elseif me:MsgIsHitByHeroSpecialAbility(me) then
                        if not quest:IsConversationActive(addNewConversation) then
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING", me, hero, false)
                        end
                        quest:ModifyThingHealth(me, 1000.0, false)
                    end
                end
            end
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveQuestInfoElement(infoCounter)
                quest:DisplayQuestInfo(false)
                state:SetInt("BeenHit", 0)
                state:SetBool("NotBeaten", true)
                while not resources:TryAcquire(resource, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue33 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
                    quest:FixMovieSequenceCamera(true)
                    quest:Pause(0.5)
                    quest:EntitySetAsDrawable(hero, false)
                    quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(resource)) then
                        if not me:Speak(hero, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST", GROUP_SELECT_FIRST, false, true, false) then
                            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                            resources:DestroyMovie(scratchValue33)
                            goto LAB_00d664b0
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                            resources:DestroyMovie(scratchValue33)
                            goto LAB_00d664b0
                        end
                    end
                    quest:EntitySetAsDrawable(hero, true)
                    quest:FixMovieSequenceCamera(false)
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                    resources:DestroyMovie(scratchValue33)
                    quest:EntitySetBossPhase(me, 2)
                    infoCounter2 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
                    quest:DisplayQuestInfo(true)
                    quest:UpdateQuestInfoCounter(infoCounter2, state:GetInt("BeenHit"), -1)
                    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
                    while state:GetBool("NotBeaten") do
                        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                        scratchValue31 = scratchValue27 | 0x30000
                        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
                            predicateResult15 = true
                            if quest:IsConversationActive(addNewConversation) then
                                predicateResult15 = false
                                goto FLOW_after_lab_00d65d66
                            end
                        else
                            scratchValue31 = scratchValue27 | 0xf0000
                            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then
                                predicateResult15 = true
                                if not quest:IsConversationActive(addNewConversation) then
                                    goto FLOW_after_lab_00d65d66
                                end
                            end
                            predicateResult15 = false
                        end
                        ::FLOW_after_lab_00d65d66::
                        if scratchValue31 & 0x80000 ~= 0 then
                            scratchValue31 = scratchValue31 & 0xfff7ffff
                        end
                        if scratchValue31 & 0x40000 ~= 0 then
                            scratchValue31 = scratchValue31 & 0xfffbffff
                        end
                        if scratchValue31 & 0x20000 ~= 0 then
                            scratchValue31 = scratchValue31 & 0xfffdffff
                        end
                        if scratchValue31 & 0x10000 ~= 0 then
                            scratchValue31 = scratchValue31 & 0xfffeffff
                        end
                        if predicateResult15 then
                            addNewConversation = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(addNewConversation, hero)
                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE", me, hero, false)
                            quest:ModifyThingHealth(me, 1000.0, false)
                        else
                            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                                predicateResult16 = true
                                if quest:IsConversationActive(addNewConversation) then
                                    predicateResult16 = false
                                    goto FLOW_after_lab_00d65f0c
                                end
                            else
                                scratchValue27 = scratchValue31 | 0xf00000
                                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then
                                    predicateResult16 = true
                                    if not quest:IsConversationActive(addNewConversation) then
                                        goto FLOW_after_lab_00d65f0c
                                    end
                                end
                                predicateResult16 = false
                            end
                            ::FLOW_after_lab_00d65f0c::
                            if scratchValue27 & 0x800000 ~= 0 then
                                scratchValue27 = scratchValue27 & 0xff7fffff
                            end
                            if scratchValue27 & 0x400000 ~= 0 then
                                scratchValue27 = scratchValue27 & 0xffbfffff
                            end
                            if scratchValue27 & 0x200000 ~= 0 then
                                scratchValue27 = scratchValue27 & 0xffdfffff
                            end
                            if scratchValue27 & 0x100000 ~= 0 then
                                scratchValue27 = scratchValue27 & 0xffefffff
                            end
                            if predicateResult16 then
                                addNewConversation = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(addNewConversation, hero)
                                quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW", me, hero, false)
                                quest:ModifyThingHealth(me, 1000.0, false)
                            else
                                if me:MsgIsHitByHeroSpecialAbility(me) then
                                    getStateInt3 = state:GetInt("BeenHit") + 1
                                    state:SetInt("BeenHit", getStateInt3)
                                    if getStateInt3 == 7 then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                                        state:SetBool("NotBeaten", false)
                                        quest:ModifyThingHealth(me, 1000.0, false)
                                    end
                                    quest:UpdateQuestInfoCounter(infoCounter2, state:GetInt("BeenHit"), -1)
                                    if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(addNewConversation) then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                                        scratchValue32 = scratchValue31 & 0x80000001
                                        scratchValue19 = scratchValue32 == 0
                                        if scratchValue32 < 0 then
                                            scratchValue19 = (scratchValue32 - 1 | 0xfffffffe) == 0xffffffff
                                        end
                                        if scratchValue19 then
                                            addNewConversation = quest:AddNewConversation(me, false, false)
                                            quest:AddPersonToConversation(addNewConversation, hero)
                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, hero, false)
                                        else
                                            addNewConversation = quest:AddNewConversation(me, false, false)
                                            quest:AddPersonToConversation(addNewConversation, hero)
                                            quest:AddLineToConversation(addNewConversation, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, hero, false)
                                        end
                                        quest:SetTimer(timerId, 5)
                                    end
                            end
                            end
                        end
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:RemoveQuestInfoElement(infoCounter2)
                        quest:DisplayQuestInfo(false)
                        while not resources:TryAcquire(resource, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            me:ClearCommands()
                            quest:EntitySetInFaction(me, "FACTION_HERO")
                            scratchValue33 = resources:NewResource()
                            while not resources:TryAcquire(scratchValue33, hero, 4) do
                                if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue33); goto FLOW_after_lab_00d6632b end
                            end
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue33)
                            else
                                actorMap = resources:NewActorMap()
                                resources:SetActor(actorMap, "HERO", scratchValue33)
                                resources:SetActor(actorMap, "MAZE", resource)
                                movie2 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_WIN", actorMap, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                resources:DestroyActorMap(actorMap)
                                resources:ReleaseResource(scratchValue33)
                                quest:SetStateBool("MissionSucceeded", true)
                                quest:ModifyThingHealth(me, 1000.0, false)
                                quest:RemoveThing(me, false, true)
                                quest:EntitySetAsDamageable(hero, true)
                            end
                            ::FLOW_after_lab_00d6632b::
                        end
                    end
                end
            end
        end
    end
    ::LAB_00d664b0::
    quest:DeregisterTimer(timerId)
    ::LAB_00d664b9::
    resources:ReleaseResource(resource)
end

-- FinalMaze.Init (retail 0x00d62320)
function Init(quest, me)
end

-- FinalMaze.OnPersist (retail 0x00cdebc0)
function OnPersist(quest, context)
end

-- FinalMaze.OnPredicateFail (retail 0x00d62330)
function OnPredicateFail(quest, me)
end

