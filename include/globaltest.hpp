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

	// Liu and Anderson's self-suspension and ksi time granularity parameter.
	lt_t la_delta_susp;	
	lt_t la_delta_ksi;

protected:
  void drawTaskSetSafeApprox();
  void drawTaskSetSafeApprox_DIH();


public :
  GlobalTest();
  ~GlobalTest();

	enum do_test_t {ALL=1, DENSITY_TEST, LIU_ANDERSON_TEST} do_test;
	int makeSchedTest(); 
  int makeSchedTest(enum do_test_t); 
  int makeDensityTest();
  int makeCong12Test(long,long);

	int setParameters(const CmdlParser&);


};


#endif
