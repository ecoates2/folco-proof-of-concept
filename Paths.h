#ifndef PATHS_H
#define PATHS_H

#include <string>
#include <codecvt>
#include <locale>
#include <stdlib.h>
#include <windows.h>
static const std::wstring FOLCODIR = L"folco";

static const std::wstring FOLDERS = FOLCODIR + L"\\folders.txt";

static const std::wstring CACHEDIR = FOLCODIR + L"\\cache";

static const std::wstring ASSETDIR = CACHEDIR + L"\\assets";
static const std::wstring ICON_CACHE_DIR = CACHEDIR + L"\\colorized";

static const std::wstring ICON_PE_FILE = L"shell32.dll";

static const WORD FOLDER_ICON_ID = 4;


class Paths
{

    public:
        static std::wstring getAssetDirWide();
        static std::wstring getCacheDirWide();
        static std::wstring getIconCacheDirWide();
        static std::wstring getFolcoDirWide();
        static std::string getAssetDir();
        static std::string getCacheDir();
        static std::string getIconCacheDir();
        static std::string getFolcoDir();

        static std::string getFoldersTxt();

        static std::wstring getResourceFileDirWide();

    protected:

    private:
        static std::wstring getEnvVarWide( const std::string & key );
};

#endif // PATHS_H
