void __fastcall GFLimitReturn(float* value,
                                      const float* minimum,
                                      const float* maximum)
{
    if (*value < *minimum) {
        *value = *minimum;
        return;
    }
    if (*value > *maximum)
        *value = *maximum;
}