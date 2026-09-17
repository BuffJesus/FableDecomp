-- Readable native conversion: FinalMaze. Review coverage report before use.
-- Registration remains disabled until the package is verified.

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
    local resources = quest:RetailResources()
    local predicateResult3, scratchValue5, predicateResult4, predicateResult9, predicateResult10
    local scratchValue12, predicateResult15, predicateResult16, scratchValue19, scratchValue24
    local getStateInt, scratchValue25, getStateInt2, scratchValue26, getStateInt3, scratchValue30
    local scratchValue34, scratchValue35, scratchValue36, scratchValue37, scratchValue38
    local scratchValue39, scratchValue40, scratchValue41, scratchValue42, scratchValue43
    local scratchValue44, scratchValue45, timerId
    scratchValue34 = 0
    if not quest:NewScriptFrame(me) then return end
    scratchValue45 = resources:NewResource()
    while not resources:TryAcquire(scratchValue45, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b9 end
    end
    state:SetBool("NotFighting", true)
    state:SetBool("NotBeaten", true)
    state:SetInt("BeenHit", 0)
    quest:EntitySetAsKillable(me, false, true)
    quest:EntitySetInFaction(me, "FACTION_HERO")
    quest:EntitySetAllowBossPhaseChanges(me, false)
    scratchValue43 = resources:NewResource()
    while not resources:TryAcquire(scratchValue43, quest:GetHero(), 4) do
        quest:NewScriptFrame(me)
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(scratchValue43)
            resources:ReleaseResource(scratchValue45)
            return
        end
    end
    if quest:IsActiveThreadTerminating() then
        resources:ReleaseResource(scratchValue43)
        resources:ReleaseResource(scratchValue45)
        return
    end
    scratchValue42 = resources:NewActorMap()
    resources:SetActor(scratchValue42, "HERO", scratchValue43)
    resources:SetActor(scratchValue42, "MAZE", scratchValue45)
    scratchValue40 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_START", scratchValue42, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(scratchValue40)
    resources:DestroyActorMap(scratchValue42)
    resources:ReleaseResource(scratchValue43)
    quest:EntitySetInFaction(me, "FACTION_MONSTERS")
    scratchValue44 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
    quest:DisplayQuestInfo(true)
    quest:EntitySetBossPhase(me, 0)
    quest:EntitySetAsDamageable(quest:GetHero(), false)
    quest:UpdateQuestInfoCounter(scratchValue44, state:GetInt("BeenHit"), -1)
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 0)
    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
    quest:CacheMusicSet(47)
    scratchValue24 = 0
    while state:GetBool("NotBeaten") do
        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        scratchValue35 = scratchValue34 | 3
        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
            predicateResult3 = true
        else
            scratchValue35 = scratchValue34 | 15
            predicateResult3 = me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA")
        end
        if scratchValue35 & 8 ~= 0 then
            scratchValue35 = scratchValue35 & 0xfffffff7
        end
        if scratchValue35 & 4 ~= 0 then
            scratchValue35 = scratchValue35 & 0xfffffffb
        end
        if scratchValue35 & 2 ~= 0 then
            scratchValue35 = scratchValue35 & 0xfffffffd
        end
        if scratchValue35 & 1 ~= 0 then
            scratchValue35 = scratchValue35 & 0xfffffffe
        end
        if predicateResult3 then
            getStateInt = state:GetInt("BeenHit") + 1
            state:SetInt("BeenHit", getStateInt)
            if getStateInt == 7 then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                state:SetBool("NotBeaten", false)
                quest:ModifyThingHealth(me, 1000.0, false)
            end
            quest:UpdateQuestInfoCounter(scratchValue44, state:GetInt("BeenHit"), -1)
            if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(scratchValue24) then
                if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                scratchValue34 = scratchValue34 & 0x80000001
                scratchValue5 = scratchValue34 == 0
                if scratchValue34 < 0 then
                    scratchValue5 = (scratchValue34 - 1 | 0xfffffffe) == 0xffffffff
                end
                if scratchValue5 then
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, quest:GetHero(), false)
                else
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, quest:GetHero(), false)
                end
                quest:SetTimer(timerId, 5)
            end
        else
            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                predicateResult4 = true
                if quest:IsConversationActive(scratchValue24) then
                    predicateResult4 = false
                    goto FLOW_after_lab_00d64f56
                end
            else
                scratchValue34 = scratchValue35 | 240
                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then
                    predicateResult4 = true
                    if not quest:IsConversationActive(scratchValue24) then
                        goto FLOW_after_lab_00d64f56
                    end
                end
                predicateResult4 = false
            end
            ::FLOW_after_lab_00d64f56::
            if scratchValue34 < 0 then
                scratchValue34 = scratchValue34 & 0xffffff7f
            end
            if scratchValue34 & 64 ~= 0 then
                scratchValue34 = scratchValue34 & 0xffffffbf
            end
            if scratchValue34 & 32 ~= 0 then
                scratchValue34 = scratchValue34 & 0xffffffdf
            end
            if scratchValue34 & 16 ~= 0 then
                scratchValue34 = scratchValue34 & 0xffffffef
            end
            if predicateResult4 then
                scratchValue24 = quest:AddNewConversation(me, false, false)
                quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_BOW", me, quest:GetHero(), false)
                quest:ModifyThingHealth(me, 1000.0, false)
            elseif me:MsgIsHitByHeroSpecialAbility(me) then
                if not quest:IsConversationActive(scratchValue24) then
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_MELEE_LIGHTNING", me, quest:GetHero(), false)
                end
                quest:ModifyThingHealth(me, 1000.0, false)
            end
        end
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveQuestInfoElement(scratchValue44)
        quest:DisplayQuestInfo(false)
        state:SetInt("BeenHit", 0)
        state:SetBool("NotBeaten", true)
        while not resources:TryAcquire(scratchValue45, me, 4) do
            if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
        end
        if not quest:IsActiveThreadTerminating() then
            scratchValue43 = resources:StartMovie("")
            quest:StartMovieSequence()
            -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
            quest:FixMovieSequenceCamera(true)
            quest:Pause(0.5)
            quest:EntitySetAsDrawable(quest:GetHero(), false)
            quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
            if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue45)) then
                me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SKILL_FIRST", 0, false, true, false)
                while me:IsPerformingScriptTask() do
                    quest:NewScriptFrame(me)
                    if quest:IsActiveThreadTerminating() then
                        -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                        resources:DestroyMovie(scratchValue43)
                        goto LAB_00d664b0
                    end
                end
                if quest:IsActiveThreadTerminating() then
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                    resources:DestroyMovie(scratchValue43)
                    goto LAB_00d664b0
                end
            end
            quest:EntitySetAsDrawable(quest:GetHero(), true)
            quest:FixMovieSequenceCamera(false)
            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
            resources:DestroyMovie(scratchValue43)
            quest:EntitySetBossPhase(me, 1)
            scratchValue25 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
            scratchValue44 = scratchValue25
            quest:DisplayQuestInfo(true)
            quest:UpdateQuestInfoCounter(scratchValue25, state:GetInt("BeenHit"), -1)
            quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
            while state:GetBool("NotBeaten") do
                if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                scratchValue36 = scratchValue34 | 768
                if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
                    predicateResult9 = true
                    if quest:IsConversationActive(scratchValue24) then
                        predicateResult9 = false
                        goto FLOW_after_lab_00d654e6
                    end
                else
                    scratchValue36 = scratchValue34 | 3840
                    if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then
                        predicateResult9 = true
                        if not quest:IsConversationActive(scratchValue24) then
                            goto FLOW_after_lab_00d654e6
                        end
                    end
                    predicateResult9 = false
                end
                ::FLOW_after_lab_00d654e6::
                if scratchValue36 & 2048 ~= 0 then
                    scratchValue36 = scratchValue36 & 0xfffff7ff
                end
                if scratchValue36 & 1024 ~= 0 then
                    scratchValue36 = scratchValue36 & 0xfffffbff
                end
                if scratchValue36 & 512 ~= 0 then
                    scratchValue36 = scratchValue36 & 0xfffffdff
                end
                if scratchValue36 & 256 ~= 0 then
                    scratchValue36 = scratchValue36 & 0xfffffeff
                end
                if predicateResult9 then
                    scratchValue24 = quest:AddNewConversation(me, false, false)
                    quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                    quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_MELEE", me, quest:GetHero(), false)
                    quest:ModifyThingHealth(me, 1000.0, false)
                else
                    if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                        predicateResult10 = true
                    else
                        scratchValue34 = scratchValue36 | 0xf000
                        predicateResult10 = me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW")
                    end
                    if scratchValue34 >> 8 < 0 then
                        scratchValue34 = scratchValue34 & 0xffff7fff
                    end
                    if scratchValue34 & 0x4000 ~= 0 then
                        scratchValue34 = scratchValue34 & 0xffffbfff
                    end
                    if scratchValue34 & 0x2000 ~= 0 then
                        scratchValue34 = scratchValue34 & 0xffffdfff
                    end
                    if scratchValue34 & 4096 ~= 0 then
                        scratchValue34 = scratchValue34 & 0xffffefff
                    end
                    if predicateResult10 then
                        getStateInt2 = state:GetInt("BeenHit") + 1
                        state:SetInt("BeenHit", getStateInt2)
                        if getStateInt2 == 7 then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                            state:SetBool("NotBeaten", false)
                            quest:ModifyThingHealth(me, 1000.0, false)
                        end
                        quest:UpdateQuestInfoCounter(scratchValue44, state:GetInt("BeenHit"), -1)
                        if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(scratchValue24) then
                            if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                            scratchValue37 = scratchValue36 & 0x80000001
                            scratchValue12 = scratchValue37 == 0
                            if scratchValue37 < 0 then
                                scratchValue12 = (scratchValue37 - 1 | 0xfffffffe) == 0xffffffff
                            end
                            if scratchValue12 then
                                scratchValue24 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                                quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, quest:GetHero(), false)
                            else
                                scratchValue24 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                                quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, quest:GetHero(), false)
                            end
                            quest:SetTimer(timerId, 5)
                        end
                    elseif me:MsgIsHitByHeroSpecialAbility(me) then
                        if not quest:IsConversationActive(scratchValue24) then
                            scratchValue24 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                            quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_BOW_LIGHTNING", me, quest:GetHero(), false)
                        end
                        quest:ModifyThingHealth(me, 1000.0, false)
                    end
                end
                scratchValue25 = scratchValue44
            end
            if not quest:IsActiveThreadTerminating() then
                quest:RemoveQuestInfoElement(scratchValue25)
                quest:DisplayQuestInfo(false)
                state:SetInt("BeenHit", 0)
                state:SetBool("NotBeaten", true)
                while not resources:TryAcquire(scratchValue45, me, 4) do
                    if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                end
                if not quest:IsActiveThreadTerminating() then
                    scratchValue43 = resources:StartMovie("")
                    quest:StartMovieSequence()
                    -- TODO(native): xStack_7c = *(CCharString *)(this + 4);
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,true);
                    quest:FixMovieSequenceCamera(true)
                    quest:Pause(0.5)
                    quest:EntitySetAsDrawable(quest:GetHero(), false)
                    quest:CameraUseCameraPoint(quest:GetThingWithScriptName("CAM_RC_MAZE"), me, -1.0, 0, -1)
                    if 0.0 < quest:GetHealth(resources:ScriptThing(scratchValue45)) then
                        me:Speak(quest:GetHero(), "TEXT_QST_028_MAZE_WOODS_DEPARTURE_LIGHTNING_FIRST", 0, false, true, false)
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                                resources:DestroyMovie(scratchValue43)
                                goto LAB_00d664b0
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                            resources:DestroyMovie(scratchValue43)
                            goto LAB_00d664b0
                        end
                    end
                    quest:EntitySetAsDrawable(quest:GetHero(), true)
                    quest:FixMovieSequenceCamera(false)
                    -- TODO(native): (**(code **)(*(int *)xStack_7c + 0x5ec))((void *)xStack_7c,false);
                    resources:DestroyMovie(scratchValue43)
                    quest:EntitySetBossPhase(me, 2)
                    scratchValue26 = quest:AddQuestInfoCounter("HUD_QUEST_ICON_MAZE", 7, 1.0)
                    scratchValue44 = scratchValue26
                    quest:DisplayQuestInfo(true)
                    quest:UpdateQuestInfoCounter(scratchValue26, state:GetInt("BeenHit"), -1)
                    quest:EntityWillTeleportToArea(me, me:GetPos().x, 5.0, 20.0)
                    while state:GetBool("NotBeaten") do
                        if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                        scratchValue38 = scratchValue34 | 0x30000
                        if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_LONGSWORD") then
                            predicateResult15 = true
                            if quest:IsConversationActive(scratchValue24) then
                                predicateResult15 = false
                                goto FLOW_after_lab_00d65d66
                            end
                        else
                            scratchValue38 = scratchValue34 | 0xf0000
                            if me:MsgIsHitByHeroWithWeapon("OBJECT_IRON_KATANA") then
                                predicateResult15 = true
                                if not quest:IsConversationActive(scratchValue24) then
                                    goto FLOW_after_lab_00d65d66
                                end
                            end
                            predicateResult15 = false
                        end
                        ::FLOW_after_lab_00d65d66::
                        if scratchValue38 & 0x80000 ~= 0 then
                            scratchValue38 = scratchValue38 & 0xfff7ffff
                        end
                        if scratchValue38 & 0x40000 ~= 0 then
                            scratchValue38 = scratchValue38 & 0xfffbffff
                        end
                        if scratchValue38 & 0x20000 ~= 0 then
                            scratchValue38 = scratchValue38 & 0xfffdffff
                        end
                        if scratchValue38 & 0x10000 ~= 0 then
                            scratchValue38 = scratchValue38 & 0xfffeffff
                        end
                        if predicateResult15 then
                            scratchValue24 = quest:AddNewConversation(me, false, false)
                            quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                            quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_MELEE", me, quest:GetHero(), false)
                            quest:ModifyThingHealth(me, 1000.0, false)
                        else
                            if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_LONGBOW") then
                                predicateResult16 = true
                                if quest:IsConversationActive(scratchValue24) then
                                    predicateResult16 = false
                                    goto FLOW_after_lab_00d65f0c
                                end
                            else
                                scratchValue34 = scratchValue38 | 0xf00000
                                if me:MsgIsHitByHeroWithWeapon("OBJECT_YEW_CROSSBOW") then
                                    predicateResult16 = true
                                    if not quest:IsConversationActive(scratchValue24) then
                                        goto FLOW_after_lab_00d65f0c
                                    end
                                end
                                predicateResult16 = false
                            end
                            ::FLOW_after_lab_00d65f0c::
                            if scratchValue34 & 0x800000 ~= 0 then
                                scratchValue34 = scratchValue34 & 0xff7fffff
                            end
                            if scratchValue34 & 0x400000 ~= 0 then
                                scratchValue34 = scratchValue34 & 0xffbfffff
                            end
                            if scratchValue34 & 0x200000 ~= 0 then
                                scratchValue34 = scratchValue34 & 0xffdfffff
                            end
                            if scratchValue34 & 0x100000 ~= 0 then
                                scratchValue34 = scratchValue34 & 0xffefffff
                            end
                            if predicateResult16 then
                                scratchValue24 = quest:AddNewConversation(me, false, false)
                                quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                                quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_BAD_LIGHTNING_BOW", me, quest:GetHero(), false)
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
                                    quest:UpdateQuestInfoCounter(scratchValue44, state:GetInt("BeenHit"), -1)
                                    if quest:GetTimer(timerId) < 1 and not quest:IsConversationActive(scratchValue24) then
                                        if quest:IsActiveThreadTerminating() then goto LAB_00d664b0 end
                                        scratchValue39 = scratchValue38 & 0x80000001
                                        scratchValue19 = scratchValue39 == 0
                                        if scratchValue39 < 0 then
                                            scratchValue19 = (scratchValue39 - 1 | 0xfffffffe) == 0xffffffff
                                        end
                                        if scratchValue19 then
                                            scratchValue24 = quest:AddNewConversation(me, false, false)
                                            quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                                            quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_ON_HIT", me, quest:GetHero(), false)
                                        else
                                            scratchValue24 = quest:AddNewConversation(me, false, false)
                                            quest:AddPersonToConversation(scratchValue24, quest:GetHero())
                                            quest:AddLineToConversation(scratchValue24, "TEXT_QST_028_MAZE_WOODS_DEPARTURE_SPARRING", me, quest:GetHero(), false)
                                        end
                                        quest:SetTimer(timerId, 5)
                                    end
                            end
                            end
                        end
                        scratchValue26 = scratchValue44
                    end
                    if not quest:IsActiveThreadTerminating() then
                        quest:RemoveQuestInfoElement(scratchValue26)
                        quest:DisplayQuestInfo(false)
                        while not resources:TryAcquire(scratchValue45, me, 4) do
                            if not quest:NewScriptFrame(me) then goto LAB_00d664b0 end
                        end
                        if not quest:IsActiveThreadTerminating() then
                            me:ClearCommands()
                            quest:EntitySetInFaction(me, "FACTION_HERO")
                            scratchValue43 = resources:NewResource()
                            while not resources:TryAcquire(scratchValue43, quest:GetHero(), 4) do
                                if not quest:NewScriptFrame(me) then resources:ReleaseResource(scratchValue43); goto FLOW_after_lab_00d6632b end
                            end
                            if quest:IsActiveThreadTerminating() then
                                resources:ReleaseResource(scratchValue43)
                            else
                                scratchValue30 = resources:NewActorMap()
                                resources:SetActor(scratchValue30, "HERO", scratchValue43)
                                resources:SetActor(scratchValue30, "MAZE", scratchValue45)
                                scratchValue41 = resources:StartMovie("")
                                quest:StartMovieSequence()
                                quest:PauseAllNonScriptedEntities(true)
                                quest:FixMovieSequenceCamera(true)
                                resources:RunMacro("CS_GUILD_DEPARTURE_MAZE_WIN", scratchValue30, false, true)
                                quest:FixMovieSequenceCamera(false)
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(scratchValue41)
                                resources:DestroyActorMap(scratchValue30)
                                resources:ReleaseResource(scratchValue43)
                                quest:SetStateBool("MissionSucceeded", true)
                                quest:ModifyThingHealth(me, 1000.0, false)
                                quest:RemoveThing(me, false, true)
                                quest:EntitySetAsDamageable(quest:GetHero(), true)
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
    resources:ReleaseResource(scratchValue45)
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

