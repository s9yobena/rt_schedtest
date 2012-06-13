#include "cmdlparser.h"

CmdlParser::CmdlParser(int argc, char **argv) {

  this->nb_rts = 0;
  this->cpmd = 5000;
  this->printTimestamps = false;
  this->printOverheads = false;
  this->printSchedulingTraces = false;
  this->printExecutionTimes = false;

  

  int c;
  int cpmd_flag = 0;
  int stf_flag = 0;
  int printTimestamps_flag = 0;
  int printOverheads_flag = 0;
  int printSchedulingTraces_flag = 0;
  int printExecutionTimes_flag = 0;
  
  while (1)
    {
      static struct option long_options[] =
	{
	  {"cpmd", required_argument, NULL,'c' },
	  {"stf", required_argument, NULL,'s' },
	  {"timestamps", no_argument, &printTimestamps_flag,1 },
	  {"overheads", no_argument, &printOverheads_flag, 1 },
	  {"schedtraces", no_argument, &printSchedulingTraces_flag, 1 },
	  {"exectimes", no_argument, &printExecutionTimes_flag, 1 },
	  {0, 0, 0, 0}
	};
      /* getopt_long stores the option index here. */
      int option_index = 0;
     
      c = getopt_long (argc, argv, "c:s:tode", long_options, &option_index);
      
      /* Detect the end of the options. */
      if (c == -1)
	break;
      switch (c)
	{
	case 'c':
	  if (optarg) {
	    this->cpmd = atoi(optarg);
	    cpmd_flag = 1;
	  }
	  break;
	case 's':
	  if (optarg) {
	    strcpy(this->stfName, optarg);
	    stf_flag = 1;
	  }
	  break;
	  
	case '?':
	  /* getopt_long already printed an error message. */
	  break;
	default:
	  break;
	}
    }


  if (cpmd_flag)
    printf ("CPMD set to: %d \n ",this->cpmd);
  else
    printf ("CPMD set to default: %d \n",this->cpmd);

  if (stf_flag)
    printf ("stf file set to: %s \n ",this->stfName);
  else {
    fprintf(stderr,"Please give a scheduling test input file \n");
    fprintf(stderr, "%s", USAGE);
    exit(EXIT_FAILURE);
  }
    

  if (printTimestamps_flag) {
    this->printTimestamps = true;
    printf("Printing all timestamps \n");
  }

  if (printOverheads_flag) {
    this->printOverheads = true;
    printf("Printing all registerd overheads \n");
  }

  if (printSchedulingTraces_flag) {
    this->printSchedulingTraces = true;
    printf("Printing scheduling traces \n");
  }

  if (printExecutionTimes_flag) {
    this->printExecutionTimes = true;
    printf("Printing execution times \n");
  }
     
  for(int i = optind; i<argc; i++){

    this->rt_task_id[nb_rts] = atoi(argv[i]);
    this->nb_rts ++;
  }
}

CmdlParser::~CmdlParser() {
}

