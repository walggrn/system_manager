#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "mock_command.h"
#include "utils/Parser.h"

using namespace std;

class TestParser : public testing::Test {
protected:
  Parser Parser;
  MockCommand Mock; // arg1 arg2 k1 k2(string) k3(int)
  vector<string> Parametrs;

  void SetUp() override {
    Parametrs = {"arg1", "arg2", "-k1", "-k2", "kot", "-k3", "10"};
  }

  void TearDown() override {}
};

TEST_F(TestParser, P_Input_PositiveParsingEmptyString) {
  auto tokens = Parser.parse_input("");
  ASSERT_EQ(tokens.size(), 0) << "Excpected 0 tokens but got " << tokens.size();
}

TEST_F(TestParser, P_Input_PositiveParsingString) {
  auto tokens =
      Parser.parse_input("     create                \"temp\"      -rc      ");
  ASSERT_EQ(tokens.size(), 3) << "Excpected 3 tokens but got " << tokens.size();
  EXPECT_EQ(tokens[0], "create") << tokens[0] << " != " << "create";
  EXPECT_EQ(tokens[1], "temp") << tokens[1] << " != " << "\"temp\"";
  EXPECT_EQ(tokens[2], "-rc") << tokens[2] << " != " << "-rc";
}

TEST_F(TestParser, P_Input_NegativeParsingUnfinishedQuotes) {
  EXPECT_THROW(Parser.parse_input("create \"temp\"      -rc      \"\"\""),
               runtime_error)
      << "Excpected a exception ";
}

TEST_F(TestParser, P_Tokens_PositiveParsingCorrectParametrs) {
  ASSERT_NO_THROW(Parser.parse_tokens(Parametrs, Mock))
      << "Exception was throwed during parsing tokens";
  string arg1, arg2, key2;
  bool key1;
  int key3;
  tie(arg1, arg2) = Mock.get_args();
  tie(key1, key2, key3) = Mock.get_keys();
  EXPECT_EQ(arg1, Parametrs.at(0)) << arg1 << " != " << Parametrs.at(0);
  EXPECT_EQ(arg2, Parametrs.at(1)) << arg2 << " != " << Parametrs.at(1);
  EXPECT_TRUE(key1) << "Key1 is false";
  EXPECT_EQ(key2, Parametrs.at(4)) << key2 << " != " << Parametrs.at(4);
  EXPECT_EQ(key3, stoi(Parametrs.at(6)))
      << key3 << " != " << stoi(Parametrs.at(6));
}

TEST_F(TestParser, P_Tokens_PositiveParsingEmptyArgument) {
  Parametrs.erase(Parametrs.begin() + 1);
  ASSERT_NO_THROW(Parser.parse_tokens(Parametrs, Mock))
      << "Exception was throwed during parsing tokens";
  string arg1, arg2, key2;
  bool key1;
  int key3;
  tie(arg1, arg2) = Mock.get_args();
  tie(key1, key2, key3) = Mock.get_keys();
  EXPECT_EQ(arg2, "default") << arg2 << " != " << "default";
}

TEST_F(TestParser, P_Tokens_NegativeParsingUnknownKey) {
  Parametrs[2] = "-k11";
  EXPECT_THROW(
      {
        try {
          Parser.parse_tokens(Parametrs, Mock);
        } catch (const runtime_error &e) {
          EXPECT_STREQ(e.what(),
                       "Error: unknown input detected. => Token: \"-k11\".");
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(TestParser, P_Tokens_NegativeParsingEmptyValueForKey) {
  Parametrs.pop_back();
  EXPECT_THROW(
      {
        try {
          Parser.parse_tokens(Parametrs, Mock);
        } catch (const runtime_error &e) {
          EXPECT_STREQ(e.what(),
                       "Error: missing value for key. => Key: \"--key3\".");
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(TestParser, P_Tokens_NegativeParsingWrongValueForIntegerKey) {
  Parametrs[6] = "str(10)";
  EXPECT_THROW(
      {
        try {
          Parser.parse_tokens(Parametrs, Mock);
        } catch (const runtime_error &e) {
          EXPECT_STREQ(e.what(), "Error: invalid key value. => Attempt to use "
                                 "non-integer value for key: \"--key3\".");
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(TestParser, P_Tokens_NegativeParsingBigValueForIntegerKey) {
  Parametrs[6] = "1111111111111111111111111111111111111111111";
  EXPECT_THROW(
      {
        try {
          Parser.parse_tokens(Parametrs, Mock);
        } catch (const runtime_error &e) {
          EXPECT_STREQ(e.what(), "Error: invalid key value. => Value out of "
                                 "range for an integer key: \"--key3\".");
          throw;
        }
      },
      std::runtime_error);
}

TEST_F(TestParser, P_Tokens_NegativeParsingResetKey) {
  Parametrs.push_back("-k1");
  EXPECT_THROW(
      {
        try {
          Parser.parse_tokens(Parametrs, Mock);
        } catch (const runtime_error &e) {
          EXPECT_STREQ(e.what(),
                       "Error: incorrect use of key. => Attempt to use key: "
                       "\"--key1\" but the key has already been used.");
          throw;
        }
      },
      std::runtime_error);
}