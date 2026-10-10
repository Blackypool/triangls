#ifndef MAIN_HEADER_H
#define MAIN_HEADER_H


#define EPSILON_OF_ZERO 1e-4
#define DE_BUG_ON
#define TO_str(x) #x


//_________________________________________EXCLUSIVELY_FOR_PROJECT_________________________________________________________________________//
using precision_t = double;

template <typename T>
concept is_Decard_dim = requires(T a, T b)
{
    a + b;
    {a != b} -> std::convertible_to<bool>;
};
//_________________________________________________________________________________________________________________________________________//



//____________________________________________FAST_COMMANDS________________________________________________________________________________//
    // git add .
    // git commit -m "upgrade Geometry + do wrappe for throw"
    // git push

    // git pull

    // git fetch origin // only download
    // git switch Name_of_branch
    // git branch
//_________________________________________________________________________________________________________________________________________//



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
//_________________________________________________________________________________________________________________________________________//



//________________________________________________DEBUGS___________________________________________________________________________________//
#define ERROR_IN_FUN -1
#define ALL_RIGHT     1

enum [[nodiscard]] errors_
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
            throw ExceptionType(err_msg); \
        }
#else 
    #define Th_row(what_need, ExceptionType, type_err, message)
#endif     
//_________________________________________________________________________________________________________________________________________//



//______________________________________________HEADERS_OF_ANOTHER_________________________________________________________________________//
#include "Dop_math.h"
#include "Point.h"
//_________________________________________________________________________________________________________________________________________//



#endif // MAIN_HEADER_H