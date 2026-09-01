-- Retail helper 0x00CB8930 (CTCVillage::OnInitialActivate)
-- Preserve the conditional byte state, exact native call target, and clearing write.
return function(read_u8, write_u8, invoke_native)
    if read_u8(37) ~= 0 then
        invoke_native(13361008, read_u8(36))
        write_u8(37, 0)
    end
end
