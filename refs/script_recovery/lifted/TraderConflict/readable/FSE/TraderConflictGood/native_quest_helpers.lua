-- Generated from the same native helper bodies as the quest draft.
local UpdateLiveEnemies, helper_DFDED0
function UpdateLiveEnemies(quest, me)
    local isActiveThreadTerminating, isActiveThreadTerminating2, isActiveThreadTerminating3
    local isActiveThreadTerminating4, isActiveThreadTerminating5, isActiveThreadTerminating6
    local isActiveThreadTerminating7, isActiveThreadTerminating8, predicateResult, followers
    local scratchValue, p0, scratchValue8, scratchValue9
    quest:StateListClear("AllCreatures")
    quest:StateListSet("AllCreatures", quest:GetAllCreaturesExcludingHero())
    scratchValue8 = 0
    while scratchValue8 ~= quest:GetStateListCount("AllCreatures") do
        isActiveThreadTerminating = quest:IsActiveThreadTerminating()
        if isActiveThreadTerminating then
            return isActiveThreadTerminating
        end
        if ((quest:GetStateListAt("AllCreatures", scratchValue8):GetDefName() == "CREATURE_NEW_CHICKEN_04") and 0 or 1) ~= 0 then
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "TraderToRescue" then goto LAB_00dfc413 end
            isActiveThreadTerminating2 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating2 then
                return isActiveThreadTerminating2
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc413::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "BodyGuard" then goto LAB_00dfc45c end
            isActiveThreadTerminating3 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating3 then
                return isActiveThreadTerminating3
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc45c::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "RingFighter" then goto LAB_00dfc4a5 end
            isActiveThreadTerminating4 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating4 then
                return isActiveThreadTerminating4
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc4a5::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "FisticuffsMember" then goto LAB_00dfc4ee end
            isActiveThreadTerminating5 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating5 then
                return isActiveThreadTerminating5
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc4ee::
            if quest:GetStateListAt("AllCreatures", scratchValue8):GetName() ~= "Tyler" then goto LAB_00dfc537 end
            isActiveThreadTerminating6 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating6 then
                return isActiveThreadTerminating6
            end
            quest:StateListErase("AllCreatures", scratchValue8)
            goto FLOW_after_lab_00dfc3b5
            ::LAB_00dfc537::
            isActiveThreadTerminating7 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating7 then
                return isActiveThreadTerminating7
            end
            scratchValue8 = scratchValue8 + 1
        else
            isActiveThreadTerminating8 = quest:IsActiveThreadTerminating()
            if isActiveThreadTerminating8 then
                return isActiveThreadTerminating8
            end
            quest:StateListErase("AllCreatures", scratchValue8)
        end
        ::FLOW_after_lab_00dfc3b5::
    end
    predicateResult = quest:IsActiveThreadTerminating()
    scratchValue = predicateResult
    if not predicateResult then
        scratchValue9 = 0
        followers = quest:GetFollowingEntityList(quest:GetHero())
        if #followers ~= 0 then
            scratchValue = 4
            repeat
                if quest:IsActiveThreadTerminating() then goto LAB_00dfc618 end
                p0 = 0
                while p0 ~= quest:GetStateListCount("AllCreatures") do
                    -- TODO(native): cVar8 = (**(**(iVar3 + iStack_c) + 0x138))(quest:GetStateListAt("AllCreatures", p0))
    --[[unresolved native value]]
                    if nil == 0 then
                        p0 = p0 + 1
                    else
                        quest:StateListErase("AllCreatures", p0)
                        break
                        p0 = p0 + 1
                    end
                end
                scratchValue9 = scratchValue9 + 1
                scratchValue = scratchValue + 12
            until scratchValue9 >= #followers
        end
        ::LAB_00dfc618::
    end
    return scratchValue
end

function helper_DFDED0(quest, me, strParam1)
    local resources = quest:RetailResources()
    local resource = resources:NewResource()
    local pScriptObject = resource
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local actorMap = resources:NewActorMap()
    resources:SetActor(actorMap, "HERO", resource)
    local movie = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(strParam1, actorMap, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(movie)
    resources:DestroyActorMap(actorMap)
    resources:ReleaseResource(resource)
end

return {UpdateLiveEnemies = UpdateLiveEnemies, helper_DFDED0 = helper_DFDED0}
