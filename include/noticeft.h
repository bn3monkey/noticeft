#ifndef NOTICEFT_H
#define NOTICEFT_H

#ifdef _WIN32
    #ifdef NOTICEFT_EXPORTS
        // DLL을 빌드할 때: NOTICEFT_EXPORTS가 정의되어야 함.
        #define NOTICEFT_API __declspec(dllexport)
    #else
        // DLL을 사용하는 경우
        #define NOTICEFT_API __declspec(dllimport)
    #endif
    #define CDECL __cdecl
#else
    // 다른 플랫폼 (Linux/Unix 등)
    #define NOTICEFT_API
    #define CDECL
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _NoticeFT* NoticeFT;
typedef struct _NoticeFTDirectory* NoticeFTDirectory;

#define NOTICEFT_ERR_SUCCESS 0
#define NOTICEFT_ERR_TRUE 1
#define NOTICEFT_ERR_FALSE 0

#define NOTICEFT_ERR_WRONG_CONTEXT -10
#define NOTICEFT_ERR_PATH_IS_TOO_LONG -11
#define NOTICEFT_ERR_CANNOT_OPEN_FILE -12
#define NOTICEFT_ERR_CANNOT_EXECUTE_COMMAND -13

NOTICEFT_API NoticeFT CDECL noticeft_login(const char* ip_address, int port, const char* id, const char* password, bool useTLS);
NOTICEFT_API int CDECL noticeft_logout(NoticeFT context);

NOTICEFT_API int CDECL noticeft_sendFile(NoticeFT context, const char* local_path, const char* remote_path);
NOTICEFT_API int CDECL noticeft_receiveFile(NoticeFT context, const char* remote_path, const char* local_path);
NOTICEFT_API int CDECL noticeft_exists(NoticeFT context, const char* remote_path);
NOTICEFT_API int CDECL noticeft_removeFile(NoticeFT context, const char* remote_path);
NOTICEFT_API int CDECL noticeft_renameFile(NoticeFT context, const char* old_remote_path, const char* new_remote_path);

NOTICEFT_API NoticeFTDirectory CDECL noticeft_openDirectory(NoticeFT context, const char* remote_path);
NOTICEFT_API int CDECL noticeft_readDirectory(NoticeFT context, NoticeFTDirectory directory);
NOTICEFT_API void CDECL noticeft_closeDirectory(NoticeFT context, NoticeFTDirectory directory);


#ifdef __cplusplus
}
#endif

#endif // NOTICEFT_H