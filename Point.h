#ifndef UNIVERSAL_MULTI_DIMENSION_CLASS_POINT_AND_VECTOR
#define UNIVERSAL_MULTI_DIMENSION_CLASS_POINT_AND_VECTOR

#include "Header.h"

template <std::size_t Dimension>
class Vector;

//_________________________________________________________________________________________________________________________________________//
template <std::size_t Dimension>
class Point
{
    protected:  // derived is see this section
        precision_t all_coords_[Dimension]{};  // know on pre-proc

    //_________________________________________________________________________________________________________________________________________//
    public:

        constexpr       precision_t& x()       requires (Dimension >= 1) { return all_coords_[0]; }  // constexpr - compile-time opt +how inline
        constexpr const precision_t& x() const requires (Dimension >= 1) { return all_coords_[0]; }  // const for save coords + const points

        constexpr       precision_t& y()       requires (Dimension >= 2) { return all_coords_[1]; }
        constexpr const precision_t& y() const requires (Dimension >= 2) { return all_coords_[1]; }

        constexpr       precision_t& z()       requires (Dimension >= 3) { return all_coords_[2]; }
        constexpr const precision_t& z() const requires (Dimension >= 3) { return all_coords_[2]; }

        constexpr       precision_t& operator[](std::size_t index)       { return all_coords_[index]; }
        constexpr const precision_t& operator[](std::size_t index) const { return all_coords_[index]; }

    //_________________________________________________________________________________________________________________________________________//
        //  D  //  Constructor for comfortable Zero point
        explicit constexpr Point() = default;

        //  0  //  Constructor
        template <is_Decard_dim TypeCoord>
        explicit constexpr Point(std::initializer_list <TypeCoord> list)  // explicit is protection from implicit cast
        {
            size_t i = 0;
            for (const TypeCoord& val : list)
                if (i < Dimension)
                    all_coords_[i++] = static_cast<precision_t>(val);  // not need move, because initializer_list is not owned data
        }
    //_________________________________________________________________________________________________________________________________________//

        //  ==  //
        constexpr bool operator==(const Point<Dimension>& other) const
        {
            for (std::size_t i = 0; i < Dimension; i++)
                if (flush_to_zero (all_coords_[i] - other[i]) != std::nullopt)
                    return false;

            return true;
        }

        //  -point  //
        constexpr Point<Dimension> operator-() const
        {
            Point<Dimension> result{};
            for (std::size_t i = 0; i < Dimension; i++)
                result[i] = -all_coords_[i];

            return result;
        }
};
//_________________________________________________________________________________________________________________________________________//
        //  Distance between points  //
        template <std::size_t Dimension>
        constexpr precision_t distance_bw_p (const Point<Dimension>& p_1, const Point<Dimension>& p_2)
        {
            precision_t sum_sq{};

            for (std::size_t i = 0; i < Dimension; ++i)
            {
                precision_t diff = p_1[i] - p_2[i];
                sum_sq = sum_sq + (diff * diff);
            }

            return std::sqrt(sum_sq);
        }

        //  Square of istance between points -- use for optimizations  //
        template <std::size_t Dimension>
        constexpr precision_t square_of_distance_bw_p (const Point<Dimension>& p_1, const Point<Dimension>& p_2)
        {
            precision_t sum_sq{};

            for (std::size_t i = 0; i < Dimension; ++i)
            {
                precision_t diff = p_1[i] - p_2[i];
                sum_sq = sum_sq + (diff * diff);
            }

            return sum_sq;
        }
//_________________________________________________________________________________________________________________________________________//


template <std::size_t Dimension>
class Vector : public Point<Dimension>
{
    public:
        using Point<Dimension>::Point;  //  For use constructors of Point

        //  ADD  //
        constexpr Vector<Dimension> operator+(const Vector<Dimension>& other) const
        {
            Vector result = *this;
            for (std::size_t i = 0; i < Dimension; i++)
                result[i] += other[i];

            return result;
        }

        //  -vector  //
        constexpr Vector<Dimension> operator-() const
        {
            Vector<Dimension> result{};
            for (std::size_t i = 0; i < Dimension; i++)
                result[i] = -(this->all_coords_[i]);

            return result;
        }

        //  Dot Product  //  (v1 * v2)
        constexpr precision_t dot (const Vector<Dimension>& other) const
        {
            precision_t sum{};
            for (std::size_t i = 0; i < Dimension; i++)
                sum += other[i] * (this->all_coords_[i]);

            return sum;
        }

        //  Vector product (only 3D)  //  [v1 x v2]
        constexpr Vector<3> operator^(const Vector<3>& other) const requires (Dimension == 3)
        {
            return Vector<3>{
                                this->y() * other.z() - this->z() * other.y(),
                                this->z() * other.x() - this->x() * other.z(),
                                this->x() * other.y() - this->y() * other.x()
                            };
        }

        //  Lenght  //
        constexpr precision_t length () const
        {
            return std::sqrt(dot(*this));
        }

        //  *  scalar  //
        constexpr Vector<Dimension> operator*(precision_t scalar) const 
        {
            Vector result = *this;
            for (std::size_t i = 0; i < Dimension; i++)
                result[i] *= scalar;

            return result;
        }
};
//_________________________________________________________________________________________________________________________________________//
        //  scalar *  //
        template <std::size_t Dimension>
        constexpr Vector<Dimension> operator*(precision_t scalar, const Vector<Dimension>& vector) 
        {
            return vector * scalar;
        }

        //  SUB  //
        template <std::size_t Dimension>
        constexpr Vector<Dimension> operator-(const Point<Dimension>& left_p, const Point<Dimension>& right_p)
        {
            Vector result{};
            for (std::size_t i = 0; i < Dimension; i++)
                result[i] = left_p[i] - right_p[i];

            return result;
        }

        //  Point + Vector  //
        template <std::size_t Dimension>
        constexpr Point<Dimension> operator+(const Point<Dimension>& point, const Vector<Dimension>& vector)
        {
            Point<Dimension> result{};
            for (std::size_t i = 0; i < Dimension; i++)
                result[i] = point[i] + vector[i];

            return result;
        }

        //  Vector + Point  //
        template <std::size_t Dimension>
        constexpr Point<Dimension> operator+(const Vector<Dimension>& vector, const Point<Dimension>& point)
        {
            return point + vector;
        }

        //  <<  //  external of class function, because need (os, p)  //  Also work for Vector
        template <std::size_t Dimension>
        std::ostream& operator<<(std::ostream& os, const Point<Dimension>& point)  // & because os havnt got copy methods
        {
            os << "{ ";

            for (std::size_t i = 0; i < Dimension; i++)
                os << point[i] << " ";

            return os << "}";
        }
//_________________________________________________________________________________________________________________________________________//


#endif // UNIVERSAL_MULTI_DIMENSION_CLASS_POINT_AND_VECTOR