
set(gcc_like_cxx $<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>)
set(msvc_cxx $<COMPILE_LANG_AND_ID:CXX,MSVC>)

set(gcc_like_cxx_flags -Wall;-Wextra;-pedantic;-Wextra;-Wshadow;-Wconversion)
set(msvc_cxx_flags -W4)

add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_17)
target_compile_options(compiler_flags INTERFACE
    $<${gcc_like_cxx}: $<BUILD_INTERFACE: ${gcc_like_cxx_flags}>>
    $<${msvc_cxx}: $<BUILD_INTERFACE: ${msvc_cxx_flags}>>
)
