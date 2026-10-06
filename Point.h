#ifndef UNIVERSAL_MULTI_DIMENSION_CLASS_POINT
#define UNIVERSAL_MULTI_DIMENSION_CLASS_POINT


#include "Header.h"
#include "Do_any_type_zero.h"


template <typename T>
concept is_Decard_dim = requires(T a, T b)
{
    a + b;
    {a != b} -> std::convertible_to<bool>;
};


template <is_Decard_dim TypeCoord, std::size_t Dimension>
class Point
{
    private:
        TypeCoord all_coords_[Dimension]{};  // know on pre-proc

    //_________________________________________________________________________________________________________________________________________//
    public:

        constexpr       TypeCoord& x()       requires (Dimension >= 1) { return all_coords_[0]; }  // constexpr - compile-time opt, work how inline
        constexpr const TypeCoord& x() const requires (Dimension >= 1) { return all_coords_[0]; }  // const for save coords + const points

        constexpr       TypeCoord& y()       requires (Dimension >= 2) { return all_coords_[1]; }
        constexpr const TypeCoord& y() const requires (Dimension >= 2) { return all_coords_[1]; }

        constexpr       TypeCoord& z()       requires (Dimension >= 3) { return all_coords_[2]; }
        constexpr const TypeCoord& z() const requires (Dimension >= 3) { return all_coords_[2]; }

        constexpr       TypeCoord& operator[](std::size_t index)       { return all_coords_[index]; }
        constexpr const TypeCoord& operator[](std::size_t index) const { return all_coords_[index]; }

    //_________________________________________________________________________________________________________________________________________//
        //  D  //  Constructor
        Point() = default;  // for comfortable Zero point

        //  0  //  Constructor
        explicit Point(std::initializer_list <TypeCoord> list)  // explicit is protection from implicit cast
        {
            size_t i = 0;
            for (const TypeCoord& val : list)      // std::initializer_list have only ptr+size
                if (i < Dimension)
                    all_coords_[i++] = val;  // not need move, because initializer_list is not owned data
        }

        //  1  //  Destructor
        ~Point() = default;

        //  2  //  Copy in new
        Point(const Point& other)  // copy other in new obj <=> constructor
        {
            for (std::size_t i = 0; i < Dimension; i++)
                all_coords_[i] = other.all_coords_[i];
        }

        //  3  //  Copy in exist
        Point& operator=(const Point& other)  // copy other to exist
        {
            if (this == &other)  // this = ptr
                return *this;

            for (std::size_t i = 0; i < Dimension; i++)
                all_coords_[i] = other.all_coords_[i];

            return *this;
        }

        //  4  //  Move to new
        Point(Point&& other) noexcept  // copy other in new obj + nulled other
        {
            for (std::size_t i = 0; i < Dimension; i++)
            {
                all_coords_[i] = std::move(other.all_coords_[i]);
                do_zero (other.all_coords_[i]);
            }
        }

        //  5  //  Move to exist
        Point& operator=(Point&& other) noexcept  // copy other in new obj + nulled other // not need zerofied our obj
        {
            if (this == &other)  // this = ptr
                return *this;

            for (std::size_t i = 0; i < Dimension; i++)
            {
                all_coords_[i] = std::move(other.all_coords_[i]);
                do_zero (other.all_coords_[i]);
            }

            return *this;
        }
    //_________________________________________________________________________________________________________________________________________//
};


#endif // UNIVERSAL_MULTI_DIMENSION_CLASS_POINT