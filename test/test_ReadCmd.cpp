#include "gtest.h"
#include "ReadCmd.h"

TEST(ReadCmd, can_creat_ReadCmd)
{
	string* tmp = new string[5];
	ASSERT_NO_THROW(ReadCmd(tmp, 5));
}

//getCmds??
//parsing??

/*TEST(ReadCmd, can_get_cmds_from_ReadCmd)
{
	string* cmds = new string[5];
	cmds[0] = "main() {";
	cmds[1] = "int i = 0;";
	cmds[2] = "i = 7 + 5;";
	cmds[3] = "i = 10 - 7;";
	cmds[4] = "}";
	ReadCmd code(cmds, 5);

	ASSERT_NO_THROW(code.getCmds());
}*/

TEST(ReadCmd, can_make_char_from_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "a";
	ASSERT_NO_THROW(code.stringToChar(a));
}
TEST(ReadCmd, cant_make_char_from_long_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "ab";
	ASSERT_ANY_THROW(code.stringToChar(a));
}
TEST(ReadCmd, correct_make_char_from_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "a";
	EXPECT_EQ(code.stringToChar(a),'a');
}
TEST(ReadCmd, can_make_int_from_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "10";
	ASSERT_NO_THROW(code.stringToInt(a));
}
TEST(ReadCmd, cant_make_int_from_wrong_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "ab";
	ASSERT_ANY_THROW(code.stringToInt(a));
}
TEST(ReadCmd, correct_make_int_from_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "10";
	int b = 10;
	EXPECT_EQ(code.stringToInt(a), b);
}
TEST(ReadCmd, can_make_double_from_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "10.5";
	ASSERT_NO_THROW(code.stringToDouble(a));
}
TEST(ReadCmd, cant_make_double_from_wrong_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "1a0";
	ASSERT_ANY_THROW(code.stringToDouble(a));
}
TEST(ReadCmd, correct_make_double_from_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "10.5";
	double b = 10.5;
	EXPECT_EQ(code.stringToFloat(a), b);
}
TEST(ReadCmd, can_make_float_from_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "10.5";
	ASSERT_NO_THROW(code.stringToFloat(a));
}
TEST(ReadCmd, cant_make_float_from_wrong_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "1a0";
	ASSERT_ANY_THROW(code.stringToFloat(a));
}
TEST(ReadCmd, correct_make_float_from_string)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	string a = "10.5";
	float b = 10.5;
	EXPECT_EQ(code.stringToFloat(a), b);
}
TEST(ReadCmd, can_make_string_from_int)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	int a = 10;
	string b = "10";
	ASSERT_NO_THROW(code.intToString(a));
}
TEST(ReadCmd, correct_make_string_from_int)
{
	string* tmp = new string[5];
	ReadCmd code(tmp, 5);
	int a = 10;
	string b = "10";
	EXPECT_EQ(code.intToString(a), b);
}
