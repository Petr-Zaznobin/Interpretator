#include "gtest.h"
#include "ReadCmd.h"

TEST(ReadCmd, can_creat_ReadCmd)
{
	string* tmp = new string[5];
	ASSERT_NO_THROW(ReadCmd(tmp, 5));
}

//getCmds??
//parsing??

