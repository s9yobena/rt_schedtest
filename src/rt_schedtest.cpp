/*    rt_schedtest -- Online overhead aware schedulability testing
 *      
 *    Youcef A. Benabbas.
 */
#include "cmdlparser.h"
#include <unistd.h>
#include "configschedtest.hpp"

int main(int argc, char **argv) {
  
  CmdlParser cmdlParser(argc, argv);    
  ConfigSchedTest *configSchedTest;
  configSchedTest = ConfigSchedTest::getInstance();

  configSchedTest->setParameters(cmdlParser);

  configSchedTest->callSchedTest();

  return 0;
}
