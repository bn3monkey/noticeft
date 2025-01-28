#include "alimftp.h"
#include <curl/curl.h>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>

static constexpr size_t MAX_URL_SIZE = 256;

struct _AlimFTP
{
    CURL* curl;
    char base_url[MAX_URL_SIZE];
    size_t base_url_size;

    char id[256];
    char password[256];
    bool useTLS;
};

struct AlimRemoteFile
{
    char filename[256] {0};
    char filedate[256] {0};
};

struct _AlimFTPDirectory
{
    std::vector<AlimRemoteFile> files;
};

static struct _AlimFTP _context;


// Helper function to check if the string has an extension
static inline int hasExtension(const char* url) {
    const char* dot = strrchr(url, '.'); // Find the last dot in the string

    if (!dot || dot == url) {
        return 0; // No dot found, or dot is the first character (not an extension)
    }

    // Check if the dot is part of a valid extension (e.g., alphanumeric after the dot)
    const char* extension = dot + 1;
    while (*extension) {
        if (!isalnum(*extension)) {
            return 0; // Invalid character in the extension
        }
        extension++;
    }

    return 1; // Valid extension found
}

// Function to append '/' if needed
static inline void ensureTrailingSlash(char* url, size_t max_length) {
    size_t len = strlen(url);

    // Check if URL already ends with '/' or has an extension
    if (len > 0 && url[len - 1] != '/' && !hasExtension(url)) {
        if (len + 1 < max_length) { // Ensure there is space for the '/'
            strcat(url, "/");
        }
        else {
            printf("Error: Not enough space to add trailing slash.\n");
        }
    }
}

static inline int buildURL(AlimFTP context, const char* remote_path, char* dest)
{
    size_t len = _context.base_url_size + strlen(remote_path) + 1;
    if (len >= MAX_URL_SIZE-1) return ALIMFTP_ERR_PATH_IS_TOO_LONG;
    snprintf(dest, len, "%s%s", context->base_url, remote_path);
    ensureTrailingSlash(dest, MAX_URL_SIZE);
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
    explicit CurlDebugger(CURL* curl, bool is_text) : is_text(is_text), is_euc_kr(is_euc_kr) {
        if (is_text)
        {
            file = fopen("test.txt", "wb");
            curl_easy_setopt(curl, CURLOPT_STDERR, file);
        }
    }
    ~CurlDebugger() {
        if (is_text)
        {
            fclose(file);
        }
    }

private:
    FILE* file{ nullptr };
    bool is_text;
    bool is_euc_kr;
};

static inline void enableTLS(AlimFTP context)
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

static int& ftp_getCodePageImpl() {
    static int ret{ 0 };
    return ret;

}
static void ftp_setCodepageImpl(int code_page)
{
    int& ret = ftp_getCodePageImpl();
    ret = code_page;
    SetConsoleOutputCP(code_page);
}

static AlimFTP ftp_loginImpl(const char* ip_address, int port, const char* id, const char* password, bool useTLS)
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

    if (_context.curl) {
        curl_easy_setopt(_context.curl, CURLOPT_URL, _context.base_url);
        curl_easy_setopt(_context.curl, CURLOPT_USERNAME, _context.id);
        curl_easy_setopt(_context.curl, CURLOPT_PASSWORD, _context.password);

        enableTLS(&_context);

        auto res = curl_easy_perform(_context.curl);

        if (res != CURLE_OK) {
            curl_easy_cleanup(_context.curl);
            return nullptr;
        }

    }

    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, _context.base_url, _context.id, _context.useTLS ? "enable" : "disable");
    return &_context;
}
static int ftp_logoutImpl(AlimFTP context)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");

    if (context == nullptr) return ALIMFTP_ERR_WRONG_CONTEXT;
    curl_easy_cleanup(context->curl);
    curl_global_cleanup();
    return ALIMFTP_ERR_SUCCESS;
}



