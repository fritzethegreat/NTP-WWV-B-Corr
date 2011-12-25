#
# Makefile for crsCorr containing all of the "global variables"
#
# Modified:   07/10/10
# Notes:      Initial creation
#

ROOT_DIR      	:= $(shell pwd)
SRC_DIR       	:= $(ROOT_DIR)/src
TEST_DIR      	:= $(ROOT_DIR)/tests
INCLUDE_DIR   	:= $(ROOT_DIR)/include
LIB_DIR       	:= $(ROOT_DIR)/lib
BIN_DIR       	:= $(ROOT_DIR)/bin
LIB_INSTALL_DIR := /usr/local/lib
CC_LIBINCLUDE 	:= 
CC_INCLUDE    	:= -I$(INCLUDE_DIR)
CC_FLAGS      	:= -fPIC -Wall $(CC_LIBINCLUDE) $(CC_INCLUDE)


VPATH = $(SRC_DIR):$(INCLUDE_DIR):$(TEST_DIR)