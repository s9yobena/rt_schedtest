# Copied from liblitmus and modified

ARCH           = i386                
CFLAGS         = -Wall -Werror -g
LDFLAGS        = -m32 -fopenmp                
LDLIBS         = -L${LIBLITMUS} -llitmus
CPPFLAGS       = -D_XOPEN_SOURCE=600 -D_GNU_SOURCE -m32 -DARCH=i386 \
		 -I${LIBLITMUS}/include -I${LIBLITMUS}/arch/x86/include \
		 -Iinclude/ -fopenmp
CC             = /usr/bin/g++
LD             = /usr/bin/ld         
AR             = /usr/bin/ar         
