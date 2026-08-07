struct S { virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
           virtual void Write(void* p, int n); void WriteCBYTE(char c); };
void S::WriteCBYTE(char c){ this->Write(&c, 4); }