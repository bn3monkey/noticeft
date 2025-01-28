// Wrapping noticeft.dll for xHarbour

// Please check the name of function with dumpbin /exports by visual studio command
// Please set the calling convetion as stdcall

DLL32 FUNCTION ftp_login(ip_address AS STRING, port AS LONG, id AS STRING, password AS STRING, usetls AS LONG ) AS LONG PASCAL FROM "_ftp_login@20" LIB "alimftp.dll"
DLL32 FUNCTION ftp_logout(context AS LONG) AS VOID PASCAL FROM " _ftp_logout@4" LIB "alimftp.dll"

DLL32 FUNCTION ftp_sendFile(context AS LONG, local_path AS STRING, remote_path AS STRING) AS LONG PASCAL FROM "_ftp_sendFile@12" LIB "alimftp.dll"
DLL32 FUNCTION ftp_receiveFile(context AS LONG, remote_path AS STRING, local_path AS STRING) AS LONG PASCAL FROM "_ftp_receiveFile@12" LIB "alimftp.dll"
DLL32 FUNCTION ftp_exists(context AS LONG, filepath AS STRING) AS LONG PASCAL FROM "_ftp_exists@8" LIB "alimftp.dll"
DLL32 FUNCTION ftp_removeFile(context AS LONG, remote_path AS STRING) AS LONG PASCAL FROM "_ftp_removeFile@8" LIB "alimftp.dll"
DLL32 FUNCTION ftp_renameFile(context AS LONG, old_remote_path AS STRING, new_remote_path AS STRING) AS LONG PASCAL FROM "_ftp_renameFile@12" LIB "alimftp.dll"

DLL32 FUNCTION ftp_openDirectory(context AS LONG, remote_path AS STRING) AS LONG PASCAL FROM "_ftp_openDirectory@8" LIB "alimftp.dll"
DLL32 FUNCTION ftp_closeDirectory(context AS LONG, directory AS LONG) AS LONG PASCAL FROM "_ftp_closeDirectory@8" LIB "alimftp.dll"

DLL32 FUNCTION ftp_numberOfFilesInDirectory(context AS LONG, directory AS LONG) AS LONG PASCAL FROM "_ftp_numberOfFilesInDirectory@8" LIB "alimftp.dll"
DLL32 FUNCTION ftp_fileNameInDirectory(context AS LONG, directory AS LONG, INDEX AS LONG) AS STRING PASCAL FROM "_ftp_fileNameInDirectory@12" LIB "alimftp.dll"
DLL32 FUNCTION ftp_fileDateInDirectory(context AS LONG, directory AS LONG, INDEX AS LONG) AS STRING PASCAL FROM "_ftp_fileDateInDirectory@12" LIB "alimftp.dll"