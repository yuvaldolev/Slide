# NOTE(yuval): Compiler And Linker Flags
compiler_flags="-g -Weverything -Wall -Werror -fdiagnostics-absolute-paths -std=c++11 -fno-rtti -fno-exceptions"
compiler_flags+=" -Wno-unsequenced -Wno-comment -Wno-unused-variable -Wno-unused-function -Wno-unused-result -Wno-switch -Wno-old-style-cast -Wno-zero-as-null-pointer-constant -Wno-string-conversion  -Wno-newline-eof -Wno-c++98-compat-pedantic -Wno-gnu-anonymous-struct -Wno-nested-anon-types -Wno-unused-parameter -Wno-padded -Wno-missing-prototypes -Wno-cast-align -Wno-sign-conversion -Wno-switch-enum -Wno-double-promotion -Wno-gnu-zero-variadic-macro-arguments -Wno-missing-noreturn -Wno-class-varargs -Wno-deprecated-declarations -Wno-documentation-unknown-command -Wno-weak-vtables -Wno-cast-qual -Wno-float-equal -Wno-atomic-implicit-seq-cst -Wno-char-subscripts"
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
$CXX $compiler_flags $mac_compiler_flags ../src/mac_slide_opengl.mm -fPIC -shared -o ../run_tree/mac_slide_opengl.dylib -ldl $linker_flags $mac_linker_flags -framework OpenGL -L../third_party/freetype-2.10.1/bin/lib -lfreetype

# NOTE(yuval): Mac Platform Compilation
$CXX $compiler_flags $mac_compiler_flags ../src/mac_slide.mm -o ../run_tree/mac_slide -ldl $linker_flags $mac_linker_flags

# NOTE(yuval): Compilation Teardown
popd > /dev/null