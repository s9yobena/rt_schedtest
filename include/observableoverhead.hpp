#ifndef OBSERVABLE_OVERHEAD_H
#define OBSERVABLE_OVERHEAD_H

#include "schedtestobserver.hpp"

class ObservableOverhead {
  
public:
  virtual void setObserver(SchedTestObserver*);
  virtual void updateObservers();

};

#endif
