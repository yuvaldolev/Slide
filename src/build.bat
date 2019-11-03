@echo off

REM Common Compiler & Linker Flags
set compiler_flags=-nologo -MTd -fp:fast -Gm- -GR- -EHa -Od -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4101 -Z7 -FC
set compiler_flags=-DSLIDE_DEVELOPER=1 -DSLIDE_DEBUG=1 -DSLIDE_RELEASE=0 %compiler_flags%
set linker_flags=-opt:ref -incremental:no

REM Win32 Specific Compiler & Linker Flags
set win32_compiler_flags=-DSLIDE_WIN32=1 -DSLIDE_MAC=0
set win32_linker_flags=user32.lib gdi32.lib kernel32.lib

REM Compilation Setup
IF NOT EXIST ..\.build mkdir ..\.build & attrib +h ..\.build /s /d
pushd ..\.build

REM Asset Builder Compilation
REM cl %compiler_flags% ..\src\slide_asset_builder.cpp -Fe..\run_tree\slide_asset_builder

REM Opengl Renderer Compilation
cl  %compiler_flags% %win32_compiler_flags% ..\src\win32_slide_opengl.cpp -Fe..\run_tree\win32_slide_opengl -LD /link -EXPORT:win32_load_renderer %linker_flags% %win32_linker_flags% opengl32.lib

REM Win32 Platform Compilation
cl %compiler_flags% %win32_compiler_flags% -I..\src\third_party\freetype2 ..\src\win32_slide.cpp -Fe..\run_tree\win32_slide /link %linker_flags% %win32_linker_flags% winmm.lib ..\third_party\freetype-2.10.1\bin\lib\freetype.lib

REM Compilation Teardown
popd