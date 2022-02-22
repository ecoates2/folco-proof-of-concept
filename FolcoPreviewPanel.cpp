#include "FolcoPreviewPanel.h"

FolcoPreviewPanel::FolcoPreviewPanel(wxFrame* parent, unsigned char* preview_rgb, unsigned char* preview_alpha) : wxPanel(parent)
{
    preview = wxImage(256, 256, preview_rgb, preview_alpha, true);
    preview_scaled = preview.Scale(50, 50, wxIMAGE_QUALITY_HIGH);
    this->Bind(wxEVT_PAINT, &FolcoPreviewPanel::OnPaint, this);


}

FolcoPreviewPanel::~FolcoPreviewPanel()
{

}

void FolcoPreviewPanel::updatePreviewState(unsigned char* preview_rgb, unsigned char* preview_alpha)
{
    preview.SetData(preview_rgb, true);
    preview.SetAlpha(preview_alpha, true);

    preview_scaled = preview.Scale(50, 50, wxIMAGE_QUALITY_HIGH);
}

void FolcoPreviewPanel::OnPaint(wxPaintEvent & event)
{
    wxPaintDC dc(this);
    dc.Clear();

    wxBitmap preview_bitmap(preview_scaled);
    dc.DrawBitmap(preview_bitmap, 0, 0, true);

}
