#ifndef COLORIZEICON_H
#define COLORIZEICON_H

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#include <iostream>
#include <string>
#include <filesystem>
#include <list>
#include <vector>

#include <windows.h>
#include <shlobj.h>
#include <codecvt>
#include <locale>

#include <stdlib.h>


#include <fstream>


#include "Paths.h"

using namespace std;

#include <Magick++.h>
using namespace Magick;


namespace fs = std::filesystem;

const double range = 255;

struct IMAGE_BUFFERS {
    unsigned char* rgb_buf;
    unsigned char* alpha_buf;
    int buf_size;
};


class ColorizeIcon
{
private:
    list<Magick::Image> imageListGray;
    vector<IMAGE_BUFFERS> imageBuffers;

public:
    ColorizeIcon();
    int ColorizeToBuffers(int red, int green, int blue);
    int ColorizeAndWrite(const std::wstring &folder_to_colorize, int red, int green, int blue);

    int getNumIcons();
    vector<IMAGE_BUFFERS> getIconBuffers();
};

#endif // COLORIZEICON_H
