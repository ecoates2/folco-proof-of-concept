#ifndef BUILDCACHE_H
#define BUILDCACHE_H

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <string>
#include <list>
#include <iostream>

#include <initguid.h>

#include <windows.h>
#include <shlwapi.h>
#include <commctrl.h>
#include <commoncontrols.h>

using namespace std;

#include <gdiplus.h>
using namespace Gdiplus;

// Packing of structs is necessary to match what's in PE files (EXE/DLL)

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
    BYTE   bWidth;               // Width, in pixels, of the image
    BYTE   bHeight;              // Height, in pixels, of the image
    BYTE   bColorCount;          // Number of colors in image (0 if >=8bpp)
    BYTE   bReserved;            // Reserved
    WORD   wPlanes;              // Color Planes
    WORD   wBitCount;            // Bits per pixel
    DWORD  dwBytesInRes;         // how many bytes in this resource?
    WORD   nID;                  // the ID
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;
#pragma pack( pop )

#pragma pack( push )
#pragma pack( 2 )
typedef struct
{
    WORD            idReserved;   // Reserved (must be 0)
    WORD            idType;       // Resource type (1 for icons)
    WORD            idCount;      // How many images?
    GRPICONDIRENTRY idEntries[1]; // The entries for each image
} GRPICONDIR, *LPGRPICONDIR;
#pragma pack( pop )


typedef std::list<GRPICONDIRENTRY> IconDirectory;

typedef std::list<GRPICONDIRENTRY>::const_iterator IconDirectoryCIt;

struct BITMAP_AND_BYTES {
    Gdiplus::Bitmap* bmp;
    int32_t* bytes;
};


class BuildCache
{
private:
    static IconDirectory GetIconDirectory( HMODULE hMod, WORD Id );
    static HICON LoadSpecificIcon( HMODULE hMod, WORD Id );
    static int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
    static bool hasEnding (const std::wstring& fullString, const std::wstring& ending);
    static BITMAP_AND_BYTES createAlphaChannelBitmapFromIcon(HICON hIcon);
public:
    static int saveIconGroupImages(const std::wstring& fname, const std::wstring& output_dir, WORD Id);

};




#endif // BUILDCACHE_H
