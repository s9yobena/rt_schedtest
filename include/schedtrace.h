#ifndef SCHED_TRACE_H
#define SCHED_TRACE_H

#include <inttypes.h>

typedef uint32_t cmd_t;

typedef uint8_t u8;
typedef uint16_t u16 ;
typedef uint32_t u32 ;
typedef uint64_t u64 ;

typedef unsigned long long exec_time_t;
typedef u8 ster_t;
typedef u16 task_id_t;

struct st_trace_header {
	u8	type;		/* Of what type is this record?  */
	u8	cpu;		/* On which CPU was it recorded? */
	u16	pid;		/* PID of the task.              */
	u32	job;		/* The job sequence number.      */
};

struct st_param_data {		/* regular params */
	u32	wcet;
	u32	period;
	u32	phase;
	u8	partition;
	u8	_class;
	u8	__unused[2];
};

#define STER_RELEASE 3

struct st_release_data {	/* A job is was/is going to be released. */
	u64	release;	/* What's the release time?              */
	u64	deadline;	/* By when must it finish?		 */
};


struct st_switch_to_data {	/* A process was switched to on a given CPU.   */
	u64	when;		/* When did this occur?                        */
	u32	exec_time;	/* Time the current job has executed.          */
	u8	__unused[4];

};

struct st_switch_away_data {	/* A process was switched away from on a given CPU. */
	u64	when;
	u64	exec_time;
};

#define STER_COMPLETION 7

struct st_completion_data {	/* A job completed. */
	u64	when;
	u8	forced:1; 	/* Set to 1 if job overran and kernel advanced to the
				 * next task automatically; set to 0 otherwise.
				 */
	u8	__uflags:7;
	u8	__unused[7];
};

typedef enum {
        ST_NAME = 1,		/* Start at one, so that we can spot
				 * uninitialized records. */
	ST_PARAM,
	ST_RELEASE,
	ST_ASSIGNED,
	ST_SWITCH_TO,
	ST_SWITCH_AWAY,
	ST_COMPLETION,
	ST_BLOCK,
	ST_RESUME,
	ST_ACTION,
	ST_SYS_RELEASE
} st_event_record_type_t;



struct st_event_record {
  struct st_trace_header hdr;
  union {
    u64 raw[2];

    struct st_param_data param;
    struct st_release_data release;
    struct st_switch_to_data switch_to;
    struct st_switch_away_data switch_away;
    struct st_completion_data completion;
  } data;
};


#endif
