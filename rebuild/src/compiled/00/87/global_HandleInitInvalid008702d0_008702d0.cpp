struct InvalidHandle
{
    long value00;
    unsigned long value04;
    unsigned char value08;
    unsigned char pad09[3];
};
InvalidHandle* __fastcall Handle_InitInvalid_008702d0(InvalidHandle* handle)
{
    handle->value00 = -1;
    handle->value04 = 0;
    handle->value08 = 0;
    return handle;
}