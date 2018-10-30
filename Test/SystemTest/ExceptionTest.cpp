#include "stdafx.h"

TEST(SystemTest, ExceptionTest)
{
	std::string strErrMsg = "Test SystemTest Exception";
	std::string strCatchMsg;
	try
	{
		throw exception_format("%s", strErrMsg.c_str());
	}
	catch (std::exception& e)
	{
		strCatchMsg = e.what();
	}

	EXPECT_EQ(strErrMsg, strCatchMsg);
}

