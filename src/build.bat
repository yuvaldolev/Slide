@echo off

REM Common Compiler & Linker Flags
set compiler_flags=-nologo -MTd -fp:fast -Gm- -GR- -EHa -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4101 -Z7 -FC
set compiler_flags=-DSLIDE_DEVELOPER=1 -DSLIDE_DEBUG=1 -DSLIDE_RELEASE=0 %compiler_flags%
set linker_flags=-opt:ref -incremental:no

REM Win32 Specific Compiler & Linker Flags
set win32_compiler_flags=-DSLIDE_WIN32=1
set win32_linker_flags=user32.lib gdi32.lib winmm.lib opengl32.lib

REM Compilation Setup
IF NOT EXIST ..\.build mkdir ..\.build & attrib +h ..\.build /s /d
pushd ..\.build

REM Opengl Renderer Compilation
cl  %compiler_flags% %win32_compiler_flags% ..\src\win32_slide_opengl.cpp -Fe..\run_tree\win32_slide_opengl -LD /link -incremental:no -opt:ref -EXPORT:win32_load_renderer gdi32.lib opengl32.lib user32.lib

REM Win32 Platform Compilation
cl %compiler_flags% %win32_compiler_flags% ..\src\win32_slide.cpp -Fe..\run_tree\win32_slide /link %linker_flags% %win32_linker_flags%

REM Compilation Teardown
popd