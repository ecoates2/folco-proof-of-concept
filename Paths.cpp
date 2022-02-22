#include "Paths.h"

std::string Paths::getAssetDir()
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return converter.to_bytes(getEnvVarWide("APPDATA") + L"\\" + ASSETDIR);
}

std::string Paths::getIconCacheDir()
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return converter.to_bytes(getEnvVarWide("APPDATA") + L"\\" + ICON_CACHE_DIR);
}

std::string Paths::getFolcoDir()
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return converter.to_bytes(getEnvVarWide("APPDATA") + L"\\" + FOLCODIR);
}

std::string Paths::getCacheDir()
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return converter.to_bytes(getEnvVarWide("APPDATA") + L"\\" + CACHEDIR);
}

std::wstring Paths::getAssetDirWide()
{
    return getEnvVarWide("APPDATA") + L"\\" + ASSETDIR;
}

std::wstring Paths::getIconCacheDirWide()
{
    return getEnvVarWide("APPDATA") + L"\\" + ICON_CACHE_DIR;
}

std::wstring Paths::getFolcoDirWide()
{
    return getEnvVarWide("APPDATA") + L"\\" + FOLCODIR;
}

std::wstring Paths::getCacheDirWide()
{
    return getEnvVarWide("APPDATA") + L"\\" + CACHEDIR;
}

std::wstring Paths::getResourceFileDirWide()
{
    return getEnvVarWide("WINDIR") + L"\\SYSTEM32" + L"\\" + ICON_PE_FILE;
}

std::string Paths::getFoldersTxt()
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    return converter.to_bytes(getEnvVarWide("APPDATA") + L"\\" + FOLDERS);
}

std::wstring Paths::getEnvVarWide( const std::string & key )
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    char * val = getenv( key.c_str() );
    val == NULL ? std::string("") : std::string(val);

    return converter.from_bytes(val);
}
