struct C {
    int field0;
    C* SetToMinusOne();
};

C* C::SetToMinusOne()
{
    this->field0 = -1;
    return this;
}