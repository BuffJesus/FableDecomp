-- Generated from the same native helper bodies as the quest draft.
local helper_ECE460
function helper_ECE460(quest, me)
    local resources = quest:RetailResources()
    local p0, p1
    -- TODO(native): local_1c = malloc(0x18);
    -- TODO(native): *local_1c = 0;
    -- TODO(native): *(undefined4 *)(local_1c + 4) = 0;
    -- TODO(native): *(undefined1 **)(local_1c + 8) = local_1c;
    -- TODO(native): *(undefined1 **)(local_1c + 0xc) = local_1c;
    local resource = resources:NewResource()
    local p3 = resource
    local p2 = quest:GetHero()
    me:AcquireControl(4)
    if unaff_EBP ~= nil then
        -- TODO(native): p1 = *unaff_EBP
        p1 = nil --[[unresolved native value]]
    end
    if resource ~= nil then
        -- TODO(native): p0 = *xStack_10
        p0 = nil --[[unresolved native value]]
    end
    local scratchValue = _stricmp(p0,p1,p2,p3)
    if scratchValue ~= 0 then
        local pOther = resource
        resources:SetString(0, "$ARG1", pOther)
    end
    -- TODO(native): puStack_34 = malloc(0x24);
    -- TODO(native): *puStack_34 = 0;
    -- TODO(native): *(undefined4 *)(puStack_34 + 4) = 0;
    -- TODO(native): *(undefined1 **)(puStack_34 + 8) = puStack_34;
    -- TODO(native): *(undefined1 **)(puStack_34 + 0xc) = puStack_34;
    resources:SetActor(puStack_34, "WITCH", resources:MemberResource("seh_Witch"))
    resources:SetActor(puStack_34, "MUM", resources:MemberResource("seh_Mother"))
    -- TODO(native): resources:SetActor(puStack_34, "HERO", &local_1c)
    quest:FixMovieSequenceCamera(true)
    resources:RunMacroWithStrings(resource, xStack_28, 0, false, true)
    quest:FixMovieSequenceCamera(false)
    if i_stk_14 ~= 0 then
        -- TODO(native): StdMap_DestroyNode(&xStack_28,unaff_EBP[1]);
        -- TODO(native): unaff_EBP[2] = unaff_EBP;
        -- TODO(native): unaff_EBP[1] = 0;
        -- TODO(native): unaff_EBP[3] = unaff_EBP;
    end
    if unaff_EBP ~= nil then
        -- TODO(native): free(unaff_EBP);
    end
    resources:ReleaseResource(resource)
    if nil ~= nil then
        -- TODO(native): free(xStack_1c);
    end
end

return {helper_ECE460 = helper_ECE460}
