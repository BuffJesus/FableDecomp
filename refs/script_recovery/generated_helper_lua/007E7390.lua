-- Retail helper 0x007E7390 (CScriptGameResourceObjectScriptedThingBase::?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@PBDW4ETextGroupSelectionMethod@@_N33@Z)
-- Preserve the null guard and tail-forward every caller argument opaquely.
return function(get_resource, invoke_resource, ...)
    local resource = get_resource(8)
    if resource ~= nil then
        invoke_resource(resource, "SpeakCString", 52, ...)
    end
end