static int ftp_sendFileImpl(AlimFTP context, const char* local_path, const char* remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : local_path (%s) remote_path (%s)", __FUNCTION__, local_path, remote_path);

    if (context == nullptr) return ALIMFTP_ERR_WRONG_CONTEXT;

    char url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_path, url);
        if (ret < 0)
            return ret;
    }

    FILE* file = fopen(local_path, "rb");
    if (!file) {
        return ALIMFTP_ERR_CANNOT_OPEN_FILE;
    }

    curl_easy_reset(context->curl);
    curl_easy_setopt(context->curl, CURLOPT_USERNAME, context->id);
    curl_easy_setopt(context->curl, CURLOPT_PASSWORD, context->password);
    curl_easy_setopt(context->curl, CURLOPT_URL, url);

    enableTLS(context);

    curl_easy_setopt(context->curl, CURLOPT_UPLOAD, 1L);
    curl_easy_setopt(context->curl, CURLOPT_READDATA, file);

    {
        CurlDebugger debugger(_context.curl, false);
        CURLcode res = curl_easy_perform(context->curl);
        
        fclose(file);

        if (res != CURLE_OK)
        {
            return ALIMFTP_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return ALIMFTP_ERR_SUCCESS;
}

static size_t writeToFile(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

static int ftp_receiveFileImpl(AlimFTP context, const char* remote_path, const char* local_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : local_path (%s) remote_path (%s)", __FUNCTION__, local_path, remote_path);

    if (context == nullptr) return ALIMFTP_ERR_WRONG_CONTEXT;

    char url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_path, url);
        if (ret < 0)
            return ret;
    }

    FILE* file = fopen(local_path, "wb");
    if (!file) {
        return ALIMFTP_ERR_CANNOT_OPEN_FILE;
    }

    curl_easy_reset(context->curl);
    curl_easy_setopt(context->curl, CURLOPT_USERNAME, context->id);
    curl_easy_setopt(context->curl, CURLOPT_PASSWORD, context->password);

    enableTLS(context);
    curl_easy_setopt(context->curl, CURLOPT_URL, url);
    curl_easy_setopt(context->curl, CURLOPT_WRITEDATA, file);

    {
        CurlDebugger debugger(_context.curl, false);
        CURLcode res = curl_easy_perform(context->curl);
        fclose(file);

        if (res != CURLE_OK)
        {
            return ALIMFTP_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return ALIMFTP_ERR_SUCCESS;
}
static int ftp_existsImpl(AlimFTP context, const char* remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : remote_path (%s)", __FUNCTION__, remote_path);

    if (context == nullptr) return ALIMFTP_ERR_WRONG_CONTEXT;

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
        CurlDebugger debugger(_context.curl, false);
        CURLcode res = curl_easy_perform(context->curl);
        if (res == CURLE_REMOTE_FILE_NOT_FOUND)
        {
            return ALIMFTP_ERR_FALSE;
        }
        else if (res != CURLE_OK)
        {
            return ALIMFTP_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return ALIMFTP_ERR_TRUE;
}
static int ftp_removeFileImpl(AlimFTP context, const char* remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : remote_path (%s)", __FUNCTION__, remote_path);

    if (context == nullptr) return ALIMFTP_ERR_WRONG_CONTEXT;

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
        CurlDebugger debugger(_context.curl, false);
        CURLcode res = curl_easy_perform(context->curl);
        if (res == CURLE_FTP_COULDNT_RETR_FILE)
        {
            // SKIP
        }
        else if (res != CURLE_OK)
        {
            return ALIMFTP_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return ALIMFTP_ERR_SUCCESS;
}
static int ftp_renameFileImpl(AlimFTP context, const char* old_remote_path, const char* new_remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : old_remote_path (%s) new_remote_path (%s)", __FUNCTION__, old_remote_path, new_remote_path);

    if (context == nullptr) return ALIMFTP_ERR_WRONG_CONTEXT;

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
        CurlDebugger debugger(_context.curl, false);
        CURLcode res = curl_easy_perform(context->curl);
        if (res == CURLE_FTP_COULDNT_RETR_FILE)
        {
            // SKIP
        }
        else if (res != CURLE_OK)
        {
            return ALIMFTP_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    curl_easy_setopt(context->curl, CURLOPT_URL, new_url);
    char req_buffer2[256];
    snprintf(req_buffer2, 256, "RNTO %s", remote_new_filename);
    curl_easy_setopt(context->curl, CURLOPT_CUSTOMREQUEST, req_buffer2);

    {
        CurlDebugger debugger(_context.curl, false);
        CURLcode res = curl_easy_perform(context->curl);
        if (res == CURLE_FTP_COULDNT_RETR_FILE)
        {
            // SKIP
        }
        else if (res != CURLE_OK)
        {
            return ALIMFTP_ERR_CANNOT_EXECUTE_COMMAND;
        }
    }

    return ALIMFTP_ERR_SUCCESS;
}


static size_t writeFileInformation(void* ptr, size_t size, size_t nmemb, void* userdata) {
    std::vector<char>& response = *(static_cast<std::vector<char>*>(userdata));
    char* cptr = static_cast<char*>(ptr);
    response.insert(response.end(), cptr, cptr + size * nmemb);
    response.push_back(0);
    return size * nmemb;
}


inline bool hasKey(const char* src, const char* key)
{
    return !strncmp(src, key, strlen(key));
}
inline const char* findValueLocation(const char* src, const char* key)
{
    return src + strlen(key) + sizeof('=');
}
inline const char* extractValue(const char* src, char* dest)
{
    auto* src_ptr = src;
    auto* dest_ptr = dest;
    while (*src_ptr != ';')
    {
        *dest_ptr = *src_ptr;
        src_ptr++;
        dest_ptr++;
    }
    return src_ptr + sizeof(';');
}
inline const char* extractName(const char* src, char* dest)
{
    auto* src_ptr = src + 1;
    auto* dest_ptr = dest;

    while (*src_ptr != '\r' && *src_ptr != '\n')
    {
        *dest_ptr = *src_ptr;
        src_ptr++;
        dest_ptr++;
    }
    if (*src_ptr == '\r')
        src_ptr++;
    if (*src_ptr == '\n')
        src_ptr++;
    return src_ptr;
}

/*
modify=20250128045138;perm=flcdmpe;type=cdir;unique=811U5D443F;UNIX.group=1008;UNIX.mode=0755;UNIX.owner=1008; .
modify=20250128045134;perm=flcdmpe;type=pdir;unique=811UBC4073;UNIX.group=48;UNIX.mode=0710;UNIX.owner=1008; ..
modify=20250128030935;perm=adfrw;size=60;type=file;unique=811U5D4440;UNIX.group=1008;UNIX.mode=0644;UNIX.owner=1008; Papyrus.txt
modify=20250128030935;perm=adfrw;size=20;type=file;unique=811U5D4441;UNIX.group=1008;UNIX.mode=0644;UNIX.owner=1008; SANS.txt
modify=20250128030935;perm=adfrw;size=20;type=file;unique=811U5D4442;UNIX.group=1008;UNIX.mode=0644;UNIX.owner=1008; Undyne.txt
modify=20250128045135;perm=adfrw;size=238984;type=file;unique=811U5D4443;UNIX.group=1008;UNIX.mode=0644;UNIX.owner=1008; sans.png
modify=20250128044134;perm=adfrw;size=169743;type=file;unique=811U5D4445;UNIX.group=1008;UNIX.mode=0644;UNIX.owner=1008; 슈퍼샌주.PNG
*/

static inline void parseList(AlimFTPDirectory directory, const char* data)
{
    directory->files.clear();
    auto* ptr = data;

    char date[256]{ 0 };
    char name[256]{ 0 };
    char type[256]{ 0 };
    char temp[256]{ 0 };

    while (*ptr != '\0')
    {       
        if (hasKey(ptr, "modify="))
        {
            ptr = findValueLocation(ptr, "modify");
            ptr = extractValue(ptr, date);
        }
        else if (hasKey(ptr, "perm="))
        {
            ptr = findValueLocation(ptr, "perm");
            ptr = extractValue(ptr, temp);
        }
        else if (hasKey(ptr, "size="))
        {
            ptr = findValueLocation(ptr, "size");
            ptr = extractValue(ptr, temp);
        }
        else if (hasKey(ptr, "type="))
        {
            ptr = findValueLocation(ptr, "type");
            ptr = extractValue(ptr, type);
        }
        else if (hasKey(ptr, "unique="))
        {
            ptr = findValueLocation(ptr, "unique");
            ptr = extractValue(ptr, temp);
        }
        else if (hasKey(ptr, "UNIX.group"))
        {
            ptr = findValueLocation(ptr, "UNIX.group");
            ptr = extractValue(ptr, temp);
        }
        else if (hasKey(ptr, "UNIX.mode"))
        {
            ptr = findValueLocation(ptr, "UNIX.mode");
            ptr = extractValue(ptr, temp);
        }
        else if (hasKey(ptr, "UNIX.owner"))
        {
            ptr = findValueLocation(ptr, "UNIX.owner");
            ptr = extractValue(ptr, temp);
        }
        else if (hasKey(ptr, " "))
        {
            ptr = extractName(ptr, name);
            if (!strcmp(type, "file"))
            {
                AlimRemoteFile file;
                strncpy(file.filename, name, 256);
                strncpy(file.filedate, date, 256);
                directory->files.push_back(file);

                memset(date, 0, 256);
                memset(name, 0, 256);
                memset(type, 0, 256);
                memset(temp, 0, 256);
            }
        }
    }
}

static AlimFTPDirectory ftp_openDirectoryImpl(AlimFTP context, const char* remote_path)
{
    printf("%s : url (%s) id (%s) tls (%s)\n", __FUNCTION__, context->base_url, context->id, context->useTLS ? "enable" : "disable");
    printf("%s : remote_path (%s)\n", __FUNCTION__, remote_path);

    if (context == nullptr) return nullptr;

    static constexpr size_t MAX_PDU_SIZE = 8 *1024;

    std::vector<char> response;
    response.reserve(MAX_PDU_SIZE);


    char url[MAX_URL_SIZE]{ 0 };

    {
        auto ret = buildURL(context, remote_path, url);
        if (ret < 0)
            return nullptr;
    }

    curl_easy_reset(context->curl);
    curl_easy_setopt(context->curl, CURLOPT_USERNAME, context->id);
    curl_easy_setopt(context->curl, CURLOPT_PASSWORD, context->password);
    curl_easy_setopt(context->curl, CURLOPT_URL, url);

    enableTLS(context);

    curl_easy_setopt(context->curl, CURLOPT_CUSTOMREQUEST, "MLSD");
    
    curl_easy_setopt(context->curl, CURLOPT_WRITEFUNCTION, writeFileInformation);
    curl_easy_setopt(context->curl, CURLOPT_WRITEDATA, &response);

    {

        CurlDebugger debugger(_context.curl, false);
        CURLcode res = curl_easy_perform(context->curl);
        if (res != CURLE_OK)
        {
            return nullptr;
        }
    }

    AlimFTPDirectory ret = new struct _AlimFTPDirectory();
    parseList(ret, response.data());

    return ret;

}
static int ftp_numberOfFilesInDirectoryImpl(AlimFTP context, AlimFTPDirectory directory)
{
    if (directory == nullptr)
        return ALIMFTP_ERR_WRONG_DIRECTORY;

    return (int)directory->files.size();
}
static const char* ftp_fileNameInDirectoryImpl(AlimFTP context, AlimFTPDirectory directory, size_t index)
{
    if (directory == nullptr)
        return "";

    auto num_of_files = directory->files.size();

    if (index >= num_of_files)
        return "";

    return directory->files[index].filename;
}
static const char* ftp_fileDateInDirectoryImpl(AlimFTP context, AlimFTPDirectory directory, size_t index)
{
    if (directory == nullptr)
        return "";

    auto num_of_files = directory->files.size();

    if (index >= num_of_files)
        return "";

    return directory->files[index].filedate;
}
static int ftp_closeDirectoryImpl(AlimFTP context, AlimFTPDirectory directory)
{
    if (directory == nullptr)
        return ALIMFTP_ERR_WRONG_DIRECTORY;

    delete directory;
    return ALIMFTP_ERR_SUCCESS;
}