/***************************************************************
 * Name:      FolcoApp.cpp
 * Purpose:   Code for Application Class
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

#include "FolcoApp.h"
#include "FolcoMain.h"

IMPLEMENT_APP(FolcoApp);

bool FolcoApp::OnInit()
{
    fs::path folco_dir = fs::path(Paths::getFolcoDir());
    fs::path cache_dir = fs::path(Paths::getCacheDir());
    fs::path asset_dir = fs::path(Paths::getAssetDir());
    fs::path icon_cache_dir = fs::path(Paths::getIconCacheDir());
    fs::path folderstxt_dir = fs::path(Paths::getFoldersTxt());


    wxCmdLineParser parser (cmdLineDesc, argc, argv);
    parser.Parse();

    if (!fs::exists(folco_dir)) {
        fs::create_directory(folco_dir);
    }

    if (!fs::exists(folderstxt_dir)) {
            wofstream folders_txt;
            folders_txt.open(Paths::getFoldersTxt(), std::ios::out);
            folders_txt.close();
    }

    if (!fs::exists(cache_dir)) {
        fs::create_directory(cache_dir);
        if (!fs::exists(asset_dir)) {
            fs::create_directory(asset_dir);
            const std::wstring icon_resource_location = Paths::getResourceFileDirWide();
            const std::wstring asset_dir_wide = Paths::getAssetDirWide();
            BuildCache::saveIconGroupImages(icon_resource_location, asset_dir_wide, FOLDER_ICON_ID);
        }
        if (!fs::exists(icon_cache_dir)) {
            fs::create_directory(icon_cache_dir);
        }
    }


    if (!wxApp::OnInit()) {
        return false;
    }

    wxInitAllImageHandlers();

    InitializeMagick(NULL);


    if (reset) {
        if (fs::exists(folco_dir)) {
            if (fs::exists(folderstxt_dir)) {
                std::wifstream file(Paths::getFoldersTxt());
                std::wstring str;
                while (std::getline(file, str)) {
                    if (fs::exists(fs::path(str))) {
                        SHFOLDERCUSTOMSETTINGS fcs;

                        fcs.dwSize = sizeof(fcs);
                        fcs.dwMask = FCSM_ICONFILE;
                        fcs.pszIconFile = NULL;
                        fcs.cchIconFile = 0;
                        fcs.iIconIndex = 0;

                        SHGetSetFolderCustomSettings(&fcs, str.c_str(), FCS_FORCEWRITE);

                    }

                }
                file.close();

                fs::remove(Paths::getFoldersTxt());
                this->Exit();
            }
        }
    }


    FolcoFrame* frame = new FolcoFrame(0L, _("Folco"));

    frame->setFolderArgs(folders);
    //frame->SetClientSize(frame->FromDIP(wxSize(400, 300)));
    frame->Center(wxBOTH);
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();

    return true;
}


void FolcoApp::OnInitCmdLine(wxCmdLineParser& parser)
{
    parser.SetDesc (cmdLineDesc);
    // must refuse '/' as parameter starter or cannot use "/path" style paths
    parser.SetSwitchChars ("-");
}

bool FolcoApp::OnCmdLineParsed(wxCmdLineParser& parser)
{

    reset = parser.Found("r");


    // to get at your unnamed parameters use

    for (int i = 0; i < parser.GetParamCount(); i++)
    {
            folders.Add(parser.GetParam(i));
            //wxMessageBox(parser.GetParam(i));

    }

    // and other command line parameters

    // then do what you need with them.

    return true;
}

int FolcoApp::OnExit()
{
  return 0;
}

