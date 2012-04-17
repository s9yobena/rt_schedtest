
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include "timestamp.h"
#include <arpa/inet.h>
#include "mapping.h"
#include <pthread.h>
#include <semaphore.h>
#include"schedulability_test.h"

static struct task_set_t task_set;

typedef struct overhead_t buffer_item;
#define BUFFER_SIZE 1

/* #define RAND_DIVISOR 100000000 */
#define TRUE 1

struct context_t {
  pthread_mutex_t mutex;
  sem_t full, empty;
  buffer_item buffer[BUFFER_SIZE];
  int counter;
  pthread_attr_t attr; //Set of thread attributes
};

pthread_t producer_tid;       //Thread ID
pthread_t consumer_tid;       //Thread ID

void initializeData(struct context_t*);

/* Producer Thread */
void *producer(void *param);

/* Consumer Thread */
void *consumer(void *param);
/* Add an item to the buffer */
int insert_item(buffer_item item, struct context_t*);

/* Remove an item from the buffer */
int remove_item(buffer_item *item, struct context_t*);


#define dovrhd 3000
static int cur_ovrhd = 0;



#define MAX_EVENTS 128

static int fd;
static int event_count = 0;
static cmd_t  ids[MAX_EVENTS];
static unsigned long total_bytes = 0;

/* from ft2csv.c */
static int want_interleaved    = 1;
static int want_best_effort    = 0;

/* discard samples from a specific CPU */
static int avoid_cpu = -1;
/* only use samples from a specific CPU */
static int only_cpu  = -1;

static unsigned int complete   = 0;
static unsigned int incomplete = 0;
static unsigned int filtered   = 0;
static unsigned int skipped    = 0;
static unsigned int non_rt     = 0;
static unsigned int interleaved = 0;
static unsigned int avoided    = 0;

#define CYCLES_PER_US 2128

static unsigned long long threshold = CYCLES_PER_US * 10000; /* 10 ms == 10 full ticks */

static struct timestamp* next(struct timestamp* start, struct timestamp* end,
			      int cpu);

static struct timestamp* next_id(struct timestamp* start, struct timestamp* end,
				 int cpu, unsigned long id,
				 unsigned long stop_id);

static struct timestamp* find_second_ts(struct timestamp* start,
					struct timestamp* end);

typedef void (*pair_fmt_t)(struct timestamp* first, struct timestamp* second, struct context_t* );

static void print_pair_csv(struct timestamp* first, struct timestamp* second, struct context_t* context);

/* static void print_pair_bin(struct timestamp* first, struct timestamp* second) */
/* { */
/* 	float delta = second->timestamp - first->timestamp; */
/* 	fwrite(&delta, sizeof(delta), 1, stdout); */
/* } */

pair_fmt_t format_pair = print_pair_csv;

static void show_csv(struct timestamp* first, struct timestamp *end, struct context_t* context);
typedef void (*single_fmt_t)(struct timestamp* ts);

static void print_single_csv(struct timestamp* ts);

/* static void print_single_bin(struct timestamp* ts) */
/* { */
/* 	float delta = ts->timestamp; */

/* 	fwrite(&delta, sizeof(delta), 1, stdout); */
/* } */

single_fmt_t single_fmt = print_single_csv;

/* static void show_single(struct timestamp* ts) */
/* { */
/* 	if (ts->cpu == avoid_cpu || */
/* 	    (only_cpu != -1 && ts->cpu != only_cpu)) { */
/* 		avoided++; */
/* 	} else if (ts->task_type == TSK_RT) { */
/* 		single_fmt(ts); */
/* 		complete++; */
/* 	} else */
/* 		non_rt++; */
/* } */

static void show_id(struct timestamp* start, struct timestamp* end,
		    unsigned long id, struct context_t* context);

/* static void show_single_records(struct timestamp* start, struct timestamp* end, */
/* 				unsigned long id) */
/* { */
/* 	for (; start != end; start++) */
/* 		if (start->event == id) */
/* 			show_single(start); */
/* } */

/* end form ft2csv.c */


static int disable_all(int fd);

static int enable_event(int fd, char* str);

