#include "noticeft.h"

#ifdef NOTICEFT_USING_FTP
#include "noticeft_ftp.cpp"
#endif


NoticeFT noticeft_login(const char* ip_address, int port, const char* id, const char* password, bool useTLS)
{
    return noticeft_loginImpl(ip_address, port, id, password, useTLS);
}
int noticeft_logout(NoticeFT context)
{
    return noticeft_logoutImpl(context);

}
int noticeft_sendFile(NoticeFT context, const char* local_path, const char* remote_path)
{
    return noticeft_sendFileImpl(context, local_path, remote_path);
}
int noticeft_receiveFile(NoticeFT context, const char* remote_path, const char* local_path)
{
    return noticeft_receiveFileImpl(context, remote_path, local_path);
}
int noticeft_exists(NoticeFT context, const char* remote_path)
{
    return noticeft_existsImpl(context, remote_path);
}
int noticeft_removeFile(NoticeFT context, const char* remote_path)
{
    return noticeft_removeFileImpl(context, remote_path);
}
int noticeft_renameFile(NoticeFT context, const char* old_remote_path, const char* new_remote_path)
{
    return noticeft_renameFileImpl(context, old_remote_path, new_remote_path);
}

NoticeFTDirectory noticeft_openDirectory(NoticeFT context, const char* remote_path)
{
    return noticeft_openDirectoryImpl(context, remote_path);

}
int noticeft_readDirectory(NoticeFT context, NoticeFTDirectory directory)
{
    return noticeft_readDirectoryImpl(context, directory);

}
void noticeft_closeDirectory(NoticeFT context, NoticeFTDirectory directory)
{
    return noticeft_closeDirectoryImpl(context, directory);
}