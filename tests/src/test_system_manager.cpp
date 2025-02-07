#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>

#include "SystemManager.h"

using namespace std;
namespace fs = std::filesystem;

class TestSystemManager_Find : public testing::Test {
protected:
  string Dir;
  SystemManager Manager;
  vector<string> matches;
  vector<string> visited;

  void SetUp() override {
    Dir = fs::temp_directory_path().string() + "/test_find_dir";
    fs::create_directories(Dir);
    ofstream(Dir + "/testfile1.txt").close();
    ofstream(Dir + "/testfile2.log").close();
    fs::create_directory(Dir + "/subdir");
    ofstream(Dir + "/subdir/testfile3.txt").close();
    matches = {};
    visited = {};
  }

  void TearDown() override { fs::remove_all(Dir); }
};

TEST_F(TestSystemManager_Find, PositiveSystemManagerFindExactMatch) {
  Manager.find(matches, visited, "testfile1.txt", Dir);

  ASSERT_EQ(matches.size(), 1);
  EXPECT_EQ(fs::path(matches[0]).filename(), "testfile1.txt");
}

TEST_F(TestSystemManager_Find, PositiveSystemManagerFindContainsMatch) {
  Manager.find(matches, visited, "testfile", Dir, true);

  ASSERT_EQ(matches.size(), 2);
}

TEST_F(TestSystemManager_Find, PositiveSystemManagerFindRecursiveMatch) {
  Manager.find(matches, visited, "testfile3.txt", Dir, true, All, -1, true);

  ASSERT_EQ(matches.size(), 1);
  EXPECT_TRUE(matches[0].find("subdir") != std::string::npos);
}

TEST_F(TestSystemManager_Find, NegativeSystemManagerNotExistDirectory) {
  EXPECT_THROW(Manager.find(matches, visited, "temp", "/notexistdirectory"),
               runtime_error);
}

TEST_F(TestSystemManager_Find, NegativeSystemManagerFindAtFile) {
  EXPECT_THROW(Manager.find(matches, visited, "file", Dir + "/file.txt"),
               runtime_error);
}
