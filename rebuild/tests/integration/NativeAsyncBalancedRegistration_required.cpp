#include "fable_game_text_bank.h"
typedef _STL::set<CBankFileAsyncData*>::_Rep_type Tree;
int main(int argc,char**argv){
    CBankFileAsyncData* key=(CBankFileAsyncData*)(argv+argc);
    ((Tree*)argv)->insert_unique(key);
    return 0;
}
