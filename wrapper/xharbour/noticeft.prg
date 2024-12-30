// Wrapping noticeft.dll for xHarbour

// Include the DLL functions from noticeft.dll
DLL FUNCTION noticeft_login(ip_address AS STRING, port AS INT, id AS STRING, password AS STRING) AS PTR PASCAL FROM "noticeft_login" LIB "noticeft.dll"
DLL FUNCTION noticeft_logout(context AS PTR) AS VOID PASCAL FROM "noticeft_logout" LIB "noticeft.dll"

DLL FUNCTION noticeft_sendFile(context AS PTR, local_path AS STRING, remote_path AS STRING) AS INT PASCAL FROM "noticeft_sendFile" LIB "noticeft.dll"
DLL FUNCTION noticeft_receiveFile(context AS PTR, remote_path AS STRING, local_path AS STRING) AS INT PASCAL FROM "noticeft_receiveFile" LIB "noticeft.dll"
DLL FUNCTION noticeft_exists(context AS PTR, filepath AS STRING) AS INT PASCAL FROM "noticeft_exists" LIB "noticeft.dll"
DLL FUNCTION noticeft_removeFile(context AS PTR, remote_path AS STRING) AS INT PASCAL FROM "noticeft_removeFile" LIB "noticeft.dll"
DLL FUNCTION noticeft_renameFile(context AS PTR, old_remote_path AS STRING, new_remote_path AS STRING) AS INT PASCAL FROM "noticeft_renameFile" LIB "noticeft.dll"

DLL FUNCTION noticeft_openDirectory(context AS PTR, remote_path AS STRING) AS PTR PASCAL FROM "noticeft_openDirectory" LIB "noticeft.dll"
DLL FUNCTION noticeft_readDirectory(context AS PTR, directory AS PTR) AS INT PASCAL FROM "noticeft_readDirectory" LIB "noticeft.dll"
DLL FUNCTION noticeft_closeDirectory(context AS PTR, directory AS PTR) AS VOID PASCAL FROM "noticeft_closeDirectory" LIB "noticeft.dll"

// Wrapped functions for importing into other PRG files
FUNCTION Login(ip_address, port, id, password)
   RETURN noticeft_login(ip_address, port, id, password)
ENDFUNCTION

FUNCTION Logout(context)
   noticeft_logout(context)
   RETURN NIL
ENDFUNCTION

FUNCTION SendFile(context, local_path, remote_path)
   RETURN noticeft_sendFile(context, local_path, remote_path)
ENDFUNCTION

FUNCTION ReceiveFile(context, remote_path, local_path)
   RETURN noticeft_receiveFile(context, remote_path, local_path)
ENDFUNCTION

FUNCTION FileExists(context, filepath)
   RETURN noticeft_exists(context, filepath)
ENDFUNCTION

FUNCTION RemoveFile(context, remote_path)
   RETURN noticeft_removeFile(context, remote_path)
ENDFUNCTION

FUNCTION RenameFile(context, old_remote_path, new_remote_path)
   RETURN noticeft_renameFile(context, old_remote_path, new_remote_path)
ENDFUNCTION

FUNCTION OpenDirectory(context, remote_path)
   RETURN noticeft_openDirectory(context, remote_path)
ENDFUNCTION

FUNCTION ReadDirectory(context, directory)
   RETURN noticeft_readDirectory(context, directory)
ENDFUNCTION

FUNCTION CloseDirectory(context, directory)
   noticeft_closeDirectory(context, directory)
   RETURN NIL
ENDFUNCTION