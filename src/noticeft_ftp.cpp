#include "noticeft.h"
#include <curl/curl.h>
#include <cstring>
#include <cstdio>

static constexpr size_t MAX_URL_SIZE = 256;

struct _NoticeFT
{
    CURL* curl;
    char base_url[MAX_URL_SIZE];
    size_t base_url_size;

    char id[256];
    char password[256];
    bool useTLS;
};

struct _NoticeFTDirectory
{

};

static struct _NoticeFT _context;

static inline int buildURL(NoticeFT context, const char* remote_path, char* dest)
{
    size_t len = _context.base_url_size + strlen(remote_path) + 1;
    if (len >= MAX_URL_SIZE) return NOTICEFT_ERR_PATH_IS_TOO_LONG;
    snprintf(dest, len, "%s%s", context->base_url, remote_path);
    return 0;
}

static inline void deviceDirectoryAndName(const char* path, char* directory, char* filename)
{
    const char* lastSlash = strrchr(path, '/'); // 경로에서 마지막 슬래시 찾기 (POSIX 경로 기준)

    if (lastSlash == nullptr) {
        // 슬래시가 없다면, 전체가 파일 이름이라고 간주
        strcpy(directory, ""); // 디렉터리 부분은 비어있음
        strcpy(filename, path);
    }
    else {
        // 디렉터리와 파일 이름 분리
        size_t dirLength = lastSlash - path; // 디렉터리 부분 길이 계산
        strncpy(directory, path, dirLength); // 디렉터리 부분 복사
        directory[dirLength] = '\0';         // 문자열 끝에 널 문자 추가

        strcpy(filename, lastSlash + 1);    // 파일 이름 복사
    }
}

#define DEBUG_CURLMSG 1
class CurlDebugger
{
public:
    explicit CurlDebugger(CURL* curl) {
 #ifdef DEBUG_CURLMSG
        file = fopen("test.txt", "wb");
        curl_easy_setopt(curl, CURLOPT_STDERR, file);
#endif
    }
    ~CurlDebugger() {
#ifdef DEBUG_CURLMSG
        fclose(file);
#endif
    }


private:
#ifdef DEBUG_CURLMSG
    FILE* file{ nullptr };
#endif
};

static inline void enableTLS(NoticeFT context)
{

     curl_easy_setopt(context->curl, CURLOPT_VERBOSE, 1L);
         
    // curl_version_info_data* version_info = curl_version_info(CURLVERSION_NOW);
    // printf("ssl library : %s", version_info->ssl_version);

    if (context->useTLS)
    {
        curl_easy_setopt(context->curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(context->curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(context->curl, CURLOPT_USE_SSL, CURLUSESSL_TRY);
        curl_easy_setopt(context->curl, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
    }
}

static NoticeFT noticeft_loginImpl(const char* ip_address, int port, const char* id, const char* password, bool useTLS)
{


    curl_global_init(CURL_GLOBAL_DEFAULT);
    _context.curl = curl_easy_init();
    if (!_context.curl) return nullptr;

    memset(_context.base_url, 0, sizeof(_context.base_url));
    snprintf(
        _context.base_url,
        sizeof(_context.base_url),
        "ftp://%s:%d/",
        //useTLS ? "s" : "",
        ip_address,
        port);

    _context.base_url_size = strlen(_context.base_url);

    memset(_context.id, 0, 256);
    memset(_context.password, 0, 256);
    strncpy(_context.id, id, 256);
    strncpy(_context.password, password, 256);

    _context.useTLS = useTLS;

    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, _context.base_url, _context.id, _context.useTLS ? "enable" : "disable");
    return &_context;
}
static int noticeft_logoutImpl(NoticeFT context)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");

    if (context == nullptr) return NOTICEFT_ERR_WRONG_CONTEXT;
    curl_easy_cleanup(context->curl);
    curl_global_cleanup();
    return NOTICEFT_ERR_SUCCESS;
}



static int noticeft_sendFileImpl(NoticeFT context, const char* local_path, const char* remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : local_path (%s) remote_path (%s)", __FUNCTION__, local_path, remote_path);

    if (context == nullptr) return NOTICEFT_ERR_WRONG_CONTEXT;

    char url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_path, url);
        if (ret < 0)
            return ret;
    }

    FILE* file = fopen(local_path, "rb");
    if (!file) {
        return NOTICEFT_ERR_CANNOT_OPEN_FILE;
    }

    curl_easy_reset(context->curl);
    curl_easy_setopt(context->curl, CURLOPT_USERNAME, context->id);
    curl_easy_setopt(context->curl, CURLOPT_PASSWORD, context->password);
    curl_easy_setopt(context->curl, CURLOPT_URL, url);

    enableTLS(context);

    curl_easy_setopt(context->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(context->curl, CURLOPT_READDATA, file);

    {
        CURLcode res = curl_easy_perform(context->curl);
        
        fclose(file);

        if (res != CURLE_OK)
        {
            return NOTICEFT_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return NOTICEFT_ERR_SUCCESS;
}

static size_t writeToFile(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

static int noticeft_receiveFileImpl(NoticeFT context, const char* remote_path, const char* local_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : local_path (%s) remote_path (%s)", __FUNCTION__, local_path, remote_path);

    if (context == nullptr) return NOTICEFT_ERR_WRONG_CONTEXT;

    char url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_path, url);
        if (ret < 0)
            return ret;
    }

    FILE* file = fopen(local_path, "wb");
    if (!file) {
        return NOTICEFT_ERR_CANNOT_OPEN_FILE;
    }

    curl_easy_reset(context->curl);
    curl_easy_setopt(context->curl, CURLOPT_USERNAME, context->id);
    curl_easy_setopt(context->curl, CURLOPT_PASSWORD, context->password);

    enableTLS(context);
    curl_easy_setopt(context->curl, CURLOPT_URL, url);
    curl_easy_setopt(context->curl, CURLOPT_WRITEDATA, file);

    {
        CURLcode res = curl_easy_perform(context->curl);
        fclose(file);

        if (res != CURLE_OK)
        {
            return NOTICEFT_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return NOTICEFT_ERR_SUCCESS;
}
static int noticeft_existsImpl(NoticeFT context, const char* remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : remote_path (%s)", __FUNCTION__, remote_path);

    if (context == nullptr) return NOTICEFT_ERR_WRONG_CONTEXT;

    char url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_path, url);
        if (ret < 0)
            return ret;
    }

    curl_easy_reset(context->curl);
    curl_easy_setopt(context->curl, CURLOPT_USERNAME, context->id);
    curl_easy_setopt(context->curl, CURLOPT_PASSWORD, context->password);

    enableTLS(context);
    curl_easy_setopt(context->curl, CURLOPT_URL, url);
    curl_easy_setopt(context->curl, CURLOPT_NOBODY, 1L);

    {
        CURLcode res = curl_easy_perform(context->curl);
        if (res == CURLE_REMOTE_FILE_NOT_FOUND)
        {
            return NOTICEFT_ERR_FALSE;
        }
        else if (res != CURLE_OK)
        {
            return NOTICEFT_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return NOTICEFT_ERR_TRUE;
}
static int noticeft_removeFileImpl(NoticeFT context, const char* remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : remote_path (%s)", __FUNCTION__, remote_path);

    if (context == nullptr) return NOTICEFT_ERR_WRONG_CONTEXT;

    char remote_directory_path[256];
    char remote_filename[256];
    deviceDirectoryAndName(remote_path, remote_directory_path, remote_filename);

    char url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_path, remote_directory_path);
        if (ret < 0)
            return ret;
    }

    curl_easy_reset(context->curl);
    curl_easy_setopt(context->curl, CURLOPT_USERNAME, context->id);
    curl_easy_setopt(context->curl, CURLOPT_PASSWORD, context->password);

    enableTLS(context);
    curl_easy_setopt(context->curl, CURLOPT_URL, context->base_url);

    char req_buffer[256];
    snprintf(req_buffer, 256, "DELE %s", remote_filename);
    curl_easy_setopt(context->curl, CURLOPT_CUSTOMREQUEST, req_buffer);

    {
        CURLcode res = curl_easy_perform(context->curl);
        if (res == CURLE_FTP_COULDNT_RETR_FILE)
        {
            // SKIP
        }
        else if (res != CURLE_OK)
        {
            return NOTICEFT_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return NOTICEFT_ERR_SUCCESS;
}
static int noticeft_renameFileImpl(NoticeFT context, const char* old_remote_path, const char* new_remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : old_remote_path (%s) new_remote_path (%s)", __FUNCTION__, old_remote_path, new_remote_path);

    if (context == nullptr) return NOTICEFT_ERR_WRONG_CONTEXT;

    char remote_old_directory_path[256];
    char remote_old_filename[256];
    deviceDirectoryAndName(old_remote_path, remote_old_directory_path, remote_old_filename);
    char old_url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_old_directory_path, old_url);
        if (ret < 0)
            return ret;
    }

    char remote_new_directory_path[256];
    char remote_new_filename[256];
    deviceDirectoryAndName(new_remote_path, remote_new_directory_path, remote_new_filename);
    char new_url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_new_directory_path, new_url);
        if (ret < 0)
            return ret;
    }


    curl_easy_reset(context->curl);
    curl_easy_setopt(context->curl, CURLOPT_USERNAME, context->id);
    curl_easy_setopt(context->curl, CURLOPT_PASSWORD, context->password);

    enableTLS(context);
    curl_easy_setopt(context->curl, CURLOPT_URL, old_url);

    char req_buffer1[256];
    snprintf(req_buffer1, 256, "RNFR %s", remote_old_filename);
    curl_easy_setopt(context->curl, CURLOPT_CUSTOMREQUEST, req_buffer1);

    {
        CURLcode res = curl_easy_perform(context->curl);
        if (res == CURLE_FTP_COULDNT_RETR_FILE)
        {
            // SKIP
        }
        else if (res != CURLE_OK)
        {
            return NOTICEFT_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    curl_easy_setopt(context->curl, CURLOPT_URL, new_url);
    char req_buffer2[256];
    snprintf(req_buffer2, 256, "RNTO %s", remote_new_filename);
    curl_easy_setopt(context->curl, CURLOPT_CUSTOMREQUEST, req_buffer2);

    {
        CURLcode res = curl_easy_perform(context->curl);
        if (res == CURLE_FTP_COULDNT_RETR_FILE)
        {
            // SKIP
        }
        else if (res != CURLE_OK)
        {
            return NOTICEFT_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return NOTICEFT_ERR_SUCCESS;
}

static NoticeFTDirectory noticeft_openDirectoryImpl(NoticeFT context, const char* remote_path)
{
    static _NoticeFTDirectory temp;
    return &temp;
}
static int noticeft_readDirectoryImpl(NoticeFT context, NoticeFTDirectory directory)
{
    return 0;
}
static void noticeft_closeDirectoryImpl(NoticeFT context, NoticeFTDirectory directory)
{
    return;
}