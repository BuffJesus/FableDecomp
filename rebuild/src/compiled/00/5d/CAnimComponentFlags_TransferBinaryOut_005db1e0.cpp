struct CDataOutputStream { void Write(void* d); };
struct S { char pad[8]; void* f8; void TransferBinaryOut(CDataOutputStream* s); };
void S::TransferBinaryOut(CDataOutputStream* s){ s->Write(this->f8); }