// CCountedPointer<T>::operator!= — compare the held raw pointers.
// __fastcall this=ecx, other=stack (ret 4).
struct CP {
    void* p;
    bool NotEqual(const CP* o) const;
};
bool CP::NotEqual(const CP* o) const { return this->p != o->p; }
