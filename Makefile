# Makefile copied from ft_tools

# ##############################################################################
# User variables

# user variables can be specified in the environment or in a .config file
-include .config

# Where is the LITMUS^RT userspace library source tree?
LIBLITMUS ?= ../liblitmus

# Include default configuration from liblitmus
# Liblitmus must have been built before ft_tools can be built.
include ${LIBLITMUS}/inc/config.makefile

# Use g++ instead of gcc and remove the -Wdeclaration-after-statement
CC             = /usr/bin/g++
CFLAGS         = -Wall -Werror -g

# all sources
vpath %.c src/
vpath %.cpp src/

# local include files
CPPFLAGS += -Iinclude/

# ##############################################################################
# Targets

all = rt_schedtest rt_trace-daemon

.PHONY: all clean TAGS
all: ${all}
clean:
	rm -f ${all} *.o *.d
	rm -f TAGS


obj-rt_schedtest = rt_schedtest.o cmdlparser.o taskset.o overhead.o schedtest.o litmusdaemon.o litmustracingpacket.o 

obj-rt_trace-daemon = rt_trace-daemon.o litmusdevice.o litmusoverhead.o timestampprocessor.o  timestamp.o litmustimestamp.o singlelitmustimestamp.o pairlitmustimestamp.o sendreschedlitmustimestamp.o schedtrace.o litmusschedulingtrace.o schedulingtraceprocessor.o litmusschedulingtracerecord.o litmusexecutiontime.o litmusinterarrivaltime.o litmustracingpacket.o packet2stdout.o

rt_schedtest: ${obj-rt_schedtest}

rt_trace-daemon: ${obj-rt_trace-daemon}

# Emacs Tags
TAGS:
	@echo TAGS
	@find . -regex ".*\.[cChH]\(pp\)?" -print | etags -



# dependency discovery
include ${LIBLITMUS}/inc/depend.makefile
