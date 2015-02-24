# README #

**DKDemo** is demo application for **DKLib**.
You can see 6 demo samples with this app.
All contents was written with Python.
This project is ready to build and run on Windows, Mac OS X, iOS.
This project has pre-built core library. no sources for core libraries.
You can download core library source from http://github.com/tiff2766/DKLib  

This project has IDE environment files. (Visual Studio 2013, Xcode6)  

#### Project directory structured like below.

* `DemoApp` *Demo Program source codes*
* `DKLib` *Engines*
    * `DK`  *Core Library (C++)*
        * `DKFoundation` *Templates, file, threading, data collections, foundation library*
        * `DKFramework` *Graphics, Audio, Game Physics, Application related*
    * `PyDK` *Python bindings (C++)*
    * `Bin` *Binaries*
* `DKDemo.sln` *Visual Studio 2013 Solution file*
* `DKDemo.xcworkspace` *Xcode 6 Workspace file*

# Windows Executable Download (Sample App)
* See demo quickly. (no build, no IDE required.)
* Screenshots embedded bottom of page.
* Windows
  * [Download DKDemoApp_Win32_En.zip]( https://bitbucket.org/tiff2766/dkdemo/downloads/DKDemoApp_Win32_En.zip)
  * Build: 2015-01-22, win32_i386  
* Mac OS X
  * [Download Demo_OSX.zip](https://bitbucket.org/tiff2766/dkdemo/downloads/Demo_OSX.app.zip)
  * Build: 2015-02-24

# About DKLib
a small game engine, written with C\++. This project includes pre-built DK libraries (DK, PyDK). all contents were written with Python. You can use all codes of this project for your project, whether it is commercial or not.

more detail, full-source on http://github.com/tiff2766/DKLib

## Available platforms 
* Windows Vista (or later, 32-64bit)
* Mac OS X 10.8 or later
* iOS 4.0 or later

   > In this demo, distributes with **Framework** library. So If you want to build and run with this project, a target OS should be iOS 8.0 or later. Previous iOS does not supports Framework.
   > If you want to build for iOS7 or earlier version of iOS, download DKLib and build static-library. (download link is described above.)
   >
   > i386, x64, armv7, arm64 universal binary included.

**Android and Linux are not supported at this time.**

## Build Environments
* Windows
    * Vista or later
    * Visual Studio 2013
* Mac
    * Mac OS X 10.8 or later
    * Xcode 6
    > Framework includes symbolic-link. Do not use zipped file created from windows.

## Hardware requirement
* OpenGL 3.1 or later
* OpenGL ES 2.0 or later (mobile)

## Author
    Copyright (C) 2015, Hongtae Kim. tiff2766@gmail.com


## Screenshots (*Taken from Korean-Version*)
### Character Animations
![anim.png](https://bitbucket.org/repo/keLaoe/images/2465982228-anim.png)

* Animation control sample
* 3 characters, 3 animations

### Vehicle physics
![car1.png](https://bitbucket.org/repo/keLaoe/images/1454161511-car1.png)
![car2.png](https://bitbucket.org/repo/keLaoe/images/736436422-car2.png)

* Keyboard arrow: move vehicle, Mouse left-click: move camera, right-click: shoot obstacle
* first touch moves camera, second touch shoots obstacle on iOS

### Kinematic Animation
![kine1.png](https://bitbucket.org/repo/keLaoe/images/1827184730-kine1.png)
![kine2.png](https://bitbucket.org/repo/keLaoe/images/540183168-kine2.png)

* Left click: move camera, Right click: shoot box
* Seond finger touch to shoot box on iOS

### Shadow map sample
![sm1.png](https://bitbucket.org/repo/keLaoe/images/4172863041-sm1.png)
![sm2.png](https://bitbucket.org/repo/keLaoe/images/952550011-sm2.png)

* LiSPSM algorithm written with Python.
* Move light direction with bottom-left sliders.

### 2D Sprites, Texture Atlas
![sp.png](https://bitbucket.org/repo/keLaoe/images/605784757-sp.png)

* Script demo to make 2D games.
* Texture Atlas generated from [TexturePacker.](http://www.codeandweb.com/texturepacker)

### UI Widgets
![ui.png](https://bitbucket.org/repo/keLaoe/images/3182995816-ui.png)

* Demo for UI Controls
* Can use UI for game or tools.
