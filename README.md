# Folco
A folder colorizing utility for Windows written in C++.

# Features
* The very first open source folder colorizer!
* Set custom colors for either single folders or for an entire group
* A preview window showing the colored version of every icon size, barring 256x256 (the "jumbo" size on Windows Vista and above)

# Goals
* Make the application fully deployable using a bundled installer
* Register context menu functionality in the registry, i.e. the ability to run the application directly from right clicking a folder
* Provide proper build tools
* Fix DPI scaling; the application currently scales exactly to the size of the preview window
* Provide better support for darker colors
  * Currently, the application relies on [ImageMagick's tint() function](https://legacy.imagemagick.org/Usage/color_mods/), which targets mostly midtone colors. However, the side effect of this is that using darker colors will yield results that are much lighter colored than expected.
* Implement more advanced coloring options, such as textures, patterns, overlays and gradients
* Add overlay icons (things like checkmarks)
* Clean up the codebase and optimize performance!

# Usage
For now, the application can be run using the command line.

1. Navigate to the `bin` directory of your installation in Windows CMD
2. Run `folco "path_to_folder_1" "path_to_folder_2" ... "path_to_folder_n"`
3. Select the desired color and hit apply. 

To reset any given folder's color back to default, right click on it and select `Properties -> Customize -> Change Icon... -> Reset to Defaults`. 
To batch reset all colored folders, run the application with the -r command line switch: `folco -r`
This will automatically reset all folders (that weren't renamed or moved) that you previously colorized. (Upon writing this, I've realized this restriction can be solved using the folders' PIDL values. This will be done in the proper release)

# About
I created this application to strengthen my skills in OOP and working with compiled languages, as well as to fill a quite surprising gap; most, if not all of the software previously written for this purpose has been either freeware or payware. I am eternally grateful for the open source community for a great deal of the software I use today, so I hope this serves as a worthy contribution! <br /><br />
This application uses WxWidgets, ImageMagick, and leverages the Win32 API.


