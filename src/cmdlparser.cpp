#include "cmdlparser.h"

CmdlParser::CmdlParser(int argc, char **argv) {

  this->nb_rts = 0;
  this->cpmd = 5000;

  

  int c;
  int cpmd_flag = 0;
  while (1)
    {
      static struct option long_options[] =
	{
	  {"cpmd", required_argument,&cpmd_flag,1 },
	  {0, 0, 0, 0}
	};
      /* getopt_long stores the option index here. */
      int option_index = 0;
     
      c = getopt_long (argc, argv, "", long_options, &option_index);
     
      /* Detect the end of the options. */
      if (c == -1)
	break;
      switch (c)
	{
	case 0:
	  if (optarg) {
	    this->cpmd = atoi(optarg);
	  }
	  break;
	case '?':
	  /* getopt_long already printed an error message. */
	  break;
	default:
	  fprintf(stderr, "%s", USAGE);
	  exit(1);
	}
    }

  if (argc - optind < 1){
    fprintf(stderr, "%s", USAGE);
    exit(1);
  }


  if (cpmd_flag)
    printf ("CPMD set to: %d \n ",this->cpmd);
  else
    printf ("CPMD set to default: %d \n",this->cpmd);
     
  for(int i = optind; i<argc; i++){

    this->rt_task_id[nb_rts] = atoi(argv[i]);
    this->nb_rts ++;
  }
}

CmdlParser::~CmdlParser() {
}

