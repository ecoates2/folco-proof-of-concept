/***************************************************************
 * Name:      FolcoMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2022-01-23
 * Copyright:  ()
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "FolcoMain.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

BEGIN_EVENT_TABLE(FolcoFrame, wxFrame)
    EVT_CLOSE(FolcoFrame::OnClose)
    EVT_MENU(idMenuQuit, FolcoFrame::OnQuit)
    EVT_MENU(idMenuAbout, FolcoFrame::OnAbout)
    EVT_BUTTON(idClickStart, FolcoFrame::OnStartButtonClick)
    EVT_COLOURPICKER_CHANGED(idColorPicked, FolcoFrame::OnColorPicked)
END_EVENT_TABLE()

FolcoFrame::FolcoFrame(wxFrame *frame, const wxString& title)
    : wxFrame(frame, -1, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE & ~ (wxRESIZE_BORDER | wxMAXIMIZE_BOX))
{

    std::wifstream file(Paths::getFoldersTxt());
    std::wstring str;
    while (std::getline(file, str))
    {
        colored_folders.insert(str);
    }
    file.close();


    colorizer = ColorizeIcon();

    imageBuffers = colorizer.getIconBuffers();

    colorizer.ColorizeToBuffers((int)color.Red(), (int)color.Green(), (int)color.Blue());


    for (int i = 0; i < imageBuffers.size(); i++) {
        wxImages.push_back(wxImage((imageBuffers[i]).buf_size, (imageBuffers[i]).buf_size, (imageBuffers[i]).rgb_buf, (imageBuffers[i]).alpha_buf, true));
    }


    panel = new wxPanel(this, wxID_ANY);

    preview_panel = new wxPanel( panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxBORDER_SIMPLE );
    preview_panel->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNHIGHLIGHT ) );

    wxBoxSizer *vSizer = new wxBoxSizer( wxVERTICAL );

    wxBoxSizer *hSizer = new wxBoxSizer( wxHORIZONTAL );

    vSizer->Add(preview_panel, 0, wxALL, 5);

    vSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    for (int i = 0; i < wxImages.size(); i++) {
        wxStaticBitmap* to_add = new wxStaticBitmap(preview_panel, wxID_ANY, wxBitmap(wxImages[i]));
        to_add->SetMinSize( FromDIP(wxSize(-1,imageBuffers[0].buf_size)) );
        wxStaticBitmaps.push_back(to_add);
        hSizer->Add( to_add, 0, wxALL, 5 );
    }





    //hSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    //vSizer->Add( hSizer, 1, wxEXPAND, 5 );

    clrPicker = new wxColourPickerCtrl(panel, idColorPicked, *wxBLUE);
    vSizer->Add( clrPicker, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


    button1 = new wxButton(panel, idClickStart, "Set color");
    vSizer->Add( button1, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );



    vSizer->Add( 0, 0, 1, wxEXPAND, 5 );

    panel->SetSizer( vSizer );
    preview_panel->SetSizer(hSizer);

    this->Layout();

    this->SetSize( FromDIP(wxSize(300, (preview_panel->GetSize().GetWidth())) ));
    //wxMessageBox(std::to_string(preview_panel->GetSize().GetHeight()));



    //panel->Bind(wxEVT_PAINT, &FolcoFrame::OnPaint, this);


    // create a menu bar
    wxMenuBar* mbar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu(_T(""));
    fileMenu->Append(idMenuQuit, _("&Quit\tAlt-F4"), _("Quit the application"));
    mbar->Append(fileMenu, _("&File"));

    wxMenu* helpMenu = new wxMenu(_T(""));
    helpMenu->Append(idMenuAbout, _("&About\tF1"), _("Show info about this application"));
    mbar->Append(helpMenu, _("&Help"));

    SetMenuBar(mbar);
/*
    // create a status bar with some information about the used wxWidgets version
    CreateStatusBar(2);
    SetStatusText(_("Hello Code::Blocks user!"),0);
    SetStatusText(wxbuildinfo(short_f), 1);
*/


}


FolcoFrame::~FolcoFrame()
{

}


void FolcoFrame::OnStartButtonClick(wxCommandEvent& event)
{

    fstream folders_txt;
    folders_txt.open(Paths::getFoldersTxt(), std::ios::app);

    bool any_new_folders = false;

    for (int i = 0; i < folder_args.GetCount(); i++) {
        const std::wstring curr_path = (folder_args[i]).ToStdWstring();
        colorizer.ColorizeAndWrite(curr_path, (int)color.Red(), (int)color.Green(), (int)color.Blue());


        if (colored_folders.find(curr_path) == colored_folders.end()) {
            folders_txt << curr_path << endl;
            colored_folders.insert(std::wstring(curr_path));
            any_new_folders = true;
        }
    }

    if (any_new_folders) {
        SHChangeNotify(0x8000000, 0x1000, 0, 0); // Refresh explorer
    }




    folders_txt.close();
}

void FolcoFrame::OnColorPicked(wxColourPickerEvent& event)
{
    color = event.GetColour();

    colorizer.ColorizeToBuffers((int)color.Red(), (int)color.Green(), (int)color.Blue());

    for (int i = 0; i < imageBuffers.size(); i++) {
        wxImages[i].SetData((imageBuffers[i]).rgb_buf, true);
        wxImages[i].SetAlpha((imageBuffers[i]).alpha_buf, true);
        wxStaticBitmaps[i]->SetBitmap(wxBitmap(wxImages[i]));
    }

    Refresh();
}

void FolcoFrame::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void FolcoFrame::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void FolcoFrame::OnAbout(wxCommandEvent &event)
{
    wxAboutDialogInfo info;
    info.SetName(_("Folco"));
    info.SetVersion(FOLCOVERSION);
    info.SetDescription(_("A folder colorizing utility for Windows written in C++"));
    info.SetWebSite("github link", "Folco webpage");
    info.AddDeveloper("Ethan Coates");

    wxAboutBox(info);
}

void FolcoFrame::setFolderArgs(wxArrayString input)
{
    folder_args = input;
}
/*
void FolcoFrame::OnPaint(wxPaintEvent & event)
{
    wxPaintDC dc(panel);
    dc.Clear();

    wxBitmap preview_bitmap(preview_scaled);
    dc.DrawBitmap(preview_bitmap, 50, 50, true);

}
*/
