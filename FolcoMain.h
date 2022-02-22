/***************************************************************
 * Name:      FolcoMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2022-01-23
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef FOLCOMAIN_H
#define FOLCOMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <stdlib.h>
#include <wx/clrpicker.h>
#include <wx/colour.h>
#include <wx/aboutdlg.h>
#include <wx/statbmp.h>
#include <wx/notebook.h>
#include <vector>
#include <set>
#include <algorithm>

#include <windows.h>
#include <shlobj.h>

#include "ColorizeIcon.h"

const std::string FOLCOVERSION = "0.1";

class FolcoFrame: public wxFrame
{
    public:
        FolcoFrame(wxFrame *frame, const wxString& title);
        ~FolcoFrame();

    public:
        wxPanel* panel = nullptr;

        wxPanel* preview_panel = nullptr;



        wxButton *button1 = nullptr;
        wxColourPickerCtrl *clrPicker = nullptr;

        vector<IMAGE_BUFFERS> imageBuffers;
        vector<wxImage> wxImages;
        vector<wxStaticBitmap*> wxStaticBitmaps;

        wxColour color = (*wxBLUE);
        ColorizeIcon colorizer;

        wxImage preview_small;
        wxImage preview_medium;
        wxImage preview_large;

        wxStaticBitmap* preview_small_bmp;
        wxStaticBitmap* preview_medium_bmp;
        wxStaticBitmap* preview_large_bmp;

        wxArrayString folder_args;
        std::set<std::wstring> colored_folders;
        void setFolderArgs(wxArrayString input);

        int smallIdx;
        int mediumIdx;
        int largeIdx;
    private:
        enum
        {
            idMenuQuit = 1000,
            idMenuAbout = 1001,
            idClickStart = 1002,
            idColorPicked = 1003,
            idOnPaint = 1004
        };
        void OnClose(wxCloseEvent& event);
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnStartButtonClick(wxCommandEvent& event);
        void OnColorPicked(wxColourPickerEvent& event);
        //void OnPaint(wxPaintEvent & event);
        DECLARE_EVENT_TABLE()
};


#endif // FOLCOMAIN_H
