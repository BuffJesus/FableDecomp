-- Readable native conversion: Magicman. Review coverage report before use.
-- Registration remains disabled until the package is verified.

local ENTITY_MOVE_WALK = 0  -- EScriptEntityMoveType (Ego_r.pdb)

local GROUP_SELECT_FIRST = 0  -- ETextGroupSelectionMethod

local HERO_ABILITY_HEAL_LIFE_SPELL = 14  -- EHeroAbility (Ego_r.pdb)

local helpers = require("V_Bordello.native_quest_helpers")

-- per-entity fields (native class members; one Lua state per entity instance)
local doneToutIntro_, mentionedPimpHat

-- Magicman.Main (retail 0x00e40e80)
function Main(quest, me)
    local resources = quest:RetailResources()
    local __push8, isActiveThreadTerminating, predicateResult, getStateBool, chatterCounter
    local timerId2, scratchValue21, timerId3, getHero, line, scratchValue23, getHomePos, timerId
    local scratchValue24, scratchValue25, startMovie, movie3, movie4, scratchValue37
    scratchValue25 = 0  -- compiler cleanup flags start clear
    scratchValue24 = 0  -- compiler cleanup flags start clear
    if not quest:NewScriptFrame(me) then return end
    local resource = resources:NewResource()
    resources:PrepareResource(resource)
    while not resources:TryAcquire(resource, me, 4) do
        if not quest:NewScriptFrame(me) then goto LAB_00e4451f end
    end
    if quest:IsActiveThreadTerminating() then goto LAB_00e4451f end
    resources:AssignResource(resources:MemberResource("seh_Boss"), resource)
    timerId3 = quest:RegisterTimer()
    timerId = quest:RegisterTimer()
    quest:SetTimer(timerId, 40)
    chatterCounter = 1
    scratchValue21 = 1
    getStateBool = quest:GetStateBool("PlayerOwned")
    while not getStateBool do
        if not quest:NewScriptFrame(me) then goto LAB_00e4450d end
        getStateBool = quest:GetStateBool("HeroTricking")
        while not getStateBool and not quest:GetStateBool("PlayerOwned") do
            if not quest:NewScriptFrame(me) then goto LAB_00e448a1 end
            getHomePos = me:GetHomePos()
            if (me ~= nil and me:IsDistanceFromPositionOver(getHomePos, 2.0)) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                timerId2 = me:GetHomePos()
                me:MoveToPosition(timerId2, 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if not (quest:GetStateBool("BeerSetToDPad")) then
                scratchValue25 = scratchValue25 | 1
                timerId2 = quest:GetHeroTargetedThing()
                isActiveThreadTerminating = true
                if not (timerId2 ~= nil and timerId2:IsEqualTo(me)) then goto LAB_00e410ba end
                goto FLOW_past_lab_00e410ba
            end
            ::LAB_00e410ba::
            isActiveThreadTerminating = false
            ::FLOW_past_lab_00e410ba::
            if (scratchValue25 & 1) ~= 0 then
                scratchValue25 = scratchValue25 & 0xfffffffe
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                quest:SetStateBool("BeerSetToDPad", true)
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, 0xf4240)
                quest:CreateThread("WatchForHeroLeavingRegionWithBeer")  -- native thread body 0x00E44980: lift it as function WatchForHeroLeavingRegionWithBeer(quest)
                if (scratchValue25 & 8) ~= 0 then
                    scratchValue25 = scratchValue25 & 0xfffffff7
                end
                if (scratchValue25 & 4) ~= 0 then
                    scratchValue25 = scratchValue25 & 0xfffffffb
                end
                if (scratchValue25 & 2) ~= 0 then
                    scratchValue25 = scratchValue25 & 0xfffffffd
                end
            else
                if not quest:GetStateBool("BeerSetToDPad") then
                    goto LAB_00e412bc
                else
                    scratchValue25 = scratchValue25 | 16
                    timerId2 = quest:GetHeroTargetedThing()
                    isActiveThreadTerminating = true
                    if timerId2 ~= nil and timerId2:IsEqualTo(me) then goto LAB_00e412bc end
                end
                goto FLOW_past_lab_00e412bc
                ::LAB_00e412bc::
                isActiveThreadTerminating = false
                ::FLOW_past_lab_00e412bc::
                if (scratchValue25 & 16) ~= 0 then
                    scratchValue25 = scratchValue25 & 0xffffffef
                end
                if isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                    quest:SetStateBool("BeerSetToDPad", false)
                end
            end
            if quest:GetTimer(timerId3) == 0 then
                getHero = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(me, getHero, 8.0) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId3)
                        goto LAB_00e4451f
                    end
                    quest:SetTimer(timerId3, 4)
                    local __push1 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push1, false)
                end
            end
            if quest:GetTimer(timerId) == 0 then
                getHero = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(me, getHero, 15.0) then
                    if quest:IsActiveThreadTerminating() then
                        quest:DeregisterTimer(timerId)
                        quest:DeregisterTimer(timerId3)
                        goto LAB_00e4451f
                    end
                    local __push2 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(scratchValue21)
                    if not quest:TextEntryExists(__push2) then
                        if quest:IsActiveThreadTerminating() then
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId3)
                            goto LAB_00e4451f
                        end
                        scratchValue21 = 1
                    end
                    local __push3 = quest:AddNewConversation(me, false, false)
                    local __push4 = quest:GetHero()
                    quest:AddPersonToConversation(__push3, __push4)
                    scratchValue23 = scratchValue21
                    local __push5 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(scratchValue21)
                    quest:AddLineToConversation(__push3, __push5, me, __push4, false)
                    scratchValue21 = scratchValue23 + 1
                    quest:SetTimer(timerId, 30)
                end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                me:ClearCommands()
                movie4 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local hero4 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero4, false)
                quest:Pause(1.0)
                local hero = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_B_OWNER", me, -1.0, 0, -1)
                if helpers.IsHeroWearingBeard(quest, me) then
                    if not quest:IsActiveThreadTerminating() then
                        local __push6 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(__push6) then
                            line = "TEXT_QST_B13_MAGICMAN_PIMPING"
                            timerId2 = quest:GetHero()
                            if not me:Speak(timerId2, line, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                quest:DeregisterTimer(timerId)
                                quest:DeregisterTimer(timerId3)
                                resources:ReleaseResource(resource)
                                return
                            end
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    quest:DeregisterTimer(timerId)
                                    quest:DeregisterTimer(timerId3)
                                    resources:ReleaseResource(resource)
                                    do return end
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00e448a1
                            end
                        end
                        goto LAB_00e41b75
                    end
                    goto LAB_00e445a0
                end
                if not doneToutIntro_ then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e445a0 end
                    doneToutIntro_ = true
                    local __push7 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(__push7) then
                        line = "TEXT_QST_B13_MAGICMAN_TOUTING"
                        timerId2 = quest:GetHero()
                        if not me:Speak(timerId2, line, 0, false, true, false) then
                            goto LAB_00e44615
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e44615 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e445a0 end
                    end
                else
                    goto FLOW_past_lab_00e41b75
                end
                ::LAB_00e41b75::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00e41ba8
                ::FLOW_past_lab_00e41b75::
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00e448a1
                end
                if not (mentionedPimpHat) then
                    __push8 = quest:GetHero()
                    predicateResult = true
                    if not quest:IsWearingClothingItem(__push8, "OBJECT_HERO_HAT_PIMP") then goto LAB_00e418e3 end
                    goto FLOW_past_lab_00e418e3
                end
                ::LAB_00e418e3::
                predicateResult = false
                ::FLOW_past_lab_00e418e3::
                if not predicateResult then
                    if not quest:IsActiveThreadTerminating() then
                        if 4 < chatterCounter then
                            chatterCounter = 1
                        end
                        startMovie = "TEXT_QST_B13_MAGICMAN_CHATTER_0" .. tostring(chatterCounter)
                        local __push9 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(__push9) then
                            getHomePos = startMovie
                            timerId2 = quest:GetHero()
                            if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00e44862
                            end
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie4)
                                    goto LAB_00e44862
                                end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie4)
                                goto LAB_00e448a1
                            end
                        end
                        chatterCounter = chatterCounter + 1
                        goto LAB_00e41b75
                    end
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie4)
                    goto LAB_00e448a1
                end
                if not quest:IsActiveThreadTerminating() then
                    local __push10 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(__push10) then
                        line = "TEXT_QST_B13_MAGICMAN_PIMP_HAT"
                        timerId2 = quest:GetHero()
                        if not me:Speak(timerId2, line, 0, false, true, false) then
                            goto LAB_00e44615
                        end
                        while me:IsPerformingScriptTask() do
                            if not quest:NewScriptFrame(me) then goto LAB_00e44615 end
                        end
                        if quest:IsActiveThreadTerminating() then goto LAB_00e445a0 end
                    end
                    mentionedPimpHat = true
                    goto LAB_00e41b75
                end
                ::LAB_00e445a0::
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie4)
                goto LAB_00e44862
            end
            ::LAB_00e41ba8::
            scratchValue24 = scratchValue25
            scratchValue25 = scratchValue25 | 64
            if not (me:MsgIsHitByHero()) then
                scratchValue25 = scratchValue24 | 192
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue25 = scratchValue24 | 448
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e41c4b end
                end
                isActiveThreadTerminating = false
                goto FLOW_past_lab_00e41c4b
            end
            ::LAB_00e41c4b::
            isActiveThreadTerminating = true
            ::FLOW_past_lab_00e41c4b::
            if (scratchValue25 & 256) ~= 0 then
                scratchValue25 = scratchValue25 & 0xfffffeff
            end
            if (scratchValue25 & 128) ~= 0 then
                scratchValue25 = scratchValue25 & 0xffffff7f
            end
            if (scratchValue25 & 64) ~= 0 then
                scratchValue25 = scratchValue25 & 0xffffffbf
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                me:ClearCommands()
                local movie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local hero5 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero5, false)
                quest:Pause(1.0)
                local hero2 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero2, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_B_OWNER", me, -1.0, 0, -1)
                if helpers.IsHeroWearingBeard(quest, me) then
                    if not quest:IsActiveThreadTerminating() then
                        local __push11 = resources:ScriptThing(resource)
                        if (0.0 < quest:GetHealth(__push11)) then
                            goto FLOW_past_lab_00e42003
                        end
                        ::LAB_00e42003::
                        quest:FixMovieSequenceCamera(false)
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e4203f
                        ::FLOW_past_lab_00e42003::
                        line = "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE"
                        timerId2 = quest:GetHero()
                        if not me:Speak(timerId2, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId3)
                            resources:ReleaseResource(resource)
                            return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                quest:DeregisterTimer(timerId)
                                quest:DeregisterTimer(timerId3)
                                resources:ReleaseResource(resource)
                                do return end
                            end
                        end
                        if not quest:IsActiveThreadTerminating() then goto LAB_00e42003 end
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie)
                        goto LAB_00e448a1
                    end
                elseif not quest:IsActiveThreadTerminating() then
                    local scratchValue28 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue28) then
                        line = "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE"
                        local hero3 = quest:GetHero()
                        if not me:Speak(hero3, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            quest:DeregisterTimer(timerId)
                            quest:DeregisterTimer(timerId3)
                            resources:ReleaseResource(resource)
                            return
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie)
                                quest:DeregisterTimer(timerId)
                                quest:DeregisterTimer(timerId3)
                                resources:ReleaseResource(resource)
                                do return end
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie)
                            goto LAB_00e448a1
                        end
                    end
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(movie)
                    goto LAB_00e4203f
                end
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie)
                goto LAB_00e44862
            end
            ::LAB_00e4203f::
            g_PresentedItemName = nil
            if not me:MsgIsPresentedWithItem() then
                getStateBool = quest:GetStateBool("HeroTricking")
            elseif g_PresentedItemName ~= "OBJECT_BEER_TANKARD" then
                getStateBool = quest:GetStateBool("HeroTricking")
            else
                if quest:IsActiveThreadTerminating() then goto LAB_00e448a1 end
                me:ClearCommands()
                movie3 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local hero6 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero6, false)
                quest:Pause(1.0)
                local __push13 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push13, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_B_OWNER", me, -1.0, 0, -1)
                local switch1 = quest:GetStateInt("BeersDrunk")
                repeat
                    if switch1 == 0 then
                        getHero = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(getHero) then
                            getHomePos = "TEXT_QST_B13_MAGICMAN_BEER1" .. helpers.GetHeroStatusTextTag(quest, me)
                            timerId2 = quest:GetHero()
                            if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                goto LAB_00e44811
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e44811 end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e448a1
                            end
                        end
                        break
                    elseif switch1 == 1 then
                        local scratchValue27 = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(scratchValue27) then
                            getHomePos = "TEXT_QST_B13_MAGICMAN_BEER2" .. helpers.GetHeroStatusTextTag(quest, me)
                            timerId2 = quest:GetHero()
                            if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                goto LAB_00e44811
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e44811 end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e448a1
                            end
                        end
                        break
                    elseif switch1 == 2 then
                        getHero = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(getHero) then
                            getHomePos = "TEXT_QST_B13_MAGICMAN_BEER3" .. helpers.GetHeroStatusTextTag(quest, me)
                            timerId2 = quest:GetHero()
                            if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                goto LAB_00e44811
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e44811 end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e448a1
                            end
                        end
                        break
                    elseif switch1 == 3 then
                        getHero = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(getHero) then
                            getHomePos = "TEXT_QST_B13_MAGICMAN_BEER4" .. helpers.GetHeroStatusTextTag(quest, me)
                            timerId2 = quest:GetHero()
                            if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                goto LAB_00e44811
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e44811 end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e448a1
                            end
                        end
                        break
                    elseif switch1 == 4 then
                        getHero = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(getHero) then
                            getHomePos = "TEXT_QST_B13_MAGICMAN_BEER5" .. helpers.GetHeroStatusTextTag(quest, me)
                            timerId2 = quest:GetHero()
                            if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                goto LAB_00e44811
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e44811 end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e448a1
                            end
                        end
                        break
                    elseif switch1 == 5 then
                        getHero = resources:ScriptThing(resource)
                        if 0.0 < quest:GetHealth(getHero) then
                            getHomePos = "TEXT_QST_B13_MAGICMAN_BEER6" .. helpers.GetHeroStatusTextTag(quest, me)
                            timerId2 = quest:GetHero()
                            if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                goto LAB_00e44811
                            end
                            while me:IsPerformingScriptTask() do
                                if not quest:NewScriptFrame(me) then goto LAB_00e44811 end
                            end
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e448a1
                            end
                        end
                        if not quest:GetStateBool("HeroFoundDeedsLocation") then
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e448a1
                            end
                            helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANDRUNK", false)
                            quest:SetStateBool("HeroFoundDeedsLocation", true)
                            quest:SetStateBool("HeroPartying", true)
                        else
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e44862
                            end
                            helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANDRUNK_QUICKIE", false)
                            quest:SetStateBool("HeroPartying", true)
                        end
                        break
                    else
                        local scriptThing = resources:ScriptThing(resource)
                        if not (0.0 < quest:GetHealth(scriptThing)) then
                            quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)
                        else
                            line = "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH"
                            timerId2 = quest:GetHero()
                            me:Speak(timerId2, line, 0, false, true, false)
                            while me:IsPerformingScriptTask() do
                                quest:NewScriptFrame(me)
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(movie3)
                                    goto LAB_00e44862
                                end
                            end
                            if not quest:IsActiveThreadTerminating() then
                                quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)
                            else
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie3)
                                goto LAB_00e448a1
                            end
                        end
                    end
                until true
                quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie3)
                getStateBool = quest:GetStateBool("HeroTricking")
            end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
        while not quest:GetStateBool("PlayerOwned") do
            if not quest:NewScriptFrame(me) then goto LAB_00e4450d end
            getHomePos = me:GetHomePos()
            if (me ~= nil and me:IsDistanceFromPositionOver(getHomePos, 2.0)) and not me:IsPerformingScriptTask() then
                if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                timerId2 = me:GetHomePos()
                me:MoveToPosition(timerId2, 1.0, ENTITY_MOVE_WALK, false, true)
            end
            if not (quest:GetStateBool("BeerSetToDPad")) then
                scratchValue25 = scratchValue25 | 512
                timerId2 = quest:GetHeroTargetedThing()
                isActiveThreadTerminating = true
                if not (timerId2 ~= nil and timerId2:IsEqualTo(me)) then goto LAB_00e42a3a end
                goto FLOW_past_lab_00e42a3a
            end
            ::LAB_00e42a3a::
            isActiveThreadTerminating = false
            ::FLOW_past_lab_00e42a3a::
            if (scratchValue25 & 512) ~= 0 then
                scratchValue25 = scratchValue25 & 0xfffffdff
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                quest:SetStateBool("BeerSetToDPad", true)
                quest:SetPreferredQuickAccessItem("OBJECT_BEER_TANKARD", 2, 0xf4240)
                quest:CreateThread("WatchForHeroLeavingRegionWithBeer")  -- native thread body 0x00E44980: lift it as function WatchForHeroLeavingRegionWithBeer(quest)
                if (scratchValue25 & 4096) ~= 0 then
                    scratchValue25 = scratchValue25 & 0xffffefff
                end
                if (scratchValue25 & 2048) ~= 0 then
                    scratchValue25 = scratchValue25 & 0xfffff7ff
                end
                if (scratchValue25 & 1024) ~= 0 then
                    scratchValue25 = scratchValue25 & 0xfffffbff
                end
            else
                if not quest:GetStateBool("BeerSetToDPad") then
                    goto LAB_00e42bf9
                else
                    scratchValue25 = scratchValue25 | 0x2000
                    timerId2 = quest:GetHeroTargetedThing()
                    isActiveThreadTerminating = true
                    if timerId2 ~= nil and timerId2:IsEqualTo(me) then goto LAB_00e42bf9 end
                end
                goto FLOW_past_lab_00e42bf9
                ::LAB_00e42bf9::
                isActiveThreadTerminating = false
                ::FLOW_past_lab_00e42bf9::
                if (scratchValue25 & 0x2000) ~= 0 then
                    scratchValue25 = scratchValue25 & 0xffffdfff
                end
                if isActiveThreadTerminating then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    quest:SetStateBool("BeerSetToDPad", false)
                end
            end
            if quest:GetTimer(timerId3) == 0 then
                getHero = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(me, getHero, 8.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    quest:SetTimer(timerId3, 4)
                    local __push15 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push15, false)
                end
            end
            if quest:GetTimer(timerId) == 0 then
                getHero = quest:GetHero()
                if quest:IsDistanceBetweenThingsUnder(me, getHero, 15.0) then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    local __push16 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(scratchValue21)
                    if not quest:TextEntryExists(__push16) then
                        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                        scratchValue21 = 1
                    end
                    local __push17 = quest:AddNewConversation(me, false, false)
                    local __push18 = quest:GetHero()
                    quest:AddPersonToConversation(__push17, __push18)
                    scratchValue23 = scratchValue21
                    local __push19 = "TEXT_QST_B13_MAGICMAN_TALKING_OUT_LOUD_0" .. tostring(scratchValue21)
                    quest:AddLineToConversation(__push17, __push19, me, __push18, false)
                    scratchValue21 = scratchValue23 + 1
                    quest:SetTimer(timerId, 30)
                end
            end
            if me:IsTalkedToByHero() then
                if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                me:ClearCommands()
                local trickingMovie = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local hero7 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, hero7, false)
                quest:Pause(1.0)
                local __push20 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push20, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_B_OWNER", me, -1.0, 0, -1)
                if helpers.IsHeroWearingBeard(quest, me) then
                    if not quest:IsActiveThreadTerminating() then
                        if helpers.helper_E44A40(quest, me) then
                            if not quest:IsActiveThreadTerminating() then
                                local scratchValue34 = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scratchValue34) then
                                    line = "TEXT_QST_B13_MAGICMAN_BEARD_LADY"
                                    timerId2 = quest:GetHero()
                                    if not me:Speak(timerId2, line, 0, false, true, false) then
                                        goto LAB_00e448cc
                                    end
                                    while me:IsPerformingScriptTask() do
                                        if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                    end
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(trickingMovie)
                                        goto LAB_00e4450d
                                    end
                                end
                                goto LAB_00e437b3
                            end
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(trickingMovie)
                            goto LAB_00e4450d
                        end
                        if not quest:IsActiveThreadTerminating() then
                            if helpers.IsHeroWearingTash(quest, me) then
                                if not quest:IsActiveThreadTerminating() then
                                    local scratchValue35 = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(scratchValue35) then
                                        line = "TEXT_QST_B13_MAGICMAN_BEARD_LADY_MOUSTACHE"
                                        timerId2 = quest:GetHero()
                                        if not me:Speak(timerId2, line, 0, false, true, false) then
                                            goto LAB_00e448cc
                                        end
                                        while me:IsPerformingScriptTask() do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                        end
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(trickingMovie)
                                            goto LAB_00e4450d
                                        end
                                    end
                                    goto LAB_00e437b3
                                end
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(trickingMovie)
                                goto LAB_00e4450d
                            end
                            if not quest:IsActiveThreadTerminating() then
                                local scratchValue33 = resources:ScriptThing(resource)
                                if 0.0 < quest:GetHealth(scratchValue33) then
                                    if not me:Speak(me, "TEXT_QST_B13_MAGICMAN_LOVELY_SKIN", GROUP_SELECT_FIRST, false, true, false) then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(trickingMovie)
                                        goto LAB_00e4450d
                                    end
                                    while me:IsPerformingScriptTask() do
                                        quest:NewScriptFrame(me)
                                        if quest:IsActiveThreadTerminating() then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(trickingMovie)
                                            goto LAB_00e4450d
                                        end
                                    end
                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                end
                                local __push24 = quest:GetHero()
                                if quest:IsWearingClothingItem(__push24, "OBJECT_HERO_HAT_WHOREWIG") then
                                    if not quest:IsActiveThreadTerminating() then
                                        local scratchValue42 = resources:ScriptThing(resource)
                                        if 0.0 < quest:GetHealth(scratchValue42) then
                                            line = "TEXT_QST_B13_MAGICMAN_LOVELY_HAIR"
                                            timerId2 = quest:GetHero()
                                            if not me:Speak(timerId2, line, 0, false, true, false) then
                                                goto LAB_00e448cc
                                            end
                                            while me:IsPerformingScriptTask() do
                                                if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                            end
                                            if quest:IsActiveThreadTerminating() then
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyMovie(trickingMovie)
                                                goto LAB_00e4450d
                                            end
                                        end
                                        quest:GiveHeroYesNoQuestion("TEXT_QST_B13_MAGICMAN_PARTY", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "")
                                        timerId2 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        while timerId2 < 0 do
                                            if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                            timerId2 = quest:MsgIsQuestionAnsweredYesOrNo()
                                        end
                                        if not quest:IsActiveThreadTerminating() then
                                            isActiveThreadTerminating = quest:IsActiveThreadTerminating()
                                            if timerId2 == 1 then
                                                if isActiveThreadTerminating then goto LAB_00e448cc end
                                                local scratchValue41 = resources:ScriptThing(resource)
                                                if 0.0 < quest:GetHealth(scratchValue41) then
                                                    line = "TEXT_QST_B13_MAGICMAN_HORNY"
                                                    timerId2 = quest:GetHero()
                                                    if not me:Speak(timerId2, line, 0, false, true, false) then
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyMovie(trickingMovie)
                                                        goto LAB_00e4450d
                                                    end
                                                    while me:IsPerformingScriptTask() do
                                                        quest:NewScriptFrame(me)
                                                        if quest:IsActiveThreadTerminating() then
                                                            quest:PauseAllNonScriptedEntities(false)
                                                            resources:DestroyMovie(trickingMovie)
                                                            goto LAB_00e4450d
                                                        end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                                end
                                                quest:GiveHeroGold(1000)
                                                local __push27 = quest:GetNumberOfTimesHeroHasHadSex()
                                                quest:SetNumberOfTimesHeroHasHadSex(__push27)
                                                quest:SetCutsceneSkippable(false)
                                                if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                                    if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANSEX", true)
                                                    quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                else
                                                    if quest:IsActiveThreadTerminating() then
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyMovie(trickingMovie)
                                                        goto LAB_00e4450d
                                                    end
                                                    helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANSEX_QUICKIE", true)
                                                end
                                                quest:SetCutsceneSkippable(true)
                                                quest:SetStateBool("HeroPartying", true)
                                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                                                quest:SetHeroAsHavingHadGaySex(true)
                                            else
                                                if isActiveThreadTerminating then
                                                    quest:PauseAllNonScriptedEntities(false)
                                                    resources:DestroyMovie(trickingMovie)
                                                    goto LAB_00e4450d
                                                end
                                                local scratchValue39 = resources:ScriptThing(resource)
                                                if 0.0 < quest:GetHealth(scratchValue39) then
                                                    line = "TEXT_QST_B13_MAGICMAN_REJECTED"
                                                    timerId2 = quest:GetHero()
                                                    if not me:Speak(timerId2, line, 0, false, true, false) then
                                                        goto LAB_00e448cc
                                                    end
                                                    while me:IsPerformingScriptTask() do
                                                        if not quest:NewScriptFrame(me) then goto LAB_00e448cc end
                                                    end
                                                    if quest:IsActiveThreadTerminating() then
                                                        quest:PauseAllNonScriptedEntities(false)
                                                        resources:DestroyMovie(trickingMovie)
                                                        goto LAB_00e4450d
                                                    end
                                                end
                                            end
                                            goto LAB_00e437b3
                                        end
                                    end
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(trickingMovie)
                                    goto LAB_00e4450d
                                end
                                if not quest:IsActiveThreadTerminating() then
                                    local scratchValue38 = resources:ScriptThing(resource)
                                    if 0.0 < quest:GetHealth(scratchValue38) then
                                        line = "TEXT_QST_B13_MAGICMAN_BAD_HAIR"
                                        timerId2 = quest:GetHero()
                                        if not me:Speak(timerId2, line, 0, false, true, false) then
                                            quest:PauseAllNonScriptedEntities(false)
                                            resources:DestroyMovie(trickingMovie)
                                            goto LAB_00e4450d
                                        end
                                        while me:IsPerformingScriptTask() do
                                            quest:NewScriptFrame(me)
                                            if quest:IsActiveThreadTerminating() then
                                                quest:PauseAllNonScriptedEntities(false)
                                                resources:DestroyMovie(trickingMovie)
                                                goto LAB_00e4450d
                                            end
                                        end
                                        if quest:IsActiveThreadTerminating() then goto LAB_00e448cc end
                                    end
                                    goto LAB_00e437b3
                                end
                            end
                        end
                    end
                    ::LAB_00e448cc::
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(trickingMovie)
                    goto LAB_00e4450d
                end
                if quest:IsActiveThreadTerminating() then
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(trickingMovie)
                    goto LAB_00e4450d
                end
                scratchValue37 = resources:ScriptThing(resource)
                if 0.0 < quest:GetHealth(scratchValue37) then
                    line = "TEXT_QST_B13_MAGICMAN_TOUTING"
                    timerId2 = quest:GetHero()
                    if not me:Speak(timerId2, line, 0, false, true, false) then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(trickingMovie)
                        goto LAB_00e4450d
                    end
                    while me:IsPerformingScriptTask() do
                        quest:NewScriptFrame(me)
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(trickingMovie)
                            goto LAB_00e4450d
                        end
                    end
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(trickingMovie)
                        goto LAB_00e4450d
                    end
                end
                ::LAB_00e437b3::
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(trickingMovie)
            end
            scratchValue24 = scratchValue25
            scratchValue25 = scratchValue25 | 0x4000
            if not (me:MsgIsHitByHero()) then
                scratchValue25 = scratchValue24 | 0xc000
                if me:MsgIsHitByAnySpecialAbilityFromHero() then
                    scratchValue25 = scratchValue25 | 0x10000
                    if not me:MsgIsHitByHeroSpecialAbility(HERO_ABILITY_HEAL_LIFE_SPELL) then goto LAB_00e43893 end
                end
                isActiveThreadTerminating = false
                goto FLOW_past_lab_00e43893
            end
            ::LAB_00e43893::
            isActiveThreadTerminating = true
            ::FLOW_past_lab_00e43893::
            if (scratchValue25 & 0x10000) ~= 0 then
                scratchValue25 = scratchValue25 & 0xfffeffff
            end
            if (scratchValue25 & 0x8000) ~= 0 then
                scratchValue25 = scratchValue25 & 0xffff7fff
            end
            if (scratchValue25 & 0x4000) ~= 0 then
                scratchValue25 = scratchValue25 & 0xffffbfff
            end
            if isActiveThreadTerminating then
                if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                me:ClearCommands()
                local movie2 = resources:StartMovie("")
                quest:PauseAllNonScriptedEntities(true)
                quest:FixMovieSequenceCamera(true)
                local gsi_stack_result_24004 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, gsi_stack_result_24004, false)
                quest:Pause(1.0)
                local __push31 = quest:GetHero()
                quest:EntitySetFacingAngleTowardsThing(me, __push31, true)
                quest:NewScriptFrame(me)
                quest:CameraUseCameraPoint("CAM_B_OWNER", me, -1.0, 0, -1)
                if helpers.IsHeroWearingBeard(quest, me) then
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        goto LAB_00e4450d
                    end
                    local scratchValue36 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue36) then
                        line = "TEXT_QST_B13_MAGICMAN_ATTACKED_FEMALE"
                        timerId2 = quest:GetHero()
                        if not me:Speak(timerId2, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00e4450d
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto LAB_00e4450d
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00e4450d
                        end
                    end
                else
                    if quest:IsActiveThreadTerminating() then
                        quest:PauseAllNonScriptedEntities(false)
                        resources:DestroyMovie(movie2)
                        goto LAB_00e4450d
                    end
                    local scratchValue40 = resources:ScriptThing(resource)
                    if 0.0 < quest:GetHealth(scratchValue40) then
                        line = "TEXT_QST_B13_MAGICMAN_ATTACKED_MALE"
                        timerId2 = quest:GetHero()
                        if not me:Speak(timerId2, line, 0, false, true, false) then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00e4450d
                        end
                        while me:IsPerformingScriptTask() do
                            quest:NewScriptFrame(me)
                            if quest:IsActiveThreadTerminating() then
                                quest:PauseAllNonScriptedEntities(false)
                                resources:DestroyMovie(movie2)
                                goto LAB_00e4450d
                            end
                        end
                        if quest:IsActiveThreadTerminating() then
                            quest:PauseAllNonScriptedEntities(false)
                            resources:DestroyMovie(movie2)
                            goto LAB_00e4450d
                        end
                    end
                end
                quest:FixMovieSequenceCamera(false)
                quest:PauseAllNonScriptedEntities(false)
                resources:DestroyMovie(movie2)
            end
            g_PresentedItemName = nil
            if me:MsgIsPresentedWithItem() then
                if g_PresentedItemName == "OBJECT_BEER_TANKARD" then
                    if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
                    me:ClearCommands()
                    quest:FixMovieSequenceCamera(true)
                    local __push34 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, __push34, false)
                    quest:Pause(1.0)
                    local hero8 = quest:GetHero()
                    quest:EntitySetFacingAngleTowardsThing(me, hero8, true)
                    quest:NewScriptFrame(me)
                    quest:CameraUseCameraPoint("CAM_B_OWNER", me, -1.0, 0, -1)
                    startMovie = resources:StartMovie("")
                    quest:PauseAllNonScriptedEntities(true)
                    local switch = quest:GetStateInt("BeersDrunk")
                    repeat
                        if switch == 0 then
                            getHero = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(getHero) then
                                getHomePos = "TEXT_QST_B13_MAGICMAN_BEER1" .. helpers.GetHeroStatusTextTag(quest, me)
                                timerId2 = quest:GetHero()
                                if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                    goto LAB_00e44903
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e44903 end
                                end
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(startMovie)
                                    goto LAB_00e4450d
                                end
                            end
                            break
                        elseif switch == 1 then
                            getHero = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(getHero) then
                                getHomePos = "TEXT_QST_B13_MAGICMAN_BEER2" .. helpers.GetHeroStatusTextTag(quest, me)
                                timerId2 = quest:GetHero()
                                if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                    goto LAB_00e44903
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e44903 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                            end
                            break
                        elseif switch == 2 then
                            getHero = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(getHero) then
                                getHomePos = "TEXT_QST_B13_MAGICMAN_BEER3" .. helpers.GetHeroStatusTextTag(quest, me)
                                timerId2 = quest:GetHero()
                                if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                    goto LAB_00e44903
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e44903 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                            end
                            break
                        elseif switch == 3 then
                            local scratchValue26 = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(scratchValue26) then
                                getHomePos = "TEXT_QST_B13_MAGICMAN_BEER4" .. helpers.GetHeroStatusTextTag(quest, me)
                                timerId2 = quest:GetHero()
                                if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                    goto LAB_00e44903
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e44903 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                            end
                            break
                        elseif switch == 4 then
                            getHero = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(getHero) then
                                getHomePos = "TEXT_QST_B13_MAGICMAN_BEER5" .. helpers.GetHeroStatusTextTag(quest, me)
                                timerId2 = quest:GetHero()
                                if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                    goto LAB_00e44903
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e44903 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                            end
                            break
                        elseif switch == 5 then
                            getHero = resources:ScriptThing(resource)
                            if 0.0 < quest:GetHealth(getHero) then
                                getHomePos = "TEXT_QST_B13_MAGICMAN_BEER6" .. helpers.GetHeroStatusTextTag(quest, me)
                                timerId2 = quest:GetHero()
                                if not me:Speak(timerId2, getHomePos, 0, false, true, false) then
                                    goto LAB_00e44903
                                end
                                while me:IsPerformingScriptTask() do
                                    if not quest:NewScriptFrame(me) then goto LAB_00e44903 end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                            end
                            if not quest:GetStateBool("HeroFoundDeedsLocation") then
                                if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                                helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANDRUNK", false)
                                quest:SetStateBool("HeroFoundDeedsLocation", true)
                            else
                                if quest:IsActiveThreadTerminating() then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(startMovie)
                                    goto LAB_00e4450d
                                end
                                helpers.PlayCutscene(quest, me, "CS_BORDELLO_MAGICIANDRUNK_QUICKIE", false)
                            end
                            break
                        else
                            local scratchValue32 = resources:ScriptThing(resource)
                            if not (0.0 < quest:GetHealth(scratchValue32)) then
                                quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)
                            else
                                line = "TEXT_QST_B13_MAGICMAN_BEERTOOMUCH"
                                timerId2 = quest:GetHero()
                                if not me:Speak(timerId2, line, 0, false, true, false) then
                                    quest:PauseAllNonScriptedEntities(false)
                                    resources:DestroyMovie(startMovie)
                                    goto LAB_00e4450d
                                end
                                while me:IsPerformingScriptTask() do
                                    quest:NewScriptFrame(me)
                                    if quest:IsActiveThreadTerminating() then
                                        quest:PauseAllNonScriptedEntities(false)
                                        resources:DestroyMovie(startMovie)
                                        goto LAB_00e4450d
                                    end
                                end
                                if quest:IsActiveThreadTerminating() then goto LAB_00e44903 end
                                quest:GiveHeroObject("OBJECT_BEER_TANKARD", -1, true)
                            end
                        end
                    until true
                    quest:SetStateInt("BeersDrunk", quest:GetStateInt("BeersDrunk") + 1)
                    quest:FixMovieSequenceCamera(false)
                    quest:PauseAllNonScriptedEntities(false)
                    resources:DestroyMovie(startMovie)
                end
            end
        end
        if quest:IsActiveThreadTerminating() then goto LAB_00e4450d end
        getStateBool = quest:GetStateBool("PlayerOwned")
    end
    if not quest:IsActiveThreadTerminating() then
        quest:RemoveThing(me, false, true)
    end
    ::LAB_00e4450d::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId3)
    ::LAB_00e4451f::
    resources:ReleaseResource(resource)
    do return end
    ::LAB_00e44615::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie4)
    ::LAB_00e448a1::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId3)
    goto LAB_00e4451f
    ::LAB_00e44811::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie3)
    ::LAB_00e44862::
    quest:DeregisterTimer(timerId)
    quest:DeregisterTimer(timerId3)
    goto LAB_00e4451f
    ::LAB_00e44903::
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(startMovie)
    goto LAB_00e4450d
end

-- Magicman.Init (retail 0x00e3b060)
function Init(quest, me)
    quest:SetThingHasInformation(me, true, false, false)
    quest:EntitySetAsKillable(me, false, false)
    quest:EntitySetAsDamageable(me, false)
    quest:EntitySetAsToAddToComboMultiplierWhenHit(me, false)
    quest:SetIsPushableByHero(me, false)
    quest:SetIsThingForcePushable(me, false)
    doneToutIntro_ = false
    mentionedPimpHat = false
end

-- Magicman.OnPersist (retail 0x00e3bad0)
function OnPersist(quest, me, context)
    local doneToutIntro = doneToutIntro_ or false
    doneToutIntro_ = quest:PersistTransferBool(context, "DoneToutIntro", doneToutIntro)
end

-- Magicman.OnPredicateFail (retail 0x00cdebd0)
function OnPredicateFail(quest, me)
end

