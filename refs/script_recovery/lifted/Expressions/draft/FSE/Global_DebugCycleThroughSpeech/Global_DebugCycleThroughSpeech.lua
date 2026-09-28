-- Generated native draft: Global_DebugCycleThroughSpeech. Review coverage report before use.
-- Registration remains disabled until the package is verified.

function Main(quest)
    local resources = quest:RetailResources()
    local bVar2, iVar5, i_stk_18, pCVar4, r1, uVar6, xStack_10, xStack_1c
    local alive = true
    local pCVar3 = quest:GetHero()
    local xStack_28 = nil
    -- TODO(native): xStack_28._4_4_ = *(int **)(pCVar3 + 0x4);
    local __native_condition_1 = (xStack_28 ~= nil and not xStack_28:IsNull())
    local cVar1
    if __native_condition_1 then
        cVar1 = (xStack_28 ~= nil and xStack_28:IsAlive())
        __native_condition_1 = cVar1
    end
    if __native_condition_1 then
        alive = not quest:IsActiveThreadTerminating()
        bVar2 = not alive
        if bVar2 then
            -- LAB_00ee93a8: (native jump target)
            return
        end
        -- TODO(native): List_Node_Initialize(&xStack_1c,(int)&xStack_2d,(int)&xStack_2e);
        r1 = quest:DebugGetAllTextEntriesForTargetedThing()
        if i_stk_18 ~= 0 then
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- TODO(native): LTextTreeWalkThrough_dtor(&xStack_1c);
                return
            end
            quest:FixMovieSequenceCamera(true)
            xStack_10 = resources:NewResource()
            resources:TryAcquire(xStack_10, xStack_28, 4)
            -- TODO(native): uVar6 = *(xStack_1c + 8)
            uVar6 = nil --[[unresolved native value]]
            if uVar6 ~= xStack_1c then
                repeat
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        resources:ReleaseResource(xStack_10)
                        -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)&xStack_1c);
                        -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_1c);
                        return
                    end
                    bVar2 = true
                    -- TODO(native): pCVar4 = tostring(*(uVar6 + 0x10))
                    pCVar4 = nil --[[unresolved native value]]
                    quest:AddScreenTitleMessage(r1, pCVar4, 0.0)
                    -- TODO(native): iVar5 = *(uVar6 + 0x10)
                    iVar5 = nil --[[unresolved native value]]
                    pCVar3 = quest:GetHero()
                    -- TODO(native): Speak: unresolved entity receiver/resource in quest context; arguments: (int)pCVar3,iVar5,p2,p3,p4,p5
                    iVar5 = resources:IsPerformingScriptTask(xStack_10)
                    cVar1 = iVar5
                    while cVar1 do
                        alive = quest:NewScriptFrame()
                        alive = not quest:IsActiveThreadTerminating()
                        bVar2 = not alive
                        if bVar2 then
                            resources:ReleaseResource(xStack_10)
                            -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)&xStack_1c);
                            -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_1c);
                            return
                        end
                        iVar5 = resources:IsPerformingScriptTask(xStack_10)
                        cVar1 = iVar5
                    end
                    alive = not quest:IsActiveThreadTerminating()
                    bVar2 = not alive
                    if bVar2 then
                        resources:ReleaseResource(xStack_10)
                        -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)&xStack_1c);
                        -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_1c);
                        return
                    end
                    -- TODO(native): uVar6 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_01);
                until not (uVar6 ~= xStack_1c)
            end
            alive = not quest:IsActiveThreadTerminating()
            bVar2 = not alive
            if bVar2 then
                -- LAB_00ee9391: (native jump target)
                resources:ReleaseResource(xStack_10)
                -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)&xStack_1c);
                -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_1c);
                return
            end
            quest:FixMovieSequenceCamera(false)
            resources:ReleaseResource(xStack_10)
        end
        -- TODO(native): LTextTreeWalkThrough_Cleanup(&xStack_1c);
    end
    pCVar4 = quest:GetActiveQuestName()
    quest:DeactivateQuestLater(pCVar4, 0)
    xStack_28 = nil
end

function Init(quest)
end

function OnPersist(quest, context)
end

