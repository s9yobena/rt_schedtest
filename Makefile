# Makefile copied from ft_tools and modified

# ##############################################################################
# User variables

# user variables can be specified in the environment or in a .config file
# -include .config

# Where is the LITMUS^RT userspace library source tree?
LIBLITMUS ?= ../liblitmus

# Include default configuration from liblitmus
# Liblitmus must have been built before rt_schedtest can be built.
include ${LIBLITMUS}/inc/config.makefile

# Set our configuration 
CFLAGS     = -Wall -Werror -g
LDFLAGS    +=  -fopenmp  	
CPPFLAGS  += -Iinclude/ -fopenmp -g -Wall
CC         = /usr/bin/g++



# all sources
vpath %.c src/
vpath %.cpp src/


# ##############################################################################
# Targets

all = rt_schedtest rt_global-test rt_partitionned-test rt_clustered-test rt_trace-daemon 


.PHONY: all clean TAGS
all: ${all}
clean:
	rm -f ${all} *.o *.d
	rm -f TAGS


obj-rt_schedtest = rt_schedtest.o configschedtest.o cmdlparser.o

obj-rt_global-test = rt_global-test.o schedtest.o globaltest.o cmdlparser.o taskset.o task.o overhead.o schedtest.o schedtestparam.o schedtrace.o cachetop.o

obj-rt_partitionned-test = rt_partitionned-test.o schedtest.o partitionnedtest.o cmdlparser.o taskset.o task.o overhead.o schedtest.o schedtestparam.o schedtrace.o cachetop.o

obj-rt_clustered-test = rt_clustered-test.o schedtest.o clusteredtest.o globaltest.o  partitionnedtest.o cmdlparser.o taskset.o task.o overhead.o schedtest.o schedtestparam.o schedtrace.o cachetop.o

obj-rt_trace-daemon = rt_trace-daemon.o taskset.o task.o litmusdevice.o litmusoverhead.o timestampprocessor.o  timestamp.o litmustimestamp.o singlelitmustimestamp.o pairlitmustimestamp.o ticklitmustimestamp.o sendreschedlitmustimestamp.o schedtrace.o litmusschedulingtrace.o schedulingtraceprocessor.o litmusschedulingtracerecord.o litmusexecutiontime.o litmusinterarrivaltime.o litmusselfsuspension.o schedtestparam.o litmusschedtest.o safetymargin.o cachetop.o cpuclock.o

rt_schedtest: ${obj-rt_schedtest}

rt_global-test: ${obj-rt_global-test}

rt_partitionned-test: ${obj-rt_partitionned-test}

rt_clustered-test: ${obj-rt_clustered-test}

rt_trace-daemon: ${obj-rt_trace-daemon}

# Emacs Tags
TAGS:
	@echo TAGS
	@find . -regex ".*\.[cChH]\(pp\)?" -print | etags -
