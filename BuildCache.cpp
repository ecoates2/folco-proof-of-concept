#include "BuildCache.h"



IconDirectory BuildCache::GetIconDirectory( HMODULE hMod, WORD Id ) {
    HRSRC hRsrc = FindResource( hMod, MAKEINTRESOURCE( Id ), RT_GROUP_ICON );
    HGLOBAL hGlobal = LoadResource( hMod, hRsrc );
    GRPICONDIR* lpGrpIconDir = (GRPICONDIR*)LockResource( hGlobal );

    IconDirectory dir;
    for ( size_t i = 0; i < lpGrpIconDir->idCount; ++i ) {
        dir.push_back( lpGrpIconDir->idEntries[ i ] );
    }
    return dir;
}

HICON BuildCache::LoadSpecificIcon( HMODULE hMod, WORD Id ) {
    HRSRC hRsrc = FindResource( hMod, MAKEINTRESOURCE( Id ), RT_ICON );
    HGLOBAL hGlobal = LoadResource( hMod, hRsrc );
    BYTE* lpData = (BYTE*)LockResource( hGlobal );
    DWORD dwSize = SizeofResource( hMod, hRsrc );

    HICON hIcon = CreateIconFromResourceEx( lpData, dwSize, TRUE, 0x00030000,
                                            0, 0, LR_DEFAULTCOLOR );
    return hIcon;
}



int BuildCache::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
   UINT  num = 0;          // number of image encoders
   UINT  size = 0;         // size of the image encoder array in bytes

   ImageCodecInfo* pImageCodecInfo = NULL;

   GetImageEncodersSize(&num, &size);
   if(size == 0)
      return -1;  // Failure

   pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
   if(pImageCodecInfo == NULL)
      return -1;  // Failure

   GetImageEncoders(num, size, pImageCodecInfo);

   for(UINT j = 0; j < num; ++j)
   {
      if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
      {
         *pClsid = pImageCodecInfo[j].Clsid;
         free(pImageCodecInfo);
         return j;  // Success
      }
   }

   free(pImageCodecInfo);
   return -1;  // Failure
}

bool BuildCache::hasEnding (const std::wstring& fullString, const std::wstring& ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare (fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}


BITMAP_AND_BYTES BuildCache::createAlphaChannelBitmapFromIcon(HICON hIcon) {

    // Get the icon info
    ICONINFO iconInfo = {0};
    GetIconInfo(hIcon, &iconInfo);

    // Get the screen DC
    HDC dc = GetDC(NULL);

    // Get icon size info
    BITMAP bm = {0};
    GetObject( iconInfo.hbmColor, sizeof( BITMAP ), &bm );

    // Set up BITMAPINFO
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = bm.bmWidth;
    bmi.bmiHeader.biHeight = -bm.bmHeight;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    // Extract the color bitmap
    int nBits = bm.bmWidth * bm.bmHeight;
    int32_t* colorBits = new int32_t[nBits];
    GetDIBits(dc, iconInfo.hbmColor, 0, bm.bmHeight, colorBits, &bmi, DIB_RGB_COLORS);

    // Check whether the color bitmap has an alpha channel.
    BOOL hasAlpha = FALSE;
    for (int i = 0; i < nBits; i++) {
        if ((colorBits[i] & 0xff000000) != 0) {
            hasAlpha = TRUE;
            break;
        }
    }

    // If no alpha values available, apply the mask bitmap
    if (!hasAlpha) {
        // Extract the mask bitmap
        int32_t* maskBits = new int32_t[nBits];
        GetDIBits(dc, iconInfo.hbmMask, 0, bm.bmHeight, maskBits, &bmi, DIB_RGB_COLORS);
        // Copy the mask alphas into the color bits
        for (int i = 0; i < nBits; i++) {
            if (maskBits[i] == 0) {
                colorBits[i] |= 0xff000000;
            }
        }
        delete[] maskBits;
    }

    // Release DC and GDI bitmaps
    ReleaseDC(NULL, dc);
    ::DeleteObject(iconInfo.hbmColor);
    ::DeleteObject(iconInfo.hbmMask);

    // Create GDI+ Bitmap
    Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(bm.bmWidth, bm.bmHeight, bm.bmWidth*4, PixelFormat32bppARGB, (BYTE*)colorBits);
    BITMAP_AND_BYTES ret = {bmp, colorBits};

    return ret;
}

int BuildCache::saveIconGroupImages(const std::wstring& fname, const std::wstring& output_dir, WORD Id) {


    CoInitialize(NULL);

    Gdiplus::GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    CLSID g_pngClsid;

    GetEncoderClsid(L"image/png", &g_pngClsid);


    HMODULE hMod = LoadLibraryExW( fname.c_str(),
                                   NULL, LOAD_LIBRARY_AS_IMAGE_RESOURCE);

    IconDirectory dir = GetIconDirectory( hMod, Id );

    int icons_processed = 0;

    wstring output_dir_sanitized = output_dir;

    if (!hasEnding(output_dir_sanitized, L"\\")) {
        output_dir_sanitized.append(L"\\");
    }

    for ( IconDirectoryCIt it = dir.begin(); it != dir.end(); ++it ) {

        std::wstring write_this_ico_to = output_dir_sanitized;
        write_this_ico_to.append(std::to_wstring(icons_processed));
        write_this_ico_to.append(L".png");


        HICON hIcon = LoadSpecificIcon( hMod, it->nID );
        BITMAP_AND_BYTES bbs = createAlphaChannelBitmapFromIcon(hIcon);

        IStream* fstrm = NULL;
        SHCreateStreamOnFile(write_this_ico_to.c_str(), STGM_WRITE|STGM_CREATE, &fstrm);
        bbs.bmp->Save(fstrm, &g_pngClsid, NULL);
        fstrm->Release();

        delete bbs.bmp;
        delete[] bbs.bytes;
        DestroyIcon(hIcon);

        icons_processed++;

    }

    Gdiplus::GdiplusShutdown(gdiplusToken);

    CoUninitialize();

    return 0;

}


