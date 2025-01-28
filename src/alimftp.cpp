#include "alimftp.h"

#ifdef ALIMFTP_USING_FTP
#include "alimftp_ftp.cpp"
#endif

void CC ftp_setCodepage(int code_page)
{
    ftp_setCodepageImpl(code_page);
}
AlimFTP CC ftp_login(const char* ip_address, int port, const char* id, const char* password, int useTLS)
{
    AlimFTP ret = ftp_loginImpl(ip_address, port, id, password, useTLS > 0);
    return ret;
}
int CC ftp_logout(AlimFTP context)
{
    return ftp_logoutImpl(context);

}
int CC ftp_sendFile(AlimFTP context, const char* local_path, const char* remote_path)
{
    return ftp_sendFileImpl(context, local_path, remote_path);
}
int CC ftp_receiveFile(AlimFTP context, const char* remote_path, const char* local_path)
{
    return ftp_receiveFileImpl(context, remote_path, local_path);
}
int CC ftp_exists(AlimFTP context, const char* remote_path)
{
    return ftp_existsImpl(context, remote_path);
}
int CC ftp_removeFile(AlimFTP context, const char* remote_path)
{
    return ftp_removeFileImpl(context, remote_path);
}
int CC ftp_renameFile(AlimFTP context, const char* old_remote_path, const char* new_remote_path)
{
    return ftp_renameFileImpl(context, old_remote_path, new_remote_path);
}

AlimFTPDirectory CC ftp_openDirectory(AlimFTP context, const char* remote_path)
{
    return ftp_openDirectoryImpl(context, remote_path);
}
int CC ftp_numberOfFilesInDirectory(AlimFTP context, AlimFTPDirectory directory)
{
    return ftp_numberOfFilesInDirectoryImpl(context, directory);
}
const char* CC ftp_fileNameInDirectory(AlimFTP context, AlimFTPDirectory directory, size_t index)
{
    return ftp_fileNameInDirectoryImpl(context, directory, index);

}
const char* CC ftp_fileDateInDirectory(AlimFTP context, AlimFTPDirectory directory, size_t index)
{
    return ftp_fileDateInDirectoryImpl(context, directory, index);

}
int CC ftp_closeDirectory(AlimFTP context, AlimFTPDirectory directory)
{
    return ftp_closeDirectoryImpl(context, directory);
}