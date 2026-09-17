-- Generated from the same native helper bodies as the quest draft.
local UpdateLiveEnemies, helper_DFDED0
function UpdateLiveEnemies(quest, me)
    local bVar1, cVar8, iVar3, i_stk_8, lst_AllCreatures, p0, pTarget, piVar2, piVar4, r1, uVar5
    local alive = true
    quest:StateListClear("AllCreatures")
    lst_AllCreatures = quest:GetAllCreaturesExcludingHero()
    quest:StateListSet("AllCreatures", lst_AllCreatures)
    piVar4 = 0
    if piVar4 ~= (quest:GetStateListCount("AllCreatures") * 0xc) then
        repeat
            alive = not quest:IsActiveThreadTerminating()
            bVar1 = not alive
            if bVar1 then
                return bVar1
            end
            piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetDefName()
            iVar3 = ((piVar2 == "CREATURE_NEW_CHICKEN_04") and 0 or 1)
            cVar8 = not (iVar3 ~= 0)
            if not cVar8 then
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "TraderToRescue") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc413 end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return bVar1
                end
                quest:StateListErase("AllCreatures", (piVar4) / 0xc)
                goto FLOW_after_lab_00dfc3b5
                ::LAB_00dfc413::
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "BodyGuard") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc45c end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return bVar1
                end
                quest:StateListErase("AllCreatures", (piVar4) / 0xc)
                goto FLOW_after_lab_00dfc3b5
                ::LAB_00dfc45c::
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "RingFighter") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc4a5 end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return bVar1
                end
                quest:StateListErase("AllCreatures", (piVar4) / 0xc)
                goto FLOW_after_lab_00dfc3b5
                ::LAB_00dfc4a5::
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "FisticuffsMember") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc4ee end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return bVar1
                end
                quest:StateListErase("AllCreatures", (piVar4) / 0xc)
                goto FLOW_after_lab_00dfc3b5
                ::LAB_00dfc4ee::
                piVar2 = quest:GetStateListAt("AllCreatures", (piVar4) / 0xc):GetName()
                iVar3 = ((piVar2 == "Tyler") and 0 or 1)
                if iVar3 ~= 0 then goto LAB_00dfc537 end
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return bVar1
                end
                quest:StateListErase("AllCreatures", (piVar4) / 0xc)
                goto FLOW_after_lab_00dfc3b5
                ::LAB_00dfc537::
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return bVar1
                end
                piVar4 = piVar4 + 0xc
            else
                -- LAB_00dfc3b5: (native jump target)
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then
                    return bVar1
                end
                quest:StateListErase("AllCreatures", (piVar4) / 0xc)
            end
            ::FLOW_after_lab_00dfc3b5::
        until not (piVar4 ~= (quest:GetStateListCount("AllCreatures") * 0xc))
    end
    alive = not quest:IsActiveThreadTerminating()
    bVar1 = not alive
    iVar3 = bVar1
    if not bVar1 then
        uVar5 = 0
        i_stk_8 = 0
        pTarget = quest:GetHero()
        r1 = quest:GetFollowingEntityList(pTarget)
        iVar3 = i_stk_8 - 0 >> 0x1f
        if (i_stk_8 - 0) / 0xc + iVar3 ~= iVar3 then
            iVar3 = 4
            repeat
                alive = not quest:IsActiveThreadTerminating()
                bVar1 = not alive
                if bVar1 then goto LAB_00dfc618 end
                p0 = 0
                if p0 ~= (quest:GetStateListCount("AllCreatures") * 0xc) then
                    repeat
                        -- TODO(native): cVar8 = (**(**(iVar3 + 0) + 0x138))(quest:GetStateListAt("AllCreatures", (p0) / 0xc))
                        cVar8 = nil --[[unresolved native value]]
                        if cVar8 ~= 0 then
                            quest:StateListErase("AllCreatures", (p0) / 0xc)
                            break
                        end
                        p0 = p0 + 0xc
                    until not (p0 ~= (quest:GetStateListCount("AllCreatures") * 0xc))
                end
                uVar5 = uVar5 + 1
                iVar3 = iVar3 + 0xc
            until not (uVar5 < ((i_stk_8 - 0) / 0xc))
        end
        alive = not quest:IsActiveThreadTerminating()
        ::LAB_00dfc618::
    end
    return iVar3
end

function helper_DFDED0(quest, me, native_arg_strParam_1)
    local xStack_20 = resources:NewResource()
    local pScriptObject = xStack_20
    local pThing = quest:GetHero()
    resources:TryAcquire(pScriptObject, pThing, 4)
    local xStack_2c = resources:NewActorMap()
    resources:SetActor(xStack_2c, "HERO", xStack_20)
    local xStack_10 = resources:StartMovie("")
    quest:StartMovieSequence()
    quest:PauseAllNonScriptedEntities(true)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacro(native_arg_strParam_1, xStack_2c, false, true)
    quest:FixMovieSequenceCamera(false)
    quest:PauseAllNonScriptedEntities(false)
    resources:DestroyMovie(xStack_10)
    resources:DestroyActorMap(xStack_2c)
    resources:ReleaseResource(xStack_20)
end

return {UpdateLiveEnemies = UpdateLiveEnemies, helper_DFDED0 = helper_DFDED0}
