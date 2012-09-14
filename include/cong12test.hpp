#ifndef CONG_12_TEST_HPP
#define CONG_12_TEST_HPP

#include "globaltest.hpp"

class Task;

class Cong12Test : public GlobalTest{

private:
  long double DBF(Task *_task, lt_t _t);
  long double DELTA(Task *_task, lt_t _t);
  long double W_nc(Task *_iTask, Task *_lTask, long double _lKsi, long double _lPerJobSelfSusp);
  long double W_c(Task *_iTask, Task *_lTask, long double _lKsi, long double _lPerJobSelfSusp);

public :
  Cong12Test();
  ~Cong12Test();

  int makeSchedTest(long,long);
};


#endif
