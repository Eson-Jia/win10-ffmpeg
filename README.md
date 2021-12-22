# 在 windows 上编译 libx264 和 ffmpeg 并在 CLion 工程中引用

参考[Windows 下 FFmpeg 和 LibX264 的编译和配置](https://glumes.com/post/ffmpeg/windows-compile-ffmpeg-with-libx264/)

首先我们需要清楚下面几个概念

## 名词

### MSVC

Microsoft Visual C++（简称Visual C++、MSVC、VC++或VC）是微软公司的C++开发工具，具有集成开发环境，可提供编辑C语言，C++以及C++/CLI等编程语言。 
VC++集成了便利的调试工具，特别是集成了微软Windows视窗操作系统应用程序接口（Windows API）、三维动画DirectX API，Microsoft .NET框架。
参考[wiki](https://zh.wikipedia.org/wiki/Microsoft_Visual_C%2B%2B)

### mingW

MinGW（Minimalist GNU for Windows），又称mingw32，是将GCC编译器和GNU Binutils移植到Win32平台下的产物，包括一系列头文件（Win32API）、库和可执行文件。 MinGW是从Cygwin（1.3.3版）基础上发展而来。
可以理解成是一个编译器和一些列头文件、库和执行文件。
参考[MinGW](https://zh.wikipedia.org/wiki/MinGW)

### MSYS2
MSYS2 不是“一个可以统治所有人的工具”，而是试图专注于它擅长的领域。它提供了一个基于开源软件的本机构建环境，让您在已经熟悉 Linux 的情况下感到宾至如归。在 Windows 上为不同的任务使用多种不同的环境和工具是有充分理由的。
与 Cygwin 的区别：Cygwin 专注于在 Windows 上构建 Unix 软件，而 MSYS2 专注于构建针对 Windows API 构建的本机软件。
参考[What is MSYS2?](https://www.msys2.org/docs/what-is-msys2/)

### Cygwin

Cygwin是许多自由软件的集合，最初由Cygnus Solutions开发，用于各种版本的Microsoft Windows上，运行类UNIX系统。Cygwin的主要目的是通过重新编译，
将POSIX系统（例如Linux、BSD，以及其他Unix系统）上的软件移植到Windows上。

综上所述，`MSVC`和`mingW`是两种不同的编译器和库、头文件集合。MSYS2 和 Cygwin 是两种不同的类 Unix 环境软件。环境软件可以包含多编译器(MSVC,mingW)。

## 编译过程

注意：文档中使用 MSYS2 搭建的编译环境，使用了 MSVC 编译套件进行编译。需要注意的是 MSYS2 在安装相关软件的过程中因为**莫名原因**速度非常慢，但是我们可以设置 proxy 来加速。我们打开 msys2 的安装目录可以找到一个`network.xml`，
在这里可以进行相关的配置。

## CLion 工程配置

因为文中是使用 msys2 编译的

## CMakeList

find_library

