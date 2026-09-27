-- Generated from the same native helper bodies as the quest draft.
local helper_ECE460
function helper_ECE460(quest, me)
    local resources = quest:RetailResources()
    local p0, p1, pOther
    -- TODO(native): local_1c = malloc(0x18);
    -- TODO(native): *local_1c = 0;
    -- TODO(native): *(undefined4 *)(local_1c + 4) = 0;
    -- TODO(native): *(undefined1 **)(local_1c + 8) = local_1c;
    -- TODO(native): *(undefined1 **)(local_1c + 0xc) = local_1c;
    local xStack_10 = resources:NewResource()
    local p3 = xStack_10
    local p2 = quest:GetHero()
    me:AcquireControl(4)
    if unaff_EBP == nil then
    else
        -- TODO(native): p1 = *unaff_EBP
        p1 = nil --[[unresolved native value]]
    end
    if xStack_10 == nil then
    else
        -- TODO(native): p0 = *xStack_10
        p0 = nil --[[unresolved native value]]
    end
    local iVar6 = _stricmp(p0,p1,p2,p3)
    if iVar6 ~= 0 then
        pOther = xStack_10
        resources:SetString(0x0, "$ARG1", pOther)
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
    resources:RunMacroWithStrings(xStack_10, xStack_28, 0, false, true)
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
    resources:ReleaseResource(xStack_10)
    if 0 ~= 0 then
        -- TODO(native): LTextBinTree<LTextGroup*>::LTextTreeWalkThrough::BuildTreeArray((LTextTreeWalkThrough *)&xStack_1c,*(int *)((int)xStack_1c + 4));
        -- TODO(native): *(void **)((int)xStack_1c + 8) = xStack_1c;
        -- TODO(native): *(undefined4 *)((int)xStack_1c + 4) = 0;
        -- TODO(native): *(void **)((int)xStack_1c + 0xc) = xStack_1c;
    end
    if nil ~= nil then
        -- TODO(native): free(xStack_1c);
    end
end

return {helper_ECE460 = helper_ECE460}
