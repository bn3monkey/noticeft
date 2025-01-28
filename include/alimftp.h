#ifndef ALIMFTP_H
#define ALIMFTP_H

#if defined(_WIN32) || defined(_WIN64) // Windows
    #if defined(ALIMFTP_STATIC)
        #define ALIMFTP_API 
    #elif defined(ALIMFTP_EXPORTS)
        #define ALIMFTP_API __declspec(dllexport)
    #else
        #define ALIMFTP_API __declspec(dllimport)
    #endif
    #define CC __stdcall
#elif defined(__linux__) || defined(__unix__) || defined(__ANDROID__) // Linux / Android
    #if defined(ALIMFTP_STATIC)
        #define ALIMFTP_API 
    #elif defined(ALIMFTP_EXPORTS)
        #define ALIMFTP_API __attribute__((visibility("default")))
    #else
        #define ALIMFTP_API
    #endif
#else 
    #define ALIMFTP_API
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#ifdef _WIN64
#include <cassert>
assert("This library is only for 32bit computer");
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _AlimFTP* AlimFTP;
typedef struct _AlimFTPDirectory* AlimFTPDirectory;

#define ALIMFTP_ERR_SUCCESS 0
#define ALIMFTP_ERR_TRUE 1
#define ALIMFTP_ERR_FALSE 0

#define ALIMFTP_ERR_WRONG_CONTEXT -10
#define ALIMFTP_ERR_PATH_IS_TOO_LONG -11
#define ALIMFTP_ERR_CANNOT_OPEN_FILE -12
#define ALIMFTP_ERR_CANNOT_EXECUTE_COMMAND -13
#define ALIMFTP_ERR_WRONG_DIRECTORY -14

ALIMFTP_API AlimFTP CC ftp_login(const char* ip_address, int port, const char* id, const char* password, int useTLS);
ALIMFTP_API int CC ftp_logout(AlimFTP context);

// 949 : EUC-KR  & 65001 : UTF-8  
ALIMFTP_API void CC ftp_setCodepage(int code_page); 

ALIMFTP_API int CC ftp_sendFile(AlimFTP context, const char* local_path, const char* remote_path);
ALIMFTP_API int CC ftp_receiveFile(AlimFTP context, const char* remote_path, const char* local_path);
ALIMFTP_API int CC ftp_exists(AlimFTP context, const char* remote_path);
ALIMFTP_API int CC ftp_removeFile(AlimFTP context, const char* remote_path);
ALIMFTP_API int CC ftp_renameFile(AlimFTP context, const char* old_remote_path, const char* new_remote_path);

ALIMFTP_API AlimFTPDirectory CC ftp_openDirectory(AlimFTP context, const char* remote_path);
ALIMFTP_API int CC ftp_numberOfFilesInDirectory(AlimFTP context, AlimFTPDirectory directory);
ALIMFTP_API const char* CC ftp_fileNameInDirectory(AlimFTP context, AlimFTPDirectory directory, size_t index);
ALIMFTP_API const char* CC ftp_fileDateInDirectory(AlimFTP context, AlimFTPDirectory directory, size_t index);
ALIMFTP_API int CC ftp_closeDirectory(AlimFTP context, AlimFTPDirectory directory);

#ifdef __cplusplus
}
#endif

#endif // ALIMFTP_H