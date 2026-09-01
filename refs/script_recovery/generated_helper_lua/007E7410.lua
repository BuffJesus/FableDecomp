-- Retail helper 0x007E7410 (CScriptGameResourceObjectScriptedThingBase::?ClearAllActionsIncludingLoopingAnimations@CScriptGameResourceObjectScriptedThingBase@@UAEXXZ)
-- Preserve the wrapped-resource null guard and virtual operation through callbacks.
return function(get_resource, invoke_resource)
    local resource = get_resource(8)
    if resource ~= nil then
        invoke_resource(resource, "ClearAllActionsIncludingLoopingAnimations", 88)
    end
end
