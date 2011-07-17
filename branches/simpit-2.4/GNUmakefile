# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := simpit
G4TARGET := $(name)
G4EXLIB := true
G4FOTDIR = $(G4GlobalWorkDir)/G4Fot


CPPFLAGS += -I$(G4FOTDIR)/include -D BOOST_FILESYSTEM_VERSION=2
EXTRALIBS += -L$(G4FOTDIR)/lib -lG4Fot
EXTRA_LINK_DEPENDENCIES := $(G4FOTDIR)/lib/libG4Fot.so



ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all   
all: lib bin 

include $(G4INSTALL)/config/binmake.gmk

 $(G4FOTDIR)/lib/libG4Fot.so:
	cd $(G4FOTDIR); $(MAKE)
#
G4BIN = ./bin
G4BINDIR = ./bin
G4LIB = ./lib
G4TMP = ./tmp
#LDFLAGS += -lboost_filesystem -lboost_system
LDFLAGS += -lboost_filesystem
#For debug
#CPPFLAGS += -DGP_DEBUG 
CPPFLAGS += -g