/* static void cat2stdout(int fd) */
/* { */
/* 	static char buf[4096]; */
/* 	int rd; */
/* 	while ((rd = read(fd, buf, 4096)) > 0) { */
/* 		total_bytes += rd; */
/* 		fwrite(buf, 1, rd, stdout); */
/* 	} */
/* } */


static void catcsv2stdout(int fd, struct context_t* context);


/* static void usage(void) */
/* { */
/* 	fprintf(stderr, */
/* 		"Usage: ftcat <ft device> TS1 TS2 ...." */
/* 		"\n"); */
/* 	exit(1); */
/* } */

static void shutdown2(int sig);
int overhead_init();


int main(int argc, char *argv[]) {
   /* Loop counter */
   int i;
   /* int mainSleepTime; */
   struct context_t context;


   /* Verify the correct number of arguments were passed in */
   if(argc < 3) {
      fprintf(stderr, "USAGE:./main.out <INT> <TID> ...\n");
   }

   task_set_init(&task_set);
   
   /* get task set parameters */
   for (i=1;i<argc;i++){
    task_set.rt_task_id[i-1] = atoi(argv[i]);
    get_rt_task_param(task_set.rt_task_id[i-1],  &task_set.rt_task_param[i-1] ); 
    task_set.nb_rts ++;
   }
  
   /* print parameters */
  for (i=0;i<task_set.nb_rts;i++){
    printf("rt_task %d: e = %d, p = %d \n",task_set.rt_task_id[i], 
	   (int)task_set.rt_task_param[i].exec_cost, 
	   (int)task_set.rt_task_param[i].period);
  }

  /* schedulability test */
  if (sched_test(&task_set))
    printf(" Yeah! task set schedulable\n");
  else
    printf("Oh ****, task set is not schedulable\n");


   /* mainSleepTime = atoi(argv[argc-1]); /\* Time in seconds for main to sleep *\/ */
   
   initializeData(&context);
   pthread_create(&producer_tid,&context.attr,producer, (void*)&context);
   pthread_create(&consumer_tid,&context.attr,consumer, (void*)&context);
   
   /* sleep(mainSleepTime); */
   pthread_join(producer_tid,NULL);
   
   return 0;
}

void initializeData(struct context_t* context) {

   pthread_mutex_init(&context->mutex, NULL);
   sem_init(&context->full, 0, 0);
   sem_init(&context->empty, 0, BUFFER_SIZE);
   pthread_attr_init(&context->attr);
   context->counter = 0;
}

void *producer(void *param) {
  
  struct context_t* context = (struct context_t*)param;
  /* TODO: Refactor trace_file */
  char* trace_file;
  trace_file = "/dev/litmus/ft_trace0";  

  overhead_init();
  catcsv2stdout(fd,context);
  close(fd);
  fflush(stdout);
  fprintf(stderr, "%s: %lu bytes read.\n", trace_file, total_bytes);

  return 0;
}

/* Consumer Thread */
void *consumer(void *param) {

  struct context_t* context = (struct context_t*)param;

   buffer_item item;

   while(TRUE) {
      /* sleep for a random period of time */
      /* int rNum = rand() / RAND_DIVISOR; */
      /* sleep(rNum); */

      /* aquire the full lock */
      sem_wait(&context->full);
      /* aquire the mutex lock */
      pthread_mutex_lock(&context->mutex);
      if(remove_item(&item,context)) {
         fprintf(stderr, "Consumer report error condition\n");
      }
      else {
         printf("consumer consumed cxs time: %d\n", item.cxs);

	 /* schedulability test */
	 printf("Online scheduability test: \n");
	 if (ovrhd_sched_test(&task_set, &item))
	   printf("Yeah, task set schedulable\n");
	 else
	   printf("Oh ****, task set is not schedulable\n");
      }
      /* release the mutex lock */
      pthread_mutex_unlock(&context->mutex);
      /* signal empty */
      sem_post(&context->empty);
   }
}

/* Add an item to the buffer */
int insert_item(buffer_item item, struct context_t* context) {
   /* When the buffer is not full add the item
      and increment the counter*/
   if(context->counter < BUFFER_SIZE) {
     context->buffer[context->counter] = item;
     context->counter++;
     return 0;
   }
   else { /* Error the buffer is full */
      return -1;
   }
}

/* Remove an item from the buffer */
int remove_item(buffer_item *item, struct context_t* context) {
   /* When the buffer is not empty remove the item
      and decrement the counter */
   if(context->counter > 0) {
      *item = context->buffer[(context->counter-1)];
      context->counter--;
      return 0;
   }
   else { /* Error buffer empty */
      return -1;
   }
}
/* ********************************************************* */

static struct timestamp* next(struct timestamp* start, struct timestamp* end,
			      int cpu)
{
	struct timestamp* pos;
	unsigned int last_seqno = 0;

	for (pos = start; pos != end;  pos++) {
		/* check for for holes in the sequence number */
		if (last_seqno && last_seqno + 1 != pos->seq_no) {
			/* stumbled across a hole */
			return NULL;
		}
		last_seqno = pos->seq_no;

		if (pos->cpu == cpu)
			return pos;
	}
	return NULL;
}

static struct timestamp* next_id(struct timestamp* start, struct timestamp* end,
				 int cpu, unsigned long id,
				 unsigned long stop_id)
{
	struct timestamp* pos = start;
	int restarts = 0;

	while ((pos = next(pos, end, cpu))) {
		if (pos->event == id)
			break;
		else if (pos->event == stop_id)
			return NULL;
		pos++;
		restarts++;
		if (!want_interleaved)
			return NULL;
	}
	if (pos)
		interleaved += restarts;
	return pos;
}

static struct timestamp* find_second_ts(struct timestamp* start,
					struct timestamp* end)
{
	/* convention: the end->event is start->event + 1 */
	return next_id(start + 1, end, start->cpu, start->event + 1,
		       start->event);
}


static void print_pair_csv(struct timestamp* first, struct timestamp* second, struct context_t* context)
{
	/* printf("%llu, %llu, %llu\n", */
	/*        (unsigned long long) first->timestamp, */
	/*        (unsigned long long) second->timestamp, */
	/*        (unsigned long long) */
	/*        (second->timestamp - first->timestamp)); */
	/* memcpy ((void*)doverhead, (void*)(int )(second->timestamp - first->timestamp), sizeof(unsigned long long)); */
  int doverhead;
  buffer_item item;



  doverhead = (int )(second->timestamp - first->timestamp);
  if( (doverhead > (cur_ovrhd+ dovrhd)) ||
      (doverhead < (cur_ovrhd- dovrhd)) ){
    cur_ovrhd = doverhead;
    printf("context switch overhead is   %d\n",doverhead);
    item.cxs = doverhead;
          /* acquire the empty lock */
      sem_wait(&context->empty);
      /* acquire the mutex lock */
      pthread_mutex_lock(&context->mutex);

      if(insert_item(item,context)) {
         fprintf(stderr, " Producer report error condition\n");
      }
      else {
         printf("producer produced cxs time: %d\n", item.cxs);
      }
      /* release the mutex lock */
      pthread_mutex_unlock(&context->mutex);
      /* signal full */
      sem_post(&context->full);

  }

}

/* static void print_pair_bin(struct timestamp* first, struct timestamp* second) */
/* { */
/* 	float delta = second->timestamp - first->timestamp; */
/* 	fwrite(&delta, sizeof(delta), 1, stdout); */
/* } */

/* pair_fmt_t format_pair = print_pair_csv; */

static void show_csv(struct timestamp* first, struct timestamp *end, struct context_t* context)
{
	struct timestamp *second;

	if (first->cpu == avoid_cpu ||
	    (only_cpu != -1 && first->cpu != only_cpu)) {
		avoided++;
		return;
	}

	second = find_second_ts(first, end);
	if (second) {
		if (second->timestamp - first->timestamp > threshold)
			filtered++;
		else if (first->task_type != TSK_RT &&
			 second->task_type != TSK_RT && !want_best_effort)
			non_rt++;
		else {
		        /* printf("printed anothercsv \n");  */
		  format_pair(first, second, context);
		  /* print_pair_csv(first, second, context); */
			complete++;
		}
	} else
		incomplete++;
}


static void print_single_csv(struct timestamp* ts)
{
	printf("0, 0, %llu\n",
	       (unsigned long long) (ts->timestamp));
}

/* static void print_single_bin(struct timestamp* ts) */
/* { */
/* 	float delta = ts->timestamp; */

