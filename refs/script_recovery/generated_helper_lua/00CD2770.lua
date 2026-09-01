-- Retail helper 0x00CD2770 (CMemoryDataOutputStream::Clear)
-- Preserve owned-resource destruction and the following zero write in native order.
return function(destroy_owned, write_u32)
    destroy_owned(8, 8286432)
    write_u32(8, 0)
end
