
#include <cstdio>
struct Prog { virtual void stop(bool a,bool b)=0; };
static int calls=0; static bool ga,gb;
struct Impl:Prog { void stop(bool a,bool b){calls++;ga=a;gb=b;} };
Prog* g_prog=0;
void model(bool a,bool b){ Prog*p=g_prog; if(p) p->stop(a,b);}
int main(){
  g_prog=0; calls=0; model(true,false);
  if(calls!=0){printf("FAIL null\n");return 1;}
  Impl obj; g_prog=&obj;
  model(true,false);
  if(calls!=1||ga!=true||gb!=false){printf("FAIL c1\n");return 1;}
  model(false,true);
  if(calls!=2||ga!=false||gb!=true){printf("FAIL c2\n");return 1;}
  printf("OK_009e9f20\n"); return 0;
}