void __thiscall $E1(
    CArray<std::pair<unsigned long, CSoundPair>>* self,
    std::pair<unsigned long, CSoundPair>* value)
{
    std::uint32_t scratch[21]{};
    self->push_back(value, scratch);
}