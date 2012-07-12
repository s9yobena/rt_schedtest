#include "task.hpp"

Task::Task() {
}

Task::~Task() {
}

unsigned Task::getCpu() {
  return parameters.cpu;
}

lt_t Task::getExecCost() {
  return parameters.exec_cost;
}

lt_t Task::getPeriod() {
  return parameters.period;
}

lt_t Task::getSelfSuspension() {
  return selfSuspension;
}

void Task::setExecCost(lt_t execCost) {
  parameters.exec_cost = execCost;
}

void Task::setPeriod(lt_t period) {
  parameters.period = period;
}

void  Task::setSelfSuspension(lt_t ss) {
  selfSuspension = ss;
}

void  Task::addSelfSuspension(lt_t ss) {
  selfSuspension += ss;
}

void Task::setParameters(struct rt_task taskParams) {
  parameters = taskParams;
}

