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

ifndef GP_DEBUG
 	GP_DEBUG := 1
endif

.PHONY: all
all: lib bin

include $(G4INSTALL)/config/binmake.gmk
G4BINDIR := ./bin/




