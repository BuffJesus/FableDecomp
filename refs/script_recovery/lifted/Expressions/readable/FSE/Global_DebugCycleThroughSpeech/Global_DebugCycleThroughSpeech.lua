-- Readable native conversion: Global_DebugCycleThroughSpeech. Review coverage report before use.
-- Registration remains disabled until the package is verified.

-- Global_DebugCycleThroughSpeech.Main (retail 0x00ee91e0)
function Main(quest)
    local resources = quest:RetailResources()
    local scratchValue, scratchValue3
    local thing = nil
    -- TODO(native): xStack_28 = *(int **)(pCVar3 + 0x4);
    if not ((thing ~= nil and not thing:IsNull()) and thing:IsAlive()) then quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0); return end
    if quest:IsActiveThreadTerminating() then return end
    -- TODO(native): List_Node_Initialize(&xStack_1c,(int)&xStack_2d,(int)&xStack_2e);
    local scratchValue2 = quest:DebugGetAllTextEntriesForTargetedThing()
    if scratchValue ~= 0 then
        if quest:IsActiveThreadTerminating() then
            -- TODO(native): LTextTreeWalkThrough_dtor(&xStack_1c);
            return
        end
        quest:FixMovieSequenceCamera(true)
        local resource = resources:NewResource()
        resources:TryAcquire(resource, nil, 4)
        -- TODO(native): uVar6 = *(xStack_1c + 8)
    --[[unresolved native value]]
        while nil ~= scratchValue3 do
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)&xStack_1c);
                -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_1c);
                return
            end
            -- TODO(native): pCVar4 = tostring(*(uVar6 + 0x10))
    --[[unresolved native value]]
            quest:AddScreenTitleMessage(scratchValue2, nil, 0.0)
            -- TODO(native): iVar5 = *(uVar6 + 0x10)
--[[unresolved native value]]
            -- TODO(native): Speak: unresolved entity receiver/resource in quest context; arguments: (int)pCVar3,iVar5,p2,p3,p4,p5
            while resources:IsPerformingScriptTask(resource) do
                quest:NewScriptFrame()
                if quest:IsActiveThreadTerminating() then
                    resources:ReleaseResource(resource)
                    -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)&xStack_1c);
                    -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_1c);
                    do return end
                end
            end
            if quest:IsActiveThreadTerminating() then
                resources:ReleaseResource(resource)
                -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)&xStack_1c);
                -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_1c);
                return
            end
            -- TODO(native): uVar6 = CMemoryAllocatorVariableSize::GetNoAllocatedAreas(this_01);
        end
        if quest:IsActiveThreadTerminating() then
            resources:ReleaseResource(resource)
            -- TODO(native): CTCCarryable::OnKill((CTCCarryable *)&xStack_1c);
            -- TODO(native): CFileInstaller::CActiveFile::OnReadFinished((CActiveFile *)&xStack_1c);
            return
        end
        quest:FixMovieSequenceCamera(false)
        resources:ReleaseResource(resource)
    end
    -- TODO(native): LTextTreeWalkThrough_Cleanup(&xStack_1c);
    quest:DeactivateQuestLater(quest:GetActiveQuestName(), 0)
end

-- Global_DebugCycleThroughSpeech.Init (retail 0x00ee9110)
function Init(quest)
end

-- Global_DebugCycleThroughSpeech.OnPersist (retail 0x00cbd4e0)
function OnPersist(quest, context)
end

