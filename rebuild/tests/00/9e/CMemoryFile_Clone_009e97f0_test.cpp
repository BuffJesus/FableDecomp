#include <stdio.h>
struct Base{virtual ~Base(){}};struct File:Base{long Position;File():Position(0){}File(const File& f):Position(f.Position){}
 Base* Clone()const{return new File(*this);}};
int main(){File f;f.Position=17;Base* b=f.Clone();File* copy=(File*)b;if(copy==&f||copy->Position!=17)return 1;
 delete b;printf("MEMORY_FILE_CLONE PASS\n");return 0;}