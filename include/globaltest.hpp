#ifndef GLOBALTEST_H
#define GLOBALTEST_H

#include "schedtest.hpp"

class GlobalTest : public SchedTest{

protected:
  void drawTaskSetSafeApprox();

public :
  GlobalTest();
  ~GlobalTest();

};


#endif
