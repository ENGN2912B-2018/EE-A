#include <iostream>
#include <time.h>

int main () {
   time_t curtime;

   time(&curtime);

   cout<<"Current time ="<<curtime<<endl;;

   return(0);
}
