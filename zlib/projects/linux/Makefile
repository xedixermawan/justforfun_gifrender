GXX=/usr/bin/g++ -m32
GCC=/usr/bin/gcc -m32
CWD=$(shell pwd)

ROOT_PATH=$(CWD)/../..

RANLIB=$(shell which ranlib)

LIBRARY_TARGET=libzlib.a

SRC_PATH=$(ROOT_PATH)/src
OUTPUT_PATH=$(ROOT_PATH)/lib

LIBRARIES=  

SHELL=/bin/sh
INCLUDES=-I$(ROOT_PATH)/include

DEFINES=

CXXFLAGS=-Wall -w -ggdb $(DEFINES)
LDFLAGS=

ALL_OBJECTS =   $(SRC_PATH)/adler32.o	\
				$(SRC_PATH)/compress.o \
				$(SRC_PATH)/crc32.o \
				$(SRC_PATH)/deflate.o \
				$(SRC_PATH)/gzio.o \
				$(SRC_PATH)/infback.o \
				$(SRC_PATH)/inffast.o \
				$(SRC_PATH)/inflate.o \
				$(SRC_PATH)/inftrees.o \
				$(SRC_PATH)/trees.o \
				$(SRC_PATH)/uncompr.o \
				$(SRC_PATH)/zutil.o

.SUFFIXES: .c .o

%.o : %.c
	@echo [Building] $(subst $(SRC_PATH)/,$(empty),$<)
	@$(GCC) $(INCLUDES) $(CXXFLAGS) -c $< -o $@ -pg

all : $(LIBRARY_TARGET)
	@echo Done !

$(LIBRARY_TARGET) : $(ALL_OBJECTS)
	@$(AR) rv $(OUTPUT_PATH)/$(LIBRARY_TARGET) $?
	@$(RANLIB) $(OUTPUT_PATH)/$(LIBRARY_TARGET)

clean :
	@echo Cleaning...
	@-$(RM) -f $(LIBRARY_TARGET) $(ALL_OBJECTS)
	@echo Done !
force_look :
	true
