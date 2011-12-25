#include <crsCorr/aceMagParser.h>
#include <string.h>

int main() {

  const string dataFile1 = "/mnt/storage/data/20100727_ace_mag_1m.txt";
  AceMagParser first( dataFile1 );

  const string dataFile2 = "/mnt/storage/data/20100728_ace_mag_1m.txt";
  AceMagParser second( dataFile2 );

  const FileParser::DataTag aceMagData =
    { "BX", DATATYPE_DOUBLE, RESOLUTION_IGNORE, START_TIME_IGNORE };

  const DataSeries<double>* firstSeries = first.getSeries<double>( &aceMagData );
  const DataSeries<double>* secondSeries = second.getSeries<double>( &aceMagData );

  std::cout << "Length: " << firstSeries->getLength() << std::endl;
  std::cout << "Length: " << secondSeries->getLength() << std::endl;
  DataSeries<double> combined = *firstSeries;
  std::cout << "Length: " << combined.getLength() << std::endl;
  combined += *secondSeries;

  return 0;
}