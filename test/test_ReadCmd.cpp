#include "gtest.h"
#include "ReadCmd.h"

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
	cmds[1] = "int i = 0;"; //НЕЛЬЗЯ ОСТАВЛЯТЬ ПЕРЕМЕННУЮ НЕИНИЦИАЛИЗИРОВАННОЙ!!!
	cmds[2] = "}";
	ReadCmd code(cmds, size_t(3));
	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, can_parse_cmd_with_value) //НЕЛЬЗЯ ОСТАВЛЯТЬ ПЕРЕМЕННУЮ НЕИНИЦИАЛИЗИРОВАННОЙ!!!
//{
//	string* cmds = new string[4];
//	cmds[0] = "main() {";
//	cmds[1] = "int i;";
//	cmds[2] = "i = 5;";
//	cmds[3] = "}";
//	ReadCmd code(cmds, 4);
//
//	ASSERT_NO_THROW(code.getCmds());
//}
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
	cmds[3] = "int b = 0;";
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
	cmds[3] = "int b = 0;";
	cmds[4] = "b = i + a;";
	cmds[5] = "}";
	ReadCmd code(cmds, 6);
	code.getCmds();
	//EXPECT_EQ(7,);
}

TEST(ReadCmd, can_parse_cmds_with_if_corr)
{
	string* cmds = new string[8];
	cmds[0] = "main() {";
	cmds[1] = "int i = 5;";
	cmds[2] = "int a = 3;";
	cmds[3] = "int b = 0;";
	cmds[4] = "if (i > a) {";  //ОБА АРГУМЕНТА ИФА ДОЛЖНЫ БЫТЬ ПЕРЕМЕННЫМИ!!!
	cmds[5] = "b = i + a;";
	cmds[6] = "}";
	cmds[7] = "}";
	ReadCmd code(cmds, 8);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_if)
//{
//	string* cmds = new string[8];
//	cmds[0] = "main() {";
//	cmds[1] = "int i = 5;";
//	cmds[2] = "int a = 3;";
//	cmds[3] = "int b = 0;";
//	cmds[4] = "if (i > 3) {";
//	cmds[5] = "b = i + a;";
//	cmds[6] = "}";
//	cmds[7] = "}";
//	ReadCmd code(cmds, 8);
//	//EXPECT_EQ(8,);
//
//}
TEST(ReadCmd, can_parse_cmds_with_while)
{
	string* cmds = new string[8];
	cmds[0] = "main() {";
	cmds[1] = "int i = 5;";
	cmds[2] = "int a = 2;";
	cmds[3] = "int b = 0;";
	cmds[4] = "while (b <= a) {";
	cmds[5] = "b = b + i;";
	cmds[6] = "}";
	cmds[7] = "}";
	ReadCmd code(cmds, 8);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_while)
//{
//	string* cmds = new string[8];
//	cmds[0] = "main() {";
//	cmds[1] = "int i = 5;";
//	cmds[2] = "int a = 2;";
//	cmds[3] = "int b = 0;";
//	cmds[4] = "while (b <= a) {"; //В УСЛОВИИ ПЕРЕМЕННЫЕ
//	cmds[5] = "b = b + i;";
//	cmds[6] = "}";
//	cmds[7] = "}";
//	ReadCmd code(cmds, 8);
//	code.getCmds();
//	//EXPECT_EQ(14,);
//
//}
TEST(ReadCmd, can_parse_cmds_with_for)
{
	string* cmds = new string[8];
	cmds[0] = "main() {";
	cmds[1] = "int a = 2;";
	cmds[2] = "int b = 0;";
    cmds[3] = "int c = 2;";
	cmds[4] = "for (int i = 0; i < c; i++) {";
	cmds[5] = "b = b + a;";
	cmds[6] = "}";
	cmds[7] = "}";
	ReadCmd code(cmds, 8);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_for)
//{
//	string* cmds = new string[7];
//	cmds[0] = "main() {";
//	cmds[1] = "int a = 2;";
//	cmds[2] = "int b = 0;";
//	cmds[3] = "for (int i = 0; i < 3; i++) {";
//	cmds[4] = "b = b + a;";
//	cmds[5] = "}";
//	cmds[6] = "}";
//	ReadCmd code(cmds, 7);
//	code.getCmds();
//	//EXPECT_EQ(6,);
//
//}
TEST(ReadCmd, can_parse_cmds_with_while_and_if_inside)
{
	string* cmds = new string[11];
	cmds[0] = "main() {";
	cmds[1] = "int i = 5;";
	cmds[2] = "int b = 2;";
    cmds[3] = "int c = 7;";
    cmds[4] = "int d = 3;";
	cmds[5] = "while (b <= c) {";
	cmds[6] = "if (i > d) {";
	cmds[7] = "b = b + i;";
	cmds[8] = "}";
	cmds[9] = "}";
	cmds[10] = "}";
	ReadCmd code(cmds, 11);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_while_and_if_inside)
//{
//	string* cmds = new string[9];
//	cmds[0] = "main() {";
//	cmds[1] = "int i = 5;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "while (b <= 7) {";
//	cmds[4] = "if (i > 3) {";
//	cmds[5] = "b = b + i;";
//	cmds[6] = "}";
//	cmds[7] = "}";
//	cmds[8] = "}";
//	ReadCmd code(cmds, 9);
//	code.getCmds();
//	//EXPECT_EQ(12,);
//}
TEST(ReadCmd, can_parse_cmds_with_while_and_for_inside)
{
	string* cmds = new string[10];
	cmds[0] = "main() {";
	cmds[1] = "int b = 2;";
    cmds[2] = "int c = 3;";
    cmds[3] = "int d = 9;";
	cmds[4] = "while (b <= c) {";
	cmds[5] = "for (int i = 1; i < d; i++) {";
	cmds[6] = "b = b + i;";
	cmds[7] = "}";
	cmds[8] = "}";
	cmds[9] = "}";
	ReadCmd code(cmds, 10);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_while_and_for_inside)
//{
//	string* cmds = new string[9];
//	cmds[0] = "main() {";
//	cmds[1] = "int i;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "while (b < 15) {";
//	cmds[4] = "for (i = 1; i < 5; i++) {";
//	cmds[5] = "b = b + i;";
//	cmds[6] = "}";
//	cmds[7] = "}";
//	cmds[8] = "}";
//	ReadCmd code(cmds, 9);
//	code.getCmds();
//	//EXPECT_EQ(22,);
//}
TEST(ReadCmd, can_parse_cmds_with_while_and_while_inside)
{
	string* cmds = new string[12];
	cmds[0] = "main() {";
	cmds[1] = "int i = 2;";
	cmds[2] = "int b = 2;";
    cmds[3] = "int c = 4;";
    cmds[4] = "int d = 5;";
	cmds[5] = "while (b < c) {";
	cmds[6] = "while (i < d) {";
	cmds[7] = "b = b + i;";
	cmds[8] = "i = i + 5;";
	cmds[9] = "}";
	cmds[10] = "}";
	cmds[11] = "}";
	ReadCmd code(cmds, 12);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_while_and_while_inside)
//{
//	string* cmds = new string[10];
//	cmds[0] = "main() {";
//	cmds[1] = "int i = 1;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "while (b <= 7) {";
//	cmds[4] = "while (i < 5) {";
//	cmds[5] = "b = b + i;";
//	cmds[6] = "i = i + 1";
//	cmds[7] = "}";
//	cmds[8] = "}";
//	cmds[9] = "}";
//	ReadCmd code(cmds, 10);
//	code.getCmds();
//	//EXPECT_EQ(22,);
//}
TEST(ReadCmd, can_parse_cmds_with_for_and_for_inside)
{
	string* cmds = new string[9];
	cmds[0] = "main() {";
	cmds[1] = "int a = 3;";
	cmds[2] = "int c = 0;";
	cmds[3] = "for (int b = 1; b < a; b++) {";
	cmds[4] = "for (int i = 1; i < a; i++) {";
	cmds[5] = "c = b + i;";
	cmds[6] = "}";
	cmds[7] = "}";
	cmds[8] = "}";
	ReadCmd code(cmds, 9);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_for_and_for_inside)
//{
//	string* cmds = new string[10];
//	cmds[0] = "main() {";
//	cmds[1] = "int i;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "int c = 0;";
//	cmds[4] = "for (b = 1; b < 3; b++) {";
//	cmds[5] = "for (i = 1; i < 3; i++) {";
//	cmds[6] = "c = b + i;";
//	cmds[7] = "}";
//	cmds[8] = "}";
//	cmds[9] = "}";
//	ReadCmd code(cmds, 10);
//	code.getCmds();
//	//EXPECT_EQ(12,);
//}
TEST(ReadCmd, can_parse_cmds_with_for_and_while_inside)
{
	string* cmds = new string[11];
	cmds[0] = "main() {";
	cmds[1] = "int i = 0;";
	cmds[2] = "int a = 3;";
	cmds[3] = "int c = 0;";
	cmds[4] = "for (int b = 1; b < a; b++) {";
	cmds[5] = "while (i < a) {";
	cmds[6] = "c = b + i;";
	cmds[7] = "i = i + 1;";
	cmds[8] = "}";
	cmds[9] = "}";
	cmds[10] = "}";
	ReadCmd code(cmds, 11);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_for_and_while_inside)
//{
//	string* cmds = new string[11];
//	cmds[0] = "main() {";
//	cmds[1] = "int i = 0;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "int c = 0;";
//	cmds[4] = "for (b = 1; b < 3; b++) {";
//	cmds[5] = "while (i < 4) {";
//	cmds[6] = "c = b + i;";
//	cmds[7] = "i = i + 1;";
//	cmds[8] = "}";
//	cmds[9] = "}";
//	cmds[10] = "}";
//	ReadCmd code(cmds, 11);
//	code.getCmds();
//	//EXPECT_EQ(10,);
//}
TEST(ReadCmd, can_parse_cmds_with_for_and_if_inside)
{
	string* cmds = new string[12];
	cmds[0] = "main() {";
	cmds[1] = "int i = 0;";
	cmds[2] = "int a = 3;";
	cmds[3] = "int c = 0;";
    cmds[4] = "int d = 4;";
	cmds[5] = "for (int b = 1; b < a; b++) {";
	cmds[6] = "if (i < d) {";
	cmds[7] = "c = b + i;";
	cmds[8] = "i = i + 1;";
	cmds[9] = "}";
	cmds[10] = "}";
	cmds[11] = "}";
	ReadCmd code(cmds, 12);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, can_parse_cmds_with_for_and_if_inside)
//{
//	string* cmds = new string[11];
//	cmds[0] = "main() {";
//	cmds[1] = "int i = 0;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "int c = 0;";
//	cmds[4] = "for (b = 1; b < 3; b++) {";
//	cmds[5] = "if (i < 4) {";
//	cmds[6] = "c = b + i;";
//	cmds[7] = "i = i + 1;";
//	cmds[8] = "}";
//	cmds[9] = "}";
//	cmds[10] = "}";
//	ReadCmd code(cmds, 11);
//	code.getCmds();
//	//EXPECT_EQ(4,);
//}
TEST(ReadCmd, can_parse_cmds_with_if_and_while_inside)
{
	string* cmds = new string[12];
	cmds[0] = "main() {";
	cmds[1] = "int i = 1;";
	cmds[2] = "int b = 2;";
    cmds[3] = "int c = 7;";
    cmds[4] = "int d = 5;";
	cmds[5] = "if (b <= c) {";
	cmds[6] = "while (i < d) {";
	cmds[7] = "b = b + i;";
	cmds[8] = "i = i + 1;";
	cmds[9] = "}";
	cmds[10] = "}";
	cmds[11] = "}";
	ReadCmd code(cmds, 12);

	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_if_and_while_inside)
//{
//	string* cmds = new string[10];
//	cmds[0] = "main() {";
//	cmds[1] = "int i = 1;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "if (b <= 7) {";
//	cmds[4] = "while (i < 5) {";
//	cmds[5] = "b = b + i;";
//	cmds[6] = "i = i + 1";
//	cmds[7] = "}";
//	cmds[8] = "}";
//	cmds[9] = "}";
//	ReadCmd code(cmds, 10);
//	code.getCmds();
//	//EXPECT_EQ(12,);
//}
TEST(ReadCmd, can_parse_cmds_with_if_and_for_inside)
{
	string* cmds = new string[10];
	cmds[0] = "main() {";
	cmds[1] = "int a = 3;";
	cmds[2] = "int b = 2;";
    cmds[3] = "int k = 5;";
	cmds[4] = "if (b < a) {";
	cmds[5] = "for (int i = 1; i < k; i++) {";
	cmds[6] = "b = b + i;";
	cmds[7] = "}";
	cmds[8] = "}";
	cmds[9] = "}";
	ReadCmd code(cmds, 10);
	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_if_and_for_inside)
//{
//	string* cmds = new string[9];
//	cmds[0] = "main() {";
//	cmds[1] = "int i;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "if (b < 3) {";
//	cmds[4] = "for (i = 1; i < 5; i++) {";
//	cmds[5] = "b = b + i;";
//	cmds[6] = "}";
//	cmds[7] = "}";
//	cmds[8] = "}";
//	ReadCmd code(cmds, 9);
//	code.getCmds();
//	//EXPECT_EQ(12,);
//}
TEST(ReadCmd, can_parse_cmds_with_if_and_if_inside)
{
	string* cmds = new string[11];
	cmds[0] = "main() {";
	cmds[1] = "int i = 3;";
	cmds[2] = "int b = 2;";
    cmds[3] = "int c = 3;";
    cmds[4] = "int k = 5;";
	cmds[5] = "if (b < c) {";
	cmds[6] = "if (i < k) {";
	cmds[7] = "b = b + i;";
	cmds[8] = "}";
	cmds[9] = "}";
	cmds[10] = "}";
	ReadCmd code(cmds, 11);
	ASSERT_NO_THROW(code.getCmds());
}
//TEST(ReadCmd, correct_parse_cmds_with_if_and_if_inside)
//{
//	string* cmds = new string[9];
//	cmds[0] = "main() {";
//	cmds[1] = "int i = 3;";
//	cmds[2] = "int b = 2;";
//	cmds[3] = "if (b < 3) {";
//	cmds[4] = "if (i < 5) {";
//	cmds[5] = "b = b + i;";
//	cmds[6] = "}";
//	cmds[7] = "}";
//	cmds[8] = "}";
//	ReadCmd code(cmds, 9);
//	code.getCmds();
//	//EXPECT_EQ(5,);
//}