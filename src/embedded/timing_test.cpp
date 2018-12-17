#include <iostream>
#include <time.h>

using namespace std;


int main () {
   clock_t curtime;

   curtime = clock();

   for(int i = 0; i<100000; i++){}

   curtime = clock() - curtime;

   float clocktime = (float)curtime/CLOCKS_PER_SEC;

   cout<<"Current time ="<<clocktime<<endl;;

   return(0);
}
