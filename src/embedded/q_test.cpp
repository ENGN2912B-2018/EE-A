#include <iostream>
#include <thread>
#include "circularfifo_memory_sequential_consistent.hpp"
#include <bitset>
using namespace std;
using namespace memory_sequential_consistent;

void thread_spi(CircularFifo<char*, 10> *queue){

 char data[] = {0xFF, 0x0F, 0x00};

    queue->push(data);
}


int main(){
  // init queue
  CircularFifo<char*, 10> queue;

  // create thread and pass buffer and length to it
  thread thread_obj(thread_spi, &queue);
  //check the output
  thread_obj.join();

  while(1){
    char* m;
    if (false == queue.pop(m)) {cout<<"q empty!"<<endl; break;}
    else{
      bitset<8> bq1(*(m+sizeof(char))), bq2(m[1]), bq3(m[2]);
      cout<<"bits after q: "<<bq1<<" "<<bq2<<" "<<bq3<<endl;

    }

  }


  }
