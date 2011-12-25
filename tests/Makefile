#
# Makefile for crsCorr tests
#  <COMMENT TESTS HERE>
#
# Modified:   07/29/10
# Notes:      --Updated to include object files as opposed to .cpp's
#             --Included dataSeries.o now that there's a separate object file
#
# Modified:   07/13/10
# Notes:      --Initial creation
#

tests: testDataSeries testParsers testCrsCorr

testCrsCorr: abstractDataSeries.o \
						 clkStatsParser.o \
						 aceMagParser.o \
						 fileParser.o \
						 $(INCLUDE_DIR)/global.h \
						 $(INCLUDE_DIR)/crsCorr.h \
						 $(TEST_DIR)/test_crsCorr.cpp
	g++ -g -o $(TEST_DIR)/testCrsCorr $(CC_FLAGS) $(CXX_FLAGS) \
		$(TEST_DIR)/test_crsCorr.cpp \
		$(SRC_DIR)/clkStatsParser.o \
		$(SRC_DIR)/aceMagParser.o \
		$(SRC_DIR)/fileParser.o \
		$(SRC_DIR)/abstractDataSeries.o

testDataSeries: abstractDataSeries.o \
								$(TEST_DIR)/test_dataSeries.cpp \
								$(INCLUDE_DIR)/dataSeries.h \
								$(INCLUDE_DIR)/global.h
	g++ -g -o $(TEST_DIR)/testDataSeries $(CC_FLAGS) $(CXX_FLAGS) \
		$(TEST_DIR)/test_dataSeries.cpp \
		$(SRC_DIR)/abstractDataSeries.o

testParsers: abstractDataSeries.o \
						 fileParser.o \
						 aceMagParser.o \
						 aceSweParser.o \
						 clkStatsParser.o \
						 gpMagParser.o \
						 gpXrayParser.o \
						 gsMagParser.o \
						 gsPartParser.o \
						 gpPartParser.o \
						 $(INCLUDE_DIR)/global.h \
						 $(INCLUDE_DIR)/dataSeries.h \
						 $(TEST_DIR)/test_parsers.cpp
	g++ -g -o $(TEST_DIR)/testParsers $(CC_FLAGS) $(CXX_FLAGS) \
		$(TEST_DIR)/test_parsers.cpp \
		$(SRC_DIR)/abstractDataSeries.o \
		$(SRC_DIR)/fileParser.o \
		$(SRC_DIR)/aceMagParser.o \
		$(SRC_DIR)/aceSweParser.o \
	  $(SRC_DIR)/clkStatsParser.o \
		$(SRC_DIR)/gpMagParser.o \
    $(SRC_DIR)/gpXrayParser.o \
		$(SRC_DIR)/gsMagParser.o \
		$(SRC_DIR)/gpPartParser.o \
		$(SRC_DIR)/gsPartParser.o
