#ifndef NOTICEFT_H
#define NOTICEFT_H

#if defined(_WIN32) || defined(_WIN64) // Windows
    #if defined(NOTICEFT_STATIC)
        #define NOTICEFT_API 
    #elif defined(NOTICEFT_EXPORTS)
        #define NOTICEFT_API __declspec(dllexport)
    #else
        #define NOTICEFT_API __declspec(dllimport)
    #endif
    #define CC __stdcall
#elif defined(__linux__) || defined(__unix__) || defined(__ANDROID__) // Linux / Android
    #if defined(NOTICEFT_STATIC)
        #define NOTICEFT_API 
    #elif defined(NOTICEFT_EXPORTS)
        #define NOTICEFT_API __attribute__((visibility("default")))
    #else
        #define NOTICEFT_API
    #endif
#else 
    #define NOTICEFT_API
    #pragma warning Unknown dynamic link import/export semantics.
#endif

#ifdef _WIN64
#include <cassert>
assert("This library is only for 32bit computer");
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

NOTICEFT_API NoticeFT CC noticeft_login(const char* ip_address, int port, const char* id, const char* password, int useTLS);
NOTICEFT_API int CC noticeft_logout(NoticeFT context);

NOTICEFT_API int CC noticeft_sendFile(NoticeFT context, const char* local_path, const char* remote_path);
NOTICEFT_API int CC noticeft_receiveFile(NoticeFT context, const char* remote_path, const char* local_path);
NOTICEFT_API int CC noticeft_exists(NoticeFT context, const char* remote_path);
NOTICEFT_API int CC noticeft_removeFile(NoticeFT context, const char* remote_path);
NOTICEFT_API int CC noticeft_renameFile(NoticeFT context, const char* old_remote_path, const char* new_remote_path);

NOTICEFT_API void CC noticeft_print(const char* msg);

#ifdef __cplusplus
}
#endif

#endif // NOTICEFT_H