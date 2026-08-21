// `return this->[0x0] + 0x4;` -- member pointer advanced by a constant.
#pragma pack(push,1)
struct T {
    char* ptr;
    char* Advance();
};
#pragma pack(pop)
char* T::Advance() { return this->ptr + 0x4; }