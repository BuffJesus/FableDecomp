struct StdVector { void* first; void* last; void* end; };
void __fastcall StdVector_Construct(StdVector* self)
{
    self->first = 0;
    self->last  = 0;
    self->end   = 0;
}