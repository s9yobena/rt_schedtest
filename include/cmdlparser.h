#ifndef CMDLPARSER_H
#define CMDLPARSER_H

#include <stdio.h>
#include <cstdlib>
#include <getopt.h>



#define MAX_RT_TASK_NR 100

#define USAGE							\
	"rt_schedtest [--timestamps] [--overheads] [--cpmd <CPMD>] <RT_TASK_ID1> <RT_TASK_ID2>...\n" \
	"   --timestamps: print timestamps \n"			\
	"   --overheads:  print overheads \n"			\
	"   --exectimes: print execution times \n"              \
	"   --schedtraces: print scheduling traces \n"		\
	"   -cpmd: cache related preemption and migration delay \n " \
	"   RT_TASK_ID: id of process implementing RT_TASK \n" \
	""


class CmdlParser {
 public :
  int rt_task_id[MAX_RT_TASK_NR];
  int nb_rts;
  int cpmd;
  bool printTimestamps;
  bool printOverheads;
  bool printSchedulingTraces;
  bool printExecutionTimes;


  CmdlParser(int, char**);
  ~CmdlParser();


};

#endif
