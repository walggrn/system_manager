#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

#include "utils/SystemManager.h"

using namespace std;
namespace fs = std::filesystem;

class TestSystemManager_Insert : public testing::Test {
protected:
  SystemManager Manager;
  string Dir;
  string Filename;
  string Data = " test!";
  vector<string> Filecontent = {"line 1", "line 2", "line 3"};

  void SetUp() override {
    Dir = fs::temp_directory_path().string() + "/test_insert";
    fs::create_directories(Dir);
    Filename = "/testfile.txt";
    ofstream file(Dir + Filename);
    for (const auto &line : Filecontent) {
      file << line << "\n";
    }
    file.close();
  }

  void TearDown() override { fs::remove_all(Dir); }

  vector<string> GetFileContent() {
    ifstream file(Dir + Filename);
    vector<string> content;
    string line;
    while (getline(file, line)) {
      content.push_back(line);
    }
    file.close();
    return content;
  }

  void CreateDirectory(const string &name) { fs::create_directory(Dir + name); }
};

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

// тестирование команды Insert
TEST_F(TestSystemManager_Insert, SM_Insert_PositiveInsertNoParametrs) {
  Manager.insert(Dir + Filename, Data);
  vector<string> lines = GetFileContent();
  ASSERT_EQ(lines.size(), Filecontent.size() + 1) << "";
  EXPECT_EQ(lines[Filecontent.size()], Data) << "";
}

TEST_F(TestSystemManager_Insert, SM_Insert_PositiveInsertLineParametr) {
  Manager.insert(Dir + Filename, Data, Filecontent.size() - 1);
  vector<string> lines = GetFileContent();
  ASSERT_EQ(lines.size(), Filecontent.size()) << "";
  EXPECT_EQ(lines[Filecontent.size() - 1],
            Filecontent[Filecontent.size() - 1] + Data)
      << "";
}

TEST_F(TestSystemManager_Insert, SM_Insert_PositiveInsertN1SkipParametr) {
  Manager.insert(Dir + Filename, Data, 0, 0);
  vector<string> lines = GetFileContent();
  ASSERT_EQ(lines.size(), Filecontent.size()) << "";
  EXPECT_EQ(lines[0], Data + Filecontent[0]) << "";
}

TEST_F(TestSystemManager_Insert, SM_Insert_PositiveInsertN2SkipParametr) {
  Manager.insert(Dir + Filename, Data, 0, Filecontent[0].size() - 1);
  vector<string> lines = GetFileContent();
  ASSERT_EQ(lines.size(), Filecontent.size()) << "";
  string line =
      Filecontent[0].substr(0, Filecontent[0].size() - 1) + Data +
      Filecontent[0].substr(Filecontent[0].size() - 1, Filecontent[0].size());
  EXPECT_EQ(lines[0], line) << lines[0] << " != " << line;
}

TEST_F(TestSystemManager_Insert,
       SM_Insert_PositiveInsertWithIncorrectParametrs) {
  Manager.insert(Dir + Filename, Data, Filecontent.size() + 1000);
  vector<string> lines = GetFileContent();
  ASSERT_EQ(lines.size(), Filecontent.size() + 1) << "";
  Manager.insert(Dir + Filename, Data, Filecontent.size(),
                 lines[Filecontent.size()].size() + 1000);
  lines = GetFileContent();
  ASSERT_EQ(lines.size(), Filecontent.size() + 1) << "";
  EXPECT_EQ(lines[Filecontent.size()], Data + Data);
}

TEST_F(TestSystemManager_Insert, SM_Insert_NegativeInsertAtNotExistFile) {
  EXPECT_THROW(
      {
        try {
          Manager.insert(Dir + "/notexists.txt", Data);
        } catch (const runtime_error &e) {
          string exception = "Error: object doesn't exists. => Object not "
                             "found at the specified path: \"" +
                             Dir + "/notexists.txt\".";
          EXPECT_STREQ(e.what(), exception.c_str());
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(TestSystemManager_Insert, SM_Insert_NegativeInsertAtNotFile) {
  CreateDirectory("/directory");
  EXPECT_THROW(
      {
        try {
          Manager.insert(Dir + "/directory", Data);
        } catch (const runtime_error &e) {
          string exception = "Error: type object is invalid. => Object at the "
                             "specified path: \"" +
                             Dir +
                             "/directory\" type does not "
                             "match the expected type. Expected type: File.";
          EXPECT_STREQ(e.what(), exception.c_str());
          throw;
        }
      },
      runtime_error);
}

// тестирование команды Find
TEST_F(TestSystemManager_Find, SM_Find_PositiveFindExactMatch) {
  Manager.find(matches, visited, "testfile1.txt", Dir);

  ASSERT_EQ(matches.size(), 1);
  EXPECT_EQ(fs::path(matches[0]).filename(), "testfile1.txt");
}

TEST_F(TestSystemManager_Find, SM_Find_PositiveFindConatinsMatch) {
  Manager.find(matches, visited, "testfile", Dir, true);

  ASSERT_EQ(matches.size(), 2);
}

TEST_F(TestSystemManager_Find, SM_Find_PositiveFindRecursiveMatch) {
  Manager.find(matches, visited, "testfile3.txt", Dir, true, All, -1, true);

  ASSERT_EQ(matches.size(), 1);
  EXPECT_TRUE(matches[0].find("subdir") != std::string::npos);
}