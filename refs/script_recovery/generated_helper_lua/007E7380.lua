-- Retail helper 0x007E7380 (CScriptGameResourceObjectScriptedThingBase::?Speak@CScriptGameResourceObjectScriptedThingBase@@UAEXABVCScriptThing@@KW4ETextGroupSelectionMethod@@_N22@Z)
-- Preserve the null guard and tail-forward every caller argument opaquely.
return function(get_resource, invoke_resource, ...)
    local resource = get_resource(8)
    if resource ~= nil then
        invoke_resource(resource, "SpeakTextId", 56, ...)
    end
end
