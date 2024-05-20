#include "gtest.h"
#include "ReadCmd.h"
using namespace std;

TEST(ReadCmd, can_create_ReadCmd)
{
	string* tmp = new string[5];
	ASSERT_NO_THROW(ReadCmd(tmp, 5));
}


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
TEST(ReadCmd, can_parse_cmd_with_new_value)
{
	string* cmds = new string[3];
	cmds[0] = "main() {";
	cmds[1] = "int i;";
	cmds[2] = "}";
	ReadCmd code(cmds, 3);

	ASSERT_NO_THROW(code.getCmds());
}
TEST(ReadCmd, can_parse_cmd_with_value)
{
	string* cmds = new string[4];
	cmds[0] = "main() {";
	cmds[1] = "int i;";
	cmds[2] = "i = 5;";
	cmds[3] = "}";
	ReadCmd code(cmds, 4);

	ASSERT_NO_THROW(code.getCmds());
}
//getCmds??
//parsing??


TEST(ReadCmd, can_parse_cmd_with_inicial_of_value)
{
	string* cmds = new string[3];
	cmds[0] = "main() {";
	cmds[1] = "int i = 0;";
	cmds[2] = "}";
	ReadCmd code(cmds, 3);

	ASSERT_NO_THROW(code.getCmds());
}
TEST(ReadCmd, can_parse_cmds_with_math_operation_save_in_the_same_value)
{
	string* cmds = new string[4];
	cmds[0] = "main() {";
	cmds[1] = "int i = 0;";
	cmds[2] = "i = i + 7;";
	cmds[3] = "}";
	ReadCmd code(cmds, 4);

	ASSERT_NO_THROW(code.getCmds());
}
TEST(ReadCmd, can_parse_cmds_with_math_operation_save_in_new_value)
{
	string* cmds = new string[6];
	cmds[0] = "main() {";
	cmds[1] = "int i = 4;";
	cmds[2] = "int a = 3;";
	cmds[3] = "int b;";
	cmds[4] = "b = i + a;";
	cmds[5] = "}";
	ReadCmd code(cmds, 6);

	ASSERT_NO_THROW(code.getCmds());
}
TEST(ReadCmd, correct_parse_cmds_with_math_operation_save_in_new_value)
{
	string* cmds = new string[6];
	cmds[0] = "main() {";
	cmds[1] = "int i = 4;";
	cmds[2] = "int a = 3;";
	cmds[3] = "int b;";
	cmds[4] = "b = i + a;";
	cmds[5] = "}";
	ReadCmd code(cmds, 6);
	code.getCmds();
	//EXPECT_EQ(7,);
}
TEST(ReadCmd, can_parse_cmds_with_if)
{
	string* cmds = new string[8];
	cmds[0] = "main() {";
	cmds[1] = "int i = 5;";
	cmds[2] = "int a = 0;";
	cmds[3] = "int b;";
	cmds[4] = "if (i > 3){";
	cmds[5] = "b = i + a;";
	cmds[6] = "}";
	cmds[7] = "}";
	ReadCmd code(cmds, 8);

	ASSERT_NO_THROW(code.getCmds());
}
TEST(ReadCmd, can_parse_cmds_with_if)
{
	string* cmds = new string[8];
	cmds[0] = "main() {";
	cmds[1] = "int i = 5;";
	cmds[2] = "int a = 3;";
	cmds[3] = "int b = 0;";
	cmds[4] = "if (i > 3) {";
	cmds[5] = "b = i + a;";
	cmds[6] = "}";
	cmds[7] = "}";
	ReadCmd code(cmds, 8);
	//EXPECT_EQ(8,);

}
/*TEST(ReadCmd, can_get_cmds_from_ReadCmd)
{
	string* cmds = new string[3];
	cmds[0] = "main() {";
	cmds[1] = "int i = 0;";
	cmds[2] = "}";
	ReadCmd code(cmds, 3);

	ASSERT_NO_THROW(code.getCmds());
}*/
