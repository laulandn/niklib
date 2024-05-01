
LIBNAME=$(shell basename $(shell pwd))
LDNAME=lib$(LIBNAME)_static.a

####################################################################
# Makefile
####################################################################

PATH_TO_BASE=.

include $(PATH_TO_BASE)/../Makefile.target
include $(PATH_TO_BASE)/../Makefile.common

EXTRA_FLAGS=-DNIK_GUILIB -DNICK_USE_DECODE3D -DNICK_USE_NICK3D -DNIK_USE_NIK3D
#EXTRA_FLAGS=-DNIK_NLIB -DNICK_USE_DECODE3D -DNICK_USE_NICK3D -DNIK_USE_NIK3D
#EXTRA_FLAGS=-DNIK_SDL -DNICK_USE_DECODE3D -DNICK_USE_NICK3D -DNIK_USE_NIK3D

#EXTRA_INCS=-I$(PATH_TO_BASE) -I..

EXTRA_LIBS=$(CPPLIB_L)
#EXTRA_LIBS=$(NLIB_L)


####################################################################
# Our objects 
####################################################################

DIRS=$(wildcard */)
SRCSCPP=$(wildcard nik*.cpp)
SRCSC=$(wildcard nik*.c)
OBJSCPP=$(patsubst %.cpp, %.o,$(SRCSCPP))
OBJSC=$(patsubst %.c, %.o,$(SRCSC))
OBJS=$(OBJSCPP) $(OBJSC)


####################################################################
# The libraries and programs 
####################################################################

SIMPLE_PROGRAMS1=nlines nv
SIMPLE_PROGRAMS2=nwire
SIMPLE_PROGRAMS3=
SIMPLE_PROGRAMS=$(SIMPLE_PROGRAMS1) $(SIMPLE_PROGRAMS2) $(SIMPLE_PROGRAMS3)
PROGRAMS=$(SIMPLE_PROGRAMS)


PATH_TO_NLIB=../../more_src/nlib
PATH_TO_CPP=../cpp
PATH_TO_SRC=..


NIKLIB_NAME=niklib_static
NIKLIB_L=-l$(NIKLIB_NAME)
NIKLIB_LIB=lib$(NIKLIB_NAME).$(AFILE)
NIKLIB_PATH=$(PATH_TO_CPP)/$(NIKLIB_LIB)


# Need for alib and guilib (if using it)
PATH_TO_CPP=../cpp
#
CPPLIB_NAME=cpp_static
CPPLIB_L=-L$(PATH_TO_CPP) -l$(CPPLIB_NAME)
CPPLIB_LIB=lib$(CPPLIB_NAME).$(AFILE)
CPPLIB_PATH=$(PATH_TO_CPP)/$(CPPLIB_LIB)
#
CPP_PATHS=$(CPPLIB_PATH)
CPP_LS=$(CPPLIB_L)


# Need for nlib (if using it)
PATH_TO_NLIB=../../more_src/nlib
#
NLIB_NAME=nlib_static
NLIB_L=-L$(PATH_TO_NLIB) -l$(NLIB_NAME)
NLIB_LIB=lib$(NLIB_NAME).$(AFILE)
NLIB_PATH=$(PATH_TO_NLIB)/$(NLIB_LIB)
#
NLIB_PATHS=$(NLIB_PATH)
NLIBLS=$(NLIB_L)


DECODEIMG_NAME=decodeimg_static
DECODEIMG_L=-Ldecodeimg -l$(DECODEIMG_NAME)
DECODEIMG_LIB=lib$(DECODEIMG_NAME).$(AFILE)
DECODEIMG_PATH=$(PATH_TO_CPP)/decodeimg/$(DECODEIMG_LIB)

DECODERLIB_NAME=decoderlib_static
DECODERLIB_L=-Ldecoderlib -l$(DECODERLIB_NAME)
DECODERLIB_LIB=lib$(DECODERLIB_NAME).$(AFILE)
DECODERLIB_PATH=$(PATH_TO_CPP)/decoderlib/$(DECODERLIB_LIB)


DECODE3D_NAME=decode3d_static
DECODE3D_L=-L../decode3d -l$(DECODE3D_NAME)
DECODE3D_LIB=lib$(DECODE3D_NAME).$(AFILE)
DECODE3D_PATH=$(PATH_TO_CPP)/../decode3d/$(DECODE3D_LIB)

NICK3D_NAME=nick3d_static
NICK3D_L=-L../nick3d -l$(NICK3D_NAME)
NICK3D_LIB=lib$(NICK3D_NAME).$(AFILE)
NICK3D_PATH=$(PATH_TO_CPP)/../nick3d/$(NICK3D_LIB)


ifeq ($(MSVC),yes)
	NIKLIB_L=$(NIKLIB_PATH)
endif


####################################################################
# The targets 
####################################################################

all:
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) all;)
	$(MAKE) $(LDNAME)
	$(MAKE) -i $(PROGRAMS)


all_libs: base_libs


base_libs:


$(LDNAME):	$(OBJS) $(OTHEROBJS)
	$(AR_CMD) $(OBJS) $(OTHEROBJS)
	$(RANLIB_CMD) $(LDNAME) 


clean:
	rm -rf debug.html */debug.html */debug.htm debug.txt */debug.txt ndebug.txt */ndebug.txt adebug.txt */adebug.txt *.o *.obj *.OBJ *.lib *.a *.s *.bak *.log map.f build *Data *.pdb *.lib *.bak $(PROGRAMS) *.html *.exe *.EXE *.exe.so *68k *.SYM .DS_Store *.ilk *.app *.class .gdb_history .nautilus-metafile.xml */*Data */Debug */*.ncb */*/Debug Debug */build resource.frk RESOURCE.FRK finder.dat FINDER.DAT ._* */._* */*/._* */*/*/._* *~ */*~ */*/*~ */*/*/*~
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) clean;)


fix: fix_perm fix_type
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) fix;)


# The different Libraries and Programs...

nlines: base_libs nlines.$(OFILE) $(LDNAME)
	$(CLXX) -o nlines nlines.$(OFILE) $(PATH_TO_NLIB)/nlibdbug.o $(NIKLIB_L) $(LIBS) $(LDFLAGS)
	#rm -rf $@.app
	#make $@.app

nv: base_libs $(SOUNDLIB_PATH) nv.$(OFILE) $(LDNAME)
	$(CLXX) -o nv nv.$(OFILE) $(PATH_TO_NLIB)/nlibdbug.o $(SOUNDLIB_L) $(NIKLIB_L) $(LIBS) $(LDFLAGS)
	#rm -rf $@.app
	#make $@.app

nwire: base_libs nwire.$(OFILE) $(LDNAME)
	$(CLXX) -o nwire nwire.$(OFILE) $(PATH_TO_NLIB)/nlibdbug.o $(NICK3D_L) $(DECODE3D_L) $(NIKLIB_L) $(LIBS) $(LDFLAGS)
	#rm -rf $@.app
	#make $@.app


# dependencies
# NOTE: it's ok to see errors when running makedepend
depend:
	makedepend $(DEPENDFLAGS) *.c
	makedepend $(DEPENDFLAGS) *.cpp
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -i -C $(DIR) depend;)


remove_depends:
	rm -f Makefile.dep
	touch Makefile.dep
# Libraries...
	$(foreach DIR,$(DIRS),$(MAKE) -ik -C $(DIR) remove_depends;)


include $(PATH_TO_BASE)/Makefile.tail

include Makefile.dep


# DO NOT DELETE
