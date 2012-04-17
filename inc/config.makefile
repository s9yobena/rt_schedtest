ARCH           = i386                
CFLAGS         = -Wall -Werror -g # -Wdeclaration-after-statement
LDFLAGS        = -m32                
LDLIBS         = -L${LIBLITMUS} -llitmus -lpthread
CPPFLAGS       = -D_XOPEN_SOURCE=600 -D_GNU_SOURCE -m32 -DARCH=i386 -I${LIBLITMUS}/include -I${LIBLITMUS}/arch/x86/include
CC             = /usr/bin/g++
LD             = /usr/bin/ld         
AR             = /usr/bin/ar         
