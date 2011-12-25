#
# Makefile for crsCorr
#
# Modified:   09/02/10
# Notes:			Made additions for particle parsers for both the GOES Satellites
#
# Modified:   08/12/10
# Notes:      Updated to compile data series from abstract data series
#
# Modified:   08/11/10
# Notes:      Complete rewrite due to conversion of DataSeries to template class
#             Restructured make file order in main make file
#
# Modified:		07/16/10
# Notes:			Modified to include cross correlation class
#
# Modified:		07/15/10
# Notes:		  Modified to include data series, file parser, and iono file
#             parser
#
# Modified:   07/08/10
# Notes:      Initial creation
#

objects: abstractDataSeries.o fileParser.o aceMagParser.o aceSweParser.o \
				 clkStatsParser.o gsMagParser.o gpMagParser.o gpXrayParser.o \
				 gpPartParser.o gsPartParser.o

abstractDataSeries.o: $(SRC_DIR)/abstractDataSeries.cpp \
											$(INCLUDE_DIR)/global.h \
											$(INCLUDE_DIR)/abstractDataSeries.h
	g++ -g -c -o $(SRC_DIR)/abstractDataSeries.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/abstractDataSeries.cpp

aceMagParser.o:	abstractDataSeries.o \
								fileParser.o \
								$(INCLUDE_DIR)/global.h \
								$(INCLUDE_DIR)/dataSeries.h \
								$(SRC_DIR)/aceMagParser.cpp \
								$(INCLUDE_DIR)/aceMagParser.h
	g++ -g -c -o $(SRC_DIR)/aceMagParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/aceMagParser.cpp

aceSweParser.o:	abstractDataSeries.o \
								fileParser.o \
								$(INCLUDE_DIR)/global.h \
								$(INCLUDE_DIR)/dataSeries.h \
								$(SRC_DIR)/aceSweParser.cpp \
								$(INCLUDE_DIR)/aceSweParser.h
	g++ -g -c -o $(SRC_DIR)/aceSweParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/aceSweParser.cpp

clkStatsParser.o:	abstractDataSeries.o \
									fileParser.o \
									$(INCLUDE_DIR)/global.h \
									$(INCLUDE_DIR)/dataSeries.h \
									$(SRC_DIR)/clkStatsParser.cpp \
									$(INCLUDE_DIR)/clkStatsParser.h
	g++ -g -c -o $(SRC_DIR)/clkStatsParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/clkStatsParser.cpp

fileParser.o:	abstractDataSeries.o \
							$(INCLUDE_DIR)/global.h \
							$(INCLUDE_DIR)/dataSeries.h \
							$(SRC_DIR)/fileParser.cpp \
							$(INCLUDE_DIR)/fileParser.h
	g++ -g -c -o $(SRC_DIR)/fileParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/fileParser.cpp

gpMagParser.o: abstractDataSeries.o \
							 $(INCLUDE_DIR)/global.h \
							 $(INCLUDE_DIR)/dataSeries.h \
							 fileParser.o \
							 $(SRC_DIR)/gpMagParser.cpp \
							 $(INCLUDE_DIR)/gpMagParser.h
	g++ -g -c -o $(SRC_DIR)/gpMagParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/gpMagParser.cpp

gpPartParser.o: abstractDataSeries.o \
							 $(INCLUDE_DIR)/global.h \
							 $(INCLUDE_DIR)/dataSeries.h \
							 fileParser.o \
							 $(SRC_DIR)/gpPartParser.cpp \
							 $(INCLUDE_DIR)/gpPartParser.h
	g++ -g -c -o $(SRC_DIR)/gpPartParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/gpPartParser.cpp

gpXrayParser.o:	abstractDataSeries.o \
  							$(INCLUDE_DIR)/global.h \
								$(INCLUDE_DIR)/dataSeries.h \
								fileParser.o \
								$(SRC_DIR)/gpXrayParser.cpp \
								$(INCLUDE_DIR)/gpXrayParser.h
	g++ -g -c -o $(SRC_DIR)/gpXrayParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/gpXrayParser.cpp

gsMagParser.o: abstractDataSeries.o \
							 fileParser.o \
  						 $(INCLUDE_DIR)/global.h \
							 $(INCLUDE_DIR)/dataSeries.h \
							 $(SRC_DIR)/gsMagParser.cpp \
							 $(INCLUDE_DIR)/gsMagParser.h
	g++ -g -c -o $(SRC_DIR)/gsMagParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/gsMagParser.cpp

gsPartParser.o: abstractDataSeries.o \
							  fileParser.o \
  						  $(INCLUDE_DIR)/global.h \
							  $(INCLUDE_DIR)/dataSeries.h \
							  $(SRC_DIR)/gsPartParser.cpp \
							  $(INCLUDE_DIR)/gsPartParser.h
	g++ -g -c -o $(SRC_DIR)/gsPartParser.o $(CC_FLAGS) $(CXX_FLAGS) \
		$(SRC_DIR)/gsPartParser.cpp
