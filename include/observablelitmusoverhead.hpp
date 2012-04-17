#ifndef OBSERVABLE_LITMUS_OVERHEAD_H
#define OBSERVABLE_LITMUS_OVERHEAD_H

#include "litmusoverheadobserver.hpp"

class ObservableLitmusOverhead {
  
public:
  virtual void setObserver(LitmusOverheadObserver*);
  virtual void updateObservers();

};

#endif
