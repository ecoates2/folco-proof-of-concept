#include "ColorizeIcon.h"


ColorizeIcon::ColorizeIcon()
{
  for (const auto & entry : fs::directory_iterator(Paths::getAssetDir())) {
    imageListGray.push_back(Magick::Image(entry.path().string()));
  }

  for (auto & image : imageListGray) {
    image.type(GrayscaleAlphaType);
    int curr_size = image.columns();

    if (curr_size <= 64) {
        IMAGE_BUFFERS curr_bufs;

        curr_bufs.rgb_buf = (unsigned char*) malloc(image.columns() * image.columns() * 3);
        curr_bufs.alpha_buf = (unsigned char*) malloc(image.columns() * image.columns());
        curr_bufs.buf_size = curr_size;

        //curr_bufs.rgb_buf = (unsigned char*) malloc(64 * 64 * 3);
        //curr_bufs.alpha_buf = (unsigned char*) malloc(64 * 64);
        //curr_bufs.buf_size = 64;

        imageBuffers.push_back(curr_bufs);

    }



  }

}


int ColorizeIcon::ColorizeAndWrite(const std::wstring &folder_to_colorize, int red, int green, int blue)
{
   std::string asset_path = Paths::getAssetDir();

   std::string icon_cache_path = Paths::getIconCacheDir() + "\\" + std::to_string(red) + "-" + std::to_string(green) + "-" + std::to_string(blue) + ".ico";

  ColorRGB color = ColorRGB((red / range), (green / range), (blue / range));

  list<Magick::Image> imageList;

  for (auto & image : imageListGray) {
    imageList.push_back(Magick::Image(image));
  }


  for (auto & image : imageList) {

    image.fillColor(color);
    image.tint("100"); // Doesn't work on dark colors, reference https://legacy.imagemagick.org/Usage/color_mods/#tint_overlay for usage of overlay functions.

  }

  Magick::writeImages(imageList.begin(), imageList.end(), icon_cache_path);

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

  std::wstring icon_cache_path_wide = (converter.from_bytes(icon_cache_path));


  SHFOLDERCUSTOMSETTINGS fcs;

  fcs.dwSize = sizeof(fcs);
  fcs.dwMask = FCSM_ICONFILE;
  fcs.pszIconFile = icon_cache_path_wide.c_str();
  fcs.cchIconFile = 0;
  fcs.iIconIndex = 0;

  SHGetSetFolderCustomSettings(&fcs, folder_to_colorize.c_str(), FCS_FORCEWRITE);

  return 0;
}

int ColorizeIcon::ColorizeToBuffers(int red, int green, int blue)
{

  ColorRGB color = ColorRGB((red / range), (green / range), (blue / range));

  list<Magick::Image> imageList;

  for (auto & image : imageListGray) {
    int curr_size = image.columns();
    if (curr_size <= 64) {
        Magick::Image toAdd = Magick::Image(image);
        //toAdd.resize(Magick::Geometry(64,64));
        imageList.push_back(toAdd);
    }
  }

  int vec_idx = 0;

  for (auto & image : imageList) {
    image.fillColor(color);
    image.tint("100"); // Doesn't work on dark colors, reference https://legacy.imagemagick.org/Usage/color_mods/#tint_overlay for usage of overlay functions.
    int curr_size = image.columns();
    image.write(0,0, curr_size, curr_size, "RGB", Magick::CharPixel, (imageBuffers[vec_idx]).rgb_buf);
    image.write(0,0, curr_size, curr_size, "A", Magick::CharPixel, (imageBuffers[vec_idx]).alpha_buf);

    vec_idx++;
  }

  return 0;
}

int ColorizeIcon::getNumIcons()
{
    return imageListGray.size();
}

vector<IMAGE_BUFFERS> ColorizeIcon::getIconBuffers()
{
  return imageBuffers;
}
