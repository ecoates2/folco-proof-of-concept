#ifndef FOLCOPREVIEWPANEL_H
#define FOLCOPREVIEWPANEL_H

#include "wx/wx.h"
#include "wx/sizer.h"

#include "ColorizeIcon.h"


class FolcoPreviewPanel : public wxPanel
{
    public:
        FolcoPreviewPanel(wxFrame* parent, unsigned char* preview_rgb_in, unsigned char* preview_alpha_in);
        virtual ~FolcoPreviewPanel();

        void OnPaint(wxPaintEvent & event);

        void updatePreviewState(unsigned char* preview_rgb, unsigned char* preview_alpha);


    protected:

    private:

        wxImage preview;
        wxImage preview_scaled;
};

#endif // FOLCOPREVIEWPANEL_H
