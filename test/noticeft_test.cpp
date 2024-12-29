#include <gtest/gtest.h>
#include <noticeft.h>
#include <cstdio>

TEST(NoticeFTTest, ReceiveSentfileInFTP)
{
	char* ip = "127.0.0.1";
	int port = 21;
	char* id = "test";
	char* password = "test";

	return;

	auto context = noticeft_login(ip, port, id, password, false);

	{

		int ret = noticeft_sendFile(context, "./resource/샌주.PNG", "샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	{
		int ret = noticeft_exists(context, "샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_TRUE);
	}
		mkdir("result");
	{
		int ret = noticeft_receiveFile(context, "샌주.png", "result/샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}

	{
		int ret = noticeft_removeFile(context, "샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	{
		int ret = noticeft_exists(context, "샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_FALSE);
	}

	{

		int ret = noticeft_sendFile(context, "./resource/infinity_sans.PNG", "infinity_sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	{
		int ret = noticeft_renameFile(context, "infinity_sans.png", "sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	{
		int ret = noticeft_exists(context, "infinity_sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_FALSE);
	}
	{
		int ret = noticeft_exists(context, "sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_TRUE);
	}
	{
		int ret = noticeft_receiveFile(context, "sans.png", "result/sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	
	noticeft_logout(context);
}

TEST(NoticeFTTest, ReceiveSentfileInFTPS)
{
	char* ip = "127.0.0.1";
	int port = 22;
	char* id = "test";
	char* password = "test";


	auto context = noticeft_login(ip, port, id, password, true);

	{

		int ret = noticeft_sendFile(context, "./resource/샌주.PNG", "샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	{
		int ret = noticeft_exists(context, "샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_TRUE);
	}
	mkdir("result");
	{
		int ret = noticeft_receiveFile(context, "샌주.png", "result/샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}

	{
		int ret = noticeft_removeFile(context, "샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	{
		int ret = noticeft_exists(context, "샌주.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_FALSE);
	}

	{

		int ret = noticeft_sendFile(context, "./resource/infinity_sans.PNG", "infinity_sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	{
		int ret = noticeft_renameFile(context, "infinity_sans.png", "sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}
	{
		int ret = noticeft_exists(context, "infinity_sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_FALSE);
	}
	{
		int ret = noticeft_exists(context, "sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_TRUE);
	}
	{
		int ret = noticeft_receiveFile(context, "sans.png", "result/sans.png");
		ASSERT_EQ(ret, NOTICEFT_ERR_SUCCESS);
	}

	noticeft_logout(context);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}