// Wrapping noticeft.dll for xHarbour

DLL32 FUNCTION noticeft_print(str AS STRING) AS VOID PASCAL FROM "_noticeft_print@4" LIB "noticeft.dll"

DLL32 FUNCTION noticeft_login(ip_address AS STRING, port AS LONG, id AS STRING, password AS STRING, usetls AS LONG ) AS LONG PASCAL FROM "_noticeft_login@20" LIB "noticeft.dll"
DLL32 FUNCTION noticeft_logout(context AS LONG) AS VOID PASCAL FROM " _noticeft_logout@4" LIB "noticeft.dll"

DLL32 FUNCTION noticeft_sendFile(context AS LONG, local_path AS STRING, remote_path AS STRING) AS LONG PASCAL FROM "_noticeft_sendFile@12" LIB "noticeft.dll"
DLL32 FUNCTION noticeft_receiveFile(context AS LONG, remote_path AS STRING, local_path AS STRING) AS LONG PASCAL FROM "_noticeft_receiveFile@12" LIB "noticeft.dll"
DLL32 FUNCTION noticeft_exists(context AS LONG, filepath AS STRING) AS LONG PASCAL FROM "_noticeft_exists@8" LIB "noticeft.dll"
DLL32 FUNCTION noticeft_removeFile(context AS LONG, remote_path AS STRING) AS LONG PASCAL FROM "_noticeft_removeFile@8" LIB "noticeft.dll"
DLL32 FUNCTION noticeft_renameFile(context AS LONG, old_remote_path AS STRING, new_remote_path AS STRING) AS LONG PASCAL FROM "_noticeft_renameFile@12" LIB "noticeft.dll"
