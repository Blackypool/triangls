cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # do compile_commands.json
set(CMAKE_CXX_SCAN_FOR_MODULES ON)

if(APPLE)
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm/bin/clang++")
endif()

project(Triangles_in_3D CXX)
#//_______________________________________________________________________________________________________________________________________//#


add_executable(Triangles M_triangls.cpp)

target_sources(Triangles
                    PUBLIC
                    FILE_SET cxx_geometry_modules 
                    TYPE CXX_MODULES 
                    FILES
                        geometry/point_v.cppm
                        geometry/areas.cppm
                        geometry/geometry.cppm
)

#//___________________________________________________PROPERTIES__________________________________________________________________________//#
set_target_properties(Triangles PROPERTIES
                      OUTPUT_NAME "app"
                      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/_out_puts")
#//_______________________________________________________________________________________________________________________________________//#


#//______________________________________________________FLAGS____________________________________________________________________________//#
if(APPLE)
#{
    set(PLATFORM APPLE)
    target_compile_options(Triangles PRIVATE 
    -O3 -D _DEBUG -ggdb3 -Wall -Wextra -Wmissing-declarations
    -Wcast-align -Wcast-qual -Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body
    -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline
    -Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls
    -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-overflow=2 -Wsuggest-override -Wswitch-default
    -Wswitch-enum -Wundef -Wunreachable-code -Wunused -Wvariadic-macros -Wno-missing-field-initializers
    -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector
    -fstrict-overflow -fno-omit-frame-pointer -Wlarger-than=8192 -fPIE -Werror=vla
    -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
#}
elseif(UNIX)
#{
    set(PLATFORM UNIX)
    target_compile_options(Triangles PRIVATE 
    -O3 -D _DEBUG -ggdb3 -Wall -Wextra -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations
    -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body
    -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor
    -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion
    -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods
    -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code
    -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing
    -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow
    -flto-odr-type-merging -fno-omit-frame-pointer -pie -fPIE -Werror=vla
    -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
#}
elseif(WIN32)
#{
    set(PLATFORM WIN32)
    target_compile_options(Triangles PRIVATE 
    -O3 -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef -Wfloat-equal -Winline -Wunreachable-code
    -Wmissing-declarations -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Wmain -Wextra -Wall -g -pipe
    -fexceptions -Wcast-qual -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security -Wformat=2 -Wignored-qualifiers
    -Wlogical-op -Wno-missing-field-initializers -Wnon-virtual-dtor -Woverloaded-virtual -Wpointer-arith -Wsign-promo
    -Wstack-usage=8192 -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits -Wwrite-strings -Werror=vla -D_DEBUG
    -D_EJUDGE_CLIENT_SIDE)
#}
endif()
target_link_options(Triangles PRIVATE -fsanitize=address,undefined)
message(STATUS "${PLATFORM} flags used\n")
#//_______________________________________________________________________________________________________________________________________//#