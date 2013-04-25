#ifndef GLOBALTEST_H
#define GLOBALTEST_H

#include "schedtest.hpp"

class GlobalTest : public SchedTest{

private:

  bool dedicatedIH;
  
  long double DBF(Task *_task, lt_t _t);
  long double DELTA(Task *_task, lt_t _t);
  long double W_nc(Task *_iTask, Task *_lTask, long double _lKsi, long double _lPerJobSelfSusp);
  long double W_c(Task *_iTask, Task *_lTask, long double _lKsi, long double _lPerJobSelfSusp);

protected:
  void drawTaskSetSafeApprox();
  void drawTaskSetSafeApprox_DIH();


public :
  GlobalTest();
  ~GlobalTest();
  
  int makeSchedTest(); 
  int makeDensityTest();
  int makeCong12Test(long,long);

};


#endif
