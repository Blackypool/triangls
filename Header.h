#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H

//______________________________________________BIBLIOTECS_________________________________________________________________________________//
#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <cstddef>
#include <sys/stat.h>
#include <vector>
#include <utility>
#include <optional>
#include <list>
#include <unordered_map>
#include <concepts>
#include <limits>
#include <initializer_list>
#include <type_traits>
#include <string>
#include <string_view>
#include <cmath>
#include <complex>
#include <array>
//_________________________________________________________________________________________________________________________________________//


//_____________________________________________OPTIMIZATIONS_______________________________________________________________________________//
#define PLANE_OF_TRIANGL    // Should the plane be stored in the triangle class?

#define MOLLER_ALG          // Using the Moeller algorithm to accurately check the intersection

#define SUM_AREA_for_check_point_in_triangel  // Checking whether a point lies within a triangle by summing the areas

#define AABB_SHELL          // Using AABB for broad check the intersection 

#ifndef AABB_SHELL
    #define SPHERE_SHELL    // Using Sphere around the triangle for broad check the intersection 
#endif

#define EPSILON_OF_ZERO 1e-4
using precision_t = float;
inline constexpr std::size_t GEOM_ALIGNMENT = 16;  // alignment of basic geometry structures
//_________________________________________________________________________________________________________________________________________//


//_________________________________________EXCLUSIVELY_FOR_PROJECT_________________________________________________________________________//
template <typename T>
concept is_Decard_dim = requires(T a, T b)
{
    a + b;
    {a != b} -> std::convertible_to<bool>;
};
//_________________________________________________________________________________________________________________________________________//


//____________________________________________FAST_COMMANDS________________________________________________________________________________//
    #define DE_BUG_ON
//_______________________________
    // rm -rf build
    // cmake -S . -B build -G Ninja         --  -S (source) . == where CMake.txt
    //                                      --  -B build == where save generated files (in build folder)
    //                                      --  -G (Generator) -- performing instrument
    // cmake --build build
//_______________________________
    // git add .
    // git commit -m "add source for generating tests"
    // git push
//_______________________________
    // git pull

    // git fetch origin // only download
    // git switch Name_of_branch
    // git branch
//_________________________________________________________________________________________________________________________________________//


//________________________________________________APPARAT__________________________________________________________________________________//
#define TO_str(x) #x


#define ERROR_IN_FUN -1
#define ALL_RIGHT     1

enum [[nodiscard]] errors_  // nodiscard -- no ignor result of func w\ this enum
{
    memory_aloca    = 1,
    oversize_any    = 2,
    syntax_err      = 3,
    give_null_ptr   = 4,
    file_errorr     = 5,
    error_in_deep   = 6,
    stack_errorr    = 7,
    load_data       = 8,
    euqlid_ruined   = 9,
};


#ifdef DE_BUG_ON
    #define Th_row(what_need, ExceptionType, type_err, message) \
        if (what_need) [[unlikely]] \
        { \
            std::string err_msg = std::string("\n[Error] ") + __FILE__ + ":" + std::to_string(__LINE__) + \
                                            " | Type: " + TO_str(type_err) + " | Info: " + (message);   \
            throw ExceptionType(err_msg); }
#else 
    #define Th_row(what_need, ExceptionType, type_err, message)
#endif     
//_________________________________________________________________________________________________________________________________________//



//______________________________________________HEADERS_OF_ANOTHER_________________________________________________________________________//
#include "Dop_math.h"
//_________________________________________________________________________________________________________________________________________//



#endif // MAIN_HEADER_H