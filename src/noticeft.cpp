#include "noticeft.h"

#ifdef NOTICEFT_USING_FTP
#include "noticeft_ftp.cpp"
#endif

NoticeFT CC noticeft_login(const char* ip_address, int port, const char* id, const char* password, int useTLS)
{
    NoticeFT ret = noticeft_loginImpl(ip_address, port, id, password, useTLS > 0);
    return ret;
}
int CC noticeft_logout(NoticeFT context)
{
    return noticeft_logoutImpl(context);

}
int CC noticeft_sendFile(NoticeFT context, const char* local_path, const char* remote_path)
{
    return noticeft_sendFileImpl(context, local_path, remote_path);
}
int CC noticeft_receiveFile(NoticeFT context, const char* remote_path, const char* local_path)
{
    return noticeft_receiveFileImpl(context, remote_path, local_path);
}
int CC noticeft_exists(NoticeFT context, const char* remote_path)
{
    return noticeft_existsImpl(context, remote_path);
}
int CC noticeft_removeFile(NoticeFT context, const char* remote_path)
{
    return noticeft_removeFileImpl(context, remote_path);
}
int CC noticeft_renameFile(NoticeFT context, const char* old_remote_path, const char* new_remote_path)
{
    return noticeft_renameFileImpl(context, old_remote_path, new_remote_path);
}

/*
NoticeFTDirectory STDCALL noticeft_openDirectory(NoticeFT context, const char* remote_path)
{
    return noticeft_openDirectoryImpl(context, remote_path);

}
int STDCALL noticeft_readDirectory(NoticeFT context, NoticeFTDirectory directory)
{
    return noticeft_readDirectoryImpl(context, directory);

}
void STDCALL noticeft_closeDirectory(NoticeFT context, NoticeFTDirectory directory)
{
    return noticeft_closeDirectoryImpl(context, directory);
}
*/

void CC noticeft_print(const char* msg)
{
    MessageBoxA(NULL, msg, "AlimJang!", MB_OK | MB_ICONINFORMATION);
    // printf("%s", msg);
}