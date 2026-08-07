struct S { char* M(int i); };
char* S::M(int i){ return (char*)this + i*0x648 + 4; }