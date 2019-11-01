# NOTE(yuval): Compiler And Linker Flags
compiler_flags="-g -Wall -Wextra -Werror -fdiagnostics-absolute-paths -std=c++11 -fno-rtti -fno-exceptions"
compiler_flags+=" -Wno-unused-parameter -Wno-unused-variable -Wno-deprecated-declarations -Wno-sometimes-uninitialized -Wno-unused-function -Wno-char-subscripts"
compiler_flags+=" -DSLIDE_DEVELOPER=1 -DSLIDE_DEBUG=1 -DSLIDE_RELEASE=0"

# TODO(yuval): Only darwin is supported for now (support linux)
linker_flags="-Wl,-rpath,@loader_path"

# NOTE(yuval): Mac Specific Compiler &  Linker Flags
mac_compiler_flags="-DSLIDE_MAC=1 -DSLIDE_WIN32=0"
mac_linker_flags="-framework Cocoa -framework IOKit -framework Security -framework AudioToolBox"

# NOTE(yuval): Compiler Setup
if [ -n "$(command -v clang++)" ]
then
  CXX=clang++
  compiler_flags+=" -Wno-missing-braces -Wno-null-dereference -Wno-self-assign"
else
  CXX=c++
  compiler_flags+=" -Wno-unused-but-set-variable"
fi

# NOTE(yuval): Build Directory Setup
mkdir -p "../.build"
pushd "../.build" > /dev/null

echo "Compiling Using: $CXX"

# NOTE(yuval): Opengl Renderer Compilation
$CXX $compiler_flags $mac_compiler_flags ../src/mac_slide_opengl.mm -fPIC -shared -o ../run_tree/mac_slide_opengl.dylib -ldl $linker_flags $mac_linker_flags -framework OpenGL

# NOTE(yuval): Mac Platform Compilation
$CXX $compiler_flags $mac_compiler_flags -I../src/third_party/freetype2 ../src/mac_slide.mm -o ../run_tree/mac_slide -ldl $linker_flags $mac_linker_flags -L../third_party/freetype-2.10.1/bin/lib -lfreetype

# NOTE(yuval): Compilation Teardown
popd > /dev/null