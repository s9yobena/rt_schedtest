/* USAGE message from ft_tools */
#ifndef CMDLPARSER_H
#define CMDLPARSER_H

#include <stdio.h>
#include <cstdlib>
#include <getopt.h>
#include <string.h>
#include "rttypes.h"

#define USAGE							\
	"rt_schedtest [--timestamps] [--overheads] [--cpmd <CPMD>] [--stf <inputfilename.stf>]\n" \
	"   --timestamps: print timestamps \n"			\
	"   --overheads:  print overheads \n"			\
	"   --exectimes: print execution times \n"              \
	"   --schedtraces: print scheduling traces \n"		\
	"   --cpmd: cache related preemption and migration delay \n " \
	"   --stf: scheduling test input file \n " \
	"   One of the following schedulability tests:  \n " \
	"   --density-test  \n " \
	""


class CmdlParser {
 public :
  int cpmd;
  bool printTimestamps;
  bool printOverheads;
  bool printSchedulingTraces;
  bool printExecutionTimes;
  char stfName[maxSTFname];
  SchedTestName schedTestName;

	bool density_test; 	// if true, only conduct density test

	// Liu and Anderson's self-suspension and ksi time granularity parameter.
	lt_t la_delta_susp;	
	lt_t la_delta_ksi;
	bool la_test;		// if true, only conduct Liu and Anderson's test

  CmdlParser(int, char**);
  ~CmdlParser();


};

#endif
