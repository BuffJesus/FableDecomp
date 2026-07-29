// GFWrapLimit<long, int, int> @ 0x00496EB0.

void __fastcall GFWrapLimit_00496eb0(long* value, int* minimum, int* maximum)
{
    if (*value < *minimum)
    {
        *value = *maximum;
    }
    else if (*value > *maximum)
    {
        *value = *minimum;
    }
}
