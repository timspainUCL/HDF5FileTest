#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <catch2/catch.hpp>

#include "Hdf5File.h"

const std::string testFileName = "test.hdf5";

Hdf5File& createTestFile(Hdf5File& file) {
  file.create(testFileName);
  return file;
}

TEST_CASE( "Test file creation", "[HDF5basics]" )
{
  Hdf5File file;
  createTestFile(file);
  file.close();
  REQUIRE( Hdf5File::canAccess(testFileName) );
}

TEST_CASE( "Test group creation", "[HDF5basics]" )
{
  Hdf5File file;
  createTestFile(file);
  hid_t root = file.getRootGroup();
  std::string groupName = "A group";
  hid_t groupId = file.createGroup(root, groupName);
  file.closeGroup(groupId);
  file.close();

  file.open(testFileName);
  root = file.getRootGroup();
  groupId = file.openGroup(root, groupName);
  try {
    file.readGroupInfo(groupId);
    REQUIRE(true);
  } catch (...) {
    REQUIRE(false);
  }
  file.closeGroup(groupId);
  file.close();
}

TEST_CASE( "Test dataset creation", "[HDF5basics]" )
{
  Hdf5File file;
  createTestFile(file);
  hid_t root = file.getRootGroup();
  std::string groupName = "group";
  std::string datasetName = "data";
  const int dimLength = 100;
  DimensionSizes dataSizes(dimLength, dimLength, dimLength);
  Hdf5File::MatrixDataType dataType = Hdf5File::MatrixDataType::kFloat;
  hid_t groupId = file.createGroup(root, groupName);
  hid_t datasetId = file.createDataset(groupId, datasetName, dataSizes, dataSizes, dataType, 0);
  file.closeDataset(datasetId);
  file.closeGroup(groupId);
  file.close();

  file.open(testFileName);
  try{
    root = file.getRootGroup();
    groupId = file.openGroup(root, groupName);
    datasetId = file.openDataset(groupId, datasetName);
    REQUIRE(true);
  } catch(...) {
    REQUIRE(false);
  }
  DimensionSizes readSizes = file.getDatasetDimensionSizes(groupId, datasetName);
  file.closeDataset(datasetId);
  file.closeGroup(groupId);
  file.close();

  REQUIRE( ((readSizes.nx == dataSizes.nx) &&
	    (readSizes.ny == dataSizes.ny) &&
	    (readSizes.nz == dataSizes.nz)) );
}

TEST_CASE( "Test attribute creation", "[HDF5attributes]" )
{
  Hdf5File file;
  createTestFile(file);
  hid_t root = file.getRootGroup();
  std::string groupName = "group";
  std::string datasetName = "data";
  std::string attributeName = "attrib";
  std::string attributeValue = "contents";
  const int dimLength = 100;
  DimensionSizes dataSizes(dimLength, dimLength, dimLength);
  Hdf5File::MatrixDataType dataType = Hdf5File::MatrixDataType::kFloat;
  hid_t groupId = file.createGroup(root, groupName);
  hid_t datasetId = file.createDataset(groupId, datasetName, dataSizes, dataSizes, dataType, 0);
  file.writeStringAttribute(groupId, datasetName, attributeName, attributeValue);
  file.closeDataset(datasetId);
  file.closeGroup(groupId);
  file.close();

  file.open(testFileName);
  try{
    root = file.getRootGroup();
    groupId = file.openGroup(root, groupName);
    REQUIRE(true);
  } catch(...) {
    REQUIRE(false);
  }
  std::string readValue = file.readStringAttribute(groupId, datasetName, attributeName);
  file.closeGroup(groupId);  
  file.close();

  CHECK_THAT(readValue, Catch::Equals(attributeValue));
}

TEST_CASE( "Test group attribute creation", "[HDF5attributes]" )
{
  Hdf5File file;
  createTestFile(file);
  hid_t root = file.getRootGroup();
  std::string groupName = "group";
  std::string attributeName = "attrib";
  std::string attributeValue = "contents";
  const int dimLength = 100;
  DimensionSizes dataSizes(dimLength, dimLength, dimLength);
  Hdf5File::MatrixDataType dataType = Hdf5File::MatrixDataType::kFloat;
  hid_t groupId = file.createGroup(root, groupName);

  file.writeStringAttribute(root, groupName, attributeName, attributeValue);
  file.closeGroup(groupId);
  file.close();

  file.open(testFileName);
  try{
    root = file.getRootGroup();
    REQUIRE(true);
  } catch(...) {
    REQUIRE(false);
  }
  std::string readValue = file.readStringAttribute(root, groupName, attributeName);
  file.close();

  CHECK_THAT(readValue, Catch::Equals(attributeValue));
}

TEST_CASE( "Test uint64 attribute creation", "[HDF5attributes]" )
{
  Hdf5File file;
  createTestFile(file);
  hid_t root = file.getRootGroup();
  std::string groupName = "group";
  std::string datasetName = "data";
  std::string attributeName = "uint64_attrib";
  uint64_t attributeValue = 7357ULL;
  const int dimLength = 100;
  DimensionSizes dataSizes(dimLength, dimLength, dimLength);
  Hdf5File::MatrixDataType dataType = Hdf5File::MatrixDataType::kFloat;
  hid_t groupId = file.createGroup(root, groupName);
  hid_t datasetId = file.createDataset(groupId, datasetName, dataSizes, dataSizes, dataType, 0);
  file.writeAttribute(groupId, datasetName, attributeName, attributeValue);
  file.closeDataset(datasetId);
  file.closeGroup(groupId);
  file.close();

  file.open(testFileName);
  try{
    root = file.getRootGroup();
    groupId = file.openGroup(root, groupName);
    REQUIRE(true);
  } catch(...) {
    REQUIRE(false);
  }
  uint64_t readValue = file.readAttribute<uint64_t>(groupId, datasetName, attributeName);
  file.closeGroup(groupId);  
  file.close();

  REQUIRE(readValue == attributeValue);
}

TEST_CASE( "Test uint64 group attribute creation", "[HDF5attributes]" )
{
  Hdf5File file;
  createTestFile(file);
  hid_t root = file.getRootGroup();
  std::string groupName = "group";
  std::string attributeName = "uint64_attrib";
  uint64_t attributeValue = 73571ULL;
  const int dimLength = 100;
  DimensionSizes dataSizes(dimLength, dimLength, dimLength);
  Hdf5File::MatrixDataType dataType = Hdf5File::MatrixDataType::kFloat;
  hid_t groupId = file.createGroup(root, groupName);

  file.writeAttribute(root, groupName, attributeName, attributeValue);
  file.closeGroup(groupId);
  file.close();

  file.open(testFileName);
  try{
    root = file.getRootGroup();
    REQUIRE(true);
  } catch(...) {
    REQUIRE(false);
  }
  uint64_t readValue = file.readAttribute<uint64_t>(root, groupName, attributeName);
  file.close();

  REQUIRE(readValue == attributeValue);
}
