#include <iostream>
#include <bitset>

using namespace std;

int main(){

  // 11111110 00110100 = -460
  // 00000000 00111100 = 60

  char t = 0x00;
  char s = 0x3C;

  int n = (t<<8)|s;

  bool q = (n & 0x8000);
  cout<<n - 65536*(int)q<<endl;


return 0;
}
