#include <gtest/gtest.h>
#include <alimftp.h>
#include <cstdio>

TEST(NoticeFTTest, ReceiveSentfileInFTP)
{
	/*
	char* ip = "127.0.0.1";
	int port = 21;
	char* id = "test";
	char* password = "test";
	*/

	char* ip = "bong.byus.net";
	int port = 21;
	char* id = "bong";
	char* password = "by5620";

	// Encoding : EUC-KR

	ftp_setCodepage(949);
	auto context = ftp_login(ip, port, id, password, false);
	
	{

		int ret = ftp_sendFile(context, "./resource/샌주.PNG", "샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}
	{
		int ret = ftp_exists(context, "샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_TRUE);
	}
		mkdir("result");
	{
		int ret = ftp_receiveFile(context, "샌주.png", "result/샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}

	{
		int ret = ftp_removeFile(context, "샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}
	{
		int ret = ftp_exists(context, "샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_FALSE);
	}

	{

		int ret = ftp_sendFile(context, "./resource/infinity_sans.PNG", "test/infinity_sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}
	{
		int ret = ftp_renameFile(context, "test/infinity_sans.png", "test/sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}
	{
		int ret = ftp_exists(context, "test/infinity_sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_FALSE);
	}
	{
		int ret = ftp_exists(context, "test/sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_TRUE);
	}
	{
		int ret = ftp_receiveFile(context, "test/sans.png", "result/sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}


	{
		auto directory = ftp_openDirectory(context, "test/");
		ASSERT_NE(directory, nullptr);

		auto length = ftp_numberOfFilesInDirectory(context, directory);
		ASSERT_NE(length, 0);


		ftp_setCodepage(65001);
		for (int i = 0; i < length; i++)
		{
			const char* name = ftp_fileNameInDirectory(context, directory, i);
			const char* date = ftp_fileDateInDirectory(context, directory, i);
			printf("name : %s date: %s\n", name, date);
		}

		ftp_closeDirectory(context, directory);
	}
	
	ftp_logout(context);
}

TEST(NoticeFTTest, ReceiveSentfileInFTPS)
{
	return; 

	char* ip = "127.0.0.1";
	int port = 22;
	char* id = "test";
	char* password = "test";

	auto context = ftp_login(ip, port, id, password, true);

	{

		int ret = ftp_sendFile(context, "./resource/샌주.PNG", "샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}
	{
		int ret = ftp_exists(context, "샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_TRUE);
	}
	mkdir("result");
	{
		int ret = ftp_receiveFile(context, "샌주.png", "result/샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}

	{
		int ret = ftp_removeFile(context, "샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}
	{
		int ret = ftp_exists(context, "샌주.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_FALSE);
	}

	{

		int ret = ftp_sendFile(context, "./resource/infinity_sans.PNG", "infinity_sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}
	{
		int ret = ftp_renameFile(context, "infinity_sans.png", "sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}
	{
		int ret = ftp_exists(context, "infinity_sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_FALSE);
	}
	{
		int ret = ftp_exists(context, "sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_TRUE);
	}
	{
		int ret = ftp_receiveFile(context, "sans.png", "result/sans.png");
		ASSERT_EQ(ret, ALIMFTP_ERR_SUCCESS);
	}

	ftp_logout(context);
}

int main(int argc, char** argv)
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}