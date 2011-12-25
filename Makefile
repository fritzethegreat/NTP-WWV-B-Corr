#
# Makefile for crsCorr library
#
# Modified: 07/18/10
# Notes:    Recreated...after overwrite...
#

# Contains common definitions for other make files
include common.make

# Main objectives for all
all: objects tests lib

# Include other files
include $(SRC_DIR)/Makefile
include $(TEST_DIR)/Makefile
include $(LIB_DIR)/Makefile

# Actually install the library
install:
	cp ./lib/libCrsCorr.so /usr/local/lib/