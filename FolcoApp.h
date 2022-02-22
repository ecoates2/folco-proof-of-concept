/***************************************************************
 * Name:      FolcoApp.h
 * Purpose:   Defines Application Class
 * Author:     ()
 * Created:   2022-01-23
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef FOLCOAPP_H
#define FOLCOAPP_H

#include <wx/app.h>

#include <wx/cmdline.h>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdio>
#include <windows.h>
#include <shlobj.h>

#include "BuildCache.h"
#include "ColorizeIcon.h"
#include "Paths.h"

namespace fs = std::filesystem;


static const wxCmdLineEntryDesc cmdLineDesc[] =
{
    { wxCMD_LINE_SWITCH, "h", "help", "show this help message",
        wxCMD_LINE_VAL_NONE, wxCMD_LINE_OPTION_HELP },
    { wxCMD_LINE_PARAM, "path(s)", "path(s)", "path(s)",
        wxCMD_LINE_VAL_STRING, wxCMD_LINE_NEEDS_SEPARATOR | wxCMD_LINE_PARAM_MULTIPLE | wxCMD_LINE_PARAM_OPTIONAL },
    { wxCMD_LINE_SWITCH, "r", "reset", "reset all colored icons to their default state" },

    wxCMD_LINE_DESC_END
};


class FolcoApp : public wxApp
{
    public:
        virtual bool OnInit();
        virtual int OnExit();
        virtual void OnInitCmdLine(wxCmdLineParser& parser);
        virtual bool OnCmdLineParsed(wxCmdLineParser& parser);


    private:
        wxArrayString folders;
        bool reset;
};

#endif // FOLCOAPP_H