/* 	fwrite(&delta, sizeof(delta), 1, stdout); */
/* } */


/* static void show_single(struct timestamp* ts) */
/* { */
/* 	if (ts->cpu == avoid_cpu || */
/* 	    (only_cpu != -1 && ts->cpu != only_cpu)) { */
/* 		avoided++; */
/* 	} else if (ts->task_type == TSK_RT) { */
/* 		single_fmt(ts); */
/* 		complete++; */
/* 	} else */
/* 		non_rt++; */
/* } */

static void show_id(struct timestamp* start, struct timestamp* end,
		    unsigned long id, struct context_t* context)
{
	while (start !=end && start->event != id + 1) {
		skipped++;
		start++;
	}

	for (; start != end; start++)
		if (start->event == id)
		  show_csv(start, end, context);
}

/* static void show_single_records(struct timestamp* start, struct timestamp* end, */
/* 				unsigned long id) */
/* { */
/* 	for (; start != end; start++) */
/* 		if (start->event == id) */
/* 			show_single(start); */
/* } */

/* end form ft2csv.c */


static int disable_all(int fd)
{
	int disabled = 0;
	int i;

	fprintf(stderr, "Disabling %d events.\n", event_count);
	for (i = 0; i < event_count; i++)
		if (ioctl(fd, DISABLE_CMD, ids[i]) < 0)
			perror("ioctl(DISABLE_CMD)");
		else
			disabled++;

	return  disabled == event_count;
}

static int enable_event(int fd, char* str)
{
	cmd_t   *id;
	int err;

	id = ids + event_count;
	if (!str2event(str, id)) {
		errno = EINVAL;
		return 0;
	}
	event_count += 1;

	err = ioctl(fd, ENABLE_CMD, *id);

	if (err < 0)
		printf("ioctl(%d, %d, %d) => %d (errno: %d)\n", fd, (int) ENABLE_CMD, *id,
		       err, errno);

	return err == 0;
}


/* static void cat2stdout(int fd) */
/* { */
/* 	static char buf[4096]; */
/* 	int rd; */
/* 	while ((rd = read(fd, buf, 4096)) > 0) { */
/* 		total_bytes += rd; */
/* 		fwrite(buf, 1, rd, stdout); */
/* 	} */
/* } */


static void catcsv2stdout(int fd, struct context_t* context)
{
	static char buf[/* 4096 */sizeof(struct timestamp) ];
	int rd;
	
	/* from ft2csv.c */
	/* void* mapped; */
	size_t size, count;
	struct timestamp *ts, *end;
	cmd_t id;
	/* int opt; */
	/* char event_name[80]; */

	/* id for CXS_START */
	id = 104;

	while ((rd = read(fd, buf, 4096)) > 0) {
		total_bytes += rd;
		size = rd;
		/* fwrite(buf, 1, rd, stdout); */
		ts    = (struct timestamp*) buf;
		count = size / sizeof(struct timestamp);
		end   = ts + count;
		show_id(ts, end, id,context);

		
	}
}


/* static void usage(void) */
/* { */
/* 	fprintf(stderr, */
/* 		"Usage: ftcat <ft device> TS1 TS2 ...." */
/* 		"\n"); */
/* 	exit(1); */
/* } */

static void shutdown2(int sig)
{
	int ok;
	ok = disable_all(fd);
	if (!ok)
		fprintf(stderr, "disable_all: %m\n");
	exit(0);
}

int overhead_init() {
  	 char* trace_file;
	 char* trace_event[2];
	 int nb_trace_events;
	int ec;

	trace_file = "/dev/litmus/ft_trace0";
	fd = open(trace_file, O_RDWR);
	if (fd < 0) {
		perror("could not open feathertrace");
		return 1;
	}
	
	trace_event[0]="CXS_START";
	trace_event[1]="CXS_END";
	nb_trace_events = 2;
	
	signal(SIGINT, shutdown2);
	signal(SIGUSR1, shutdown2);
	signal(SIGTERM, shutdown2);

	for (ec=0;ec<nb_trace_events;ec++) {
	  	if (!enable_event(fd, trace_event[ec])) {
			fprintf(stderr, "Enabling %s failed: %m\n",trace_event[ec] );
			return 2;
		}
	
	}

	return 0;

}

