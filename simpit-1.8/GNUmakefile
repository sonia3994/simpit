# $Id: GNUmakefile,v 1.1 1999/01/07 16:05:40 gunter Exp $
# --------------------------------------------------------------
# GNUmakefile for examples module.  Gabriele Cosmo, 06/04/98.
# --------------------------------------------------------------

name := GammaPair
G4TARGET := $(name)
G4EXLIB := true

ifndef G4INSTALL
  G4INSTALL = ../../..
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk

G4BINDIR = ./bin
#G4TMPDIR = ./tmp

LDFLAGS += -lboost_filesystem -lboost_system
#LDFLAGS += -lboost_filesystem

#For debug
#CPPFLAGS += -DGP_DEBUG 
#CPPFLAGS += -g

