#include "Header.h"

#define SPHERE_SHELL
#define BOX_SHELL
#define PLANE_OF_TRIANGL

//________________________________________________Questions_Tasks__________________________________________________________________________//
// need do define on 1/2/3 -> 1D/2D/3D ??

//_________________________________________________________________________________________________________________________________________//


int main ()
{
    return 0;
}


//_________________________________________________________________________________________________________________________________________//
// получаем 2 треугольника с requires 3d
// строим плоскости по ним, точнее нормаль берем - 
// - получаем две нормали - 
// векторное произведение нормалей = линия пересечения двух плоскостейй треугольник

// счиатем расстояние от точек треугольника до другой плоскости
// если все одного знака -> выход из фукнции (кроме нуля )

// второй треугольник аналогично

// из расстояний находим через отношения точки которые лежат на прямой пересечения

// рассматриваем 2д случай

template <std::size_t Dimension>
constexpr bool Moller_alg (const Triangl<Dimension>& tr_1, const Triangl<Dimension>& tr_2) noexcept requires (Dimension == 3)
{
    
}


//_________________________________________________________________________________________________________________________________________//


//_____________________________________________________PLANE_3D____________________________________________________________________________//
class Plane3D
{
    //  Ax + By + Cz + D = 0  //  coeffs is normilized  //  A > 0  //  D = po
    private:
        precision_t A_, B_, C_, D_;

        void need_negative_li() noexcept
        {
            if (flush_to_zero(A_) != std::nullopt)
                if (A_ < 0)
                {
                    negative_all();
                    return;
                }

            if (flush_to_zero(B_) != std::nullopt)
                if (B_ < 0)
                {
                    negative_all();
                    return;
                }

            if (flush_to_zero(C_) != std::nullopt)
                if (C_ < 0)
                {
                    negative_all();
                    return;
                }
        }

        void negative_all() noexcept
        {
            A_ *= -1;
            B_ *= -1;
            C_ *= -1;
            D_ *= -1;
        }

        void default_construct_plane()  //  Plane on 4 point  //
        {
            if (flush_to_zero(A_) == std::nullopt && flush_to_zero(B_) == std::nullopt && flush_to_zero(C_) == std::nullopt)    
                Th_row(flush_to_zero(D_) != std::nullopt, std::invalid_argument, euqlid_ruined, "wrong D coeff in full zero A, B, C\n");

            precision_t normilize = std::sqrt (A_*A_ + B_*B_ + C_*C_);

            A_ /= normilize;
            B_ /= normilize;
            C_ /= normilize;
            D_ /= normilize;

            need_negative_li();
        }

    //_________________________________________________________________________________________________________________________________________//
    public:
        //  Zero plane  //
        explicit Plane3D() noexcept = default;

        //  Plane on 3 point  //
        explicit Plane3D(const Point<3>& p1, const Point<3>& p2, const Point<3>& p3)
        {
            Vector<3> normal = (p2 - p1) ^ (p3 - p1);

            A_ = normal.x();
            B_ = normal.y();
            C_ = normal.z();
            
            D_ = -(A_ * p1.x() + B_ * p1.y() + C_ * p1.z());

            default_construct_plane();
        }

        //  Plane on 4 point  //
        explicit Plane3D(const precision_t A_A, const precision_t B_B, const precision_t C_C, const precision_t D_D) : A_(A_A), B_(B_B), C_(C_C), D_(D_D)
        {
            default_construct_plane();
        }

        // T/T  -- absolute equival
        // T/F  -- parallel              //  compare_planes  //
        // F/.. -- mismatch
        std::pair<bool, bool> compare_planes (const Plane3D& sec_plane) const noexcept
        {
            if (flush_to_zero(A_ - sec_plane.A_) == std::nullopt && 
                flush_to_zero(B_ - sec_plane.B_) == std::nullopt && 
                flush_to_zero(C_ - sec_plane.C_) == std::nullopt    )
            {
                if (flush_to_zero(D_ - sec_plane.D_) == std::nullopt)
                    return std::pair{true, true};  // equival

                return std::pair{true, false};  // parallel
            }

            return std::pair{false, false};  // mismatch
        }

        //  is point lie on plane
        bool is_on_plane (const Point<3>& p) const noexcept
        {
            precision_t zero = A_ * p.x() + B_ * p.y() + C_ * p.z() + D_;

            if (flush_to_zero (zero) == std::nullopt)
                return true;

            return false;
        }

        // min po from plane to given point
        precision_t distance (const Point<3>& p) const noexcept
        {
            return std::abs (A_ * p.x() + B_ * p.y() + C_ * p.z() + D_);
        }

        // ret vector of normal
        constexpr Vector<3> normal() const noexcept
        {
            return Vector<3>{A_, B_, C_};
        }
};
//_________________________________________________________________________________________________________________________________________//


//______________________________________________________AABB_______________________________________________________________________________//
class Parallelepiped
{
    private:
        Point<3> min_left_, max_right_;

    public:
        explicit constexpr Parallelepiped() noexcept = default;
        explicit constexpr Parallelepiped(Point<3> p_min, Point<3> p_max) noexcept : min_left_(p_min), max_right_(p_max) {}

        // is_intersection?
        constexpr bool operator^(const Parallelepiped& another) const noexcept
        {
            return (another.min_left_.x() <= max_right_.x()) & (another.min_left_.y() <= max_right_.y()) & (another.min_left_.z() <= max_right_.z()) &
                   (another.min_left_.x() >= min_left_.x() ) & (another.min_left_.y() >= min_left_.y() ) & (another.min_left_.z() >= min_left_.z())  ;
        }
};
//_________________________________________________________________________________________________________________________________________//


//___________________________________________________QUATERNION____________________________________________________________________________//
class Quaternion
{
    private:
        using quat = std::complex<precision_t>;

        precision_t scalar_;
        Vector<3> i_vector_;

    public:
        explicit constexpr Quaternion() noexcept = default;
        explicit constexpr Quaternion(precision_t sc, Vector<3> imaginary_vector) noexcept : scalar_(sc), i_vector_(imaginary_vector) {}
};
//_________________________________________________________________________________________________________________________________________//


//_____________________________________________________MATRIX______________________________________________________________________________//
class Matrix
{};
//_________________________________________________________________________________________________________________________________________//


//_____________________________________________________TRIANGL_____________________________________________________________________________//
template <std::size_t Dimension>
class Triangl
{
    private:
        Point<Dimension> point_A_, point_B_, point_C_;

        #ifdef PLANE_OF_TRIANGL
            Plane3D plane_of_{};  // The plane in which the triangle lies
        #endif

        #ifdef BOX_SHELL
            Parallelepiped around_box_{};  // Parallelepiped HIT_BOX
        #endif

        #ifdef SPHERE_SHELL
            Circle<Dimension> around_circle_{};  // Sphere HIT_BOX
        #endif

        bool is_triangle_in_3D (Point<3> A, Point<3> B, Point<3> C) noexcept requires (Dimension == 3)
        {
            Vector<Dimension> U = B - A;
            Vector<Dimension> V = C - A;

            Vector<Dimension> n = U ^ V;  // [U x V]
            precision_t n2 = n * n;

            if (flush_to_zero (n2) == std::nullopt)
                return false;

            return true;  // all good
        }

        bool is_triangle_in_2D (Point<2> A, Point<2> B, Point<2> C) noexcept requires (Dimension == 2)
        {
            Line2D line(A, B);
            if (line.is_on_line(C))
                return false;

            return true;  // all good
        }

    //_________________________________________________________________________________________________________________________________________//
    public:
        explicit Triangl() noexcept requires (Dimension > 1) = default;
        explicit Triangl(const Point<Dimension> p_A, const Point<Dimension> p_B, const Point<Dimension> p_C) requires (Dimension > 1) : 
                                   point_A_(p_A), point_B_(p_B), point_C_(p_C)
        {
            if constexpr (Dimension == 3)
                if (is_triangle_in_3D (p_A, p_B, p_C))
                {
                    #ifdef PLANE_OF_TRIANGL
                        plane_of_ = Plane3D{p_A, p_B, p_C};  // can except!!
                    #endif

                    #ifdef BOX_SHELL
                        Point<3> min_p{};
                        Point<3> max_p{};

                        min_p.x() = std::min({p_A.x(), p_B.x(), p_C.x()});
                        min_p.y() = std::min({p_A.y(), p_B.y(), p_C.y()});
                        min_p.z() = std::min({p_A.z(), p_B.z(), p_C.z()});

                        max_p.x() = std::max({p_A.x(), p_B.x(), p_C.x()});
                        max_p.y() = std::max({p_A.y(), p_B.y(), p_C.y()});
                        max_p.z() = std::max({p_A.z(), p_B.z(), p_C.z()});

                        around_box_ = Parallelepiped(min_p, max_p);
                    #endif

                    #ifdef SPHERE_SHELL
                        around_circle_ = Circle<Dimension>(p_A, p_B, p_C);
                    #endif
                    
                    return;  // all good
                }

            if constexpr (Dimension == 2)
                if (is_triangle_in_2D (p_A, p_B, p_C))
                    return;  // all good

            throw std::invalid_argument("triangle is unreal");
        }                            

        constexpr Vector<3> normal() const noexcept requires (Dimension == 3)
        {
            #ifdef PLANE_OF_TRIANGL
                return plane_of_.normal();
            #endif
        }
};
//_________________________________________________________________________________________________________________________________________//


//______________________________________________________CIRCLE_____________________________________________________________________________//
template <std::size_t Dimension>
class Circle
{
    private:
        Point<Dimension> centr_;
        precision_t radius_;
        precision_t radius2_;       // save square of radius for optimize sqrt

    //_________________________________________________________________________________________________________________________________________//
    public:
        //  Zero constructor  //
        explicit Circle() noexcept = default;

        //  point + radius  //
        explicit Circle(Point<Dimension> heart, precision_t diameter_in_half) noexcept : 
        centr_(heart), radius_(diameter_in_half), radius2_(diameter_in_half * diameter_in_half) {}

        //  3 Points 3D  //
        explicit Circle(const Point<Dimension>& A, const Point<Dimension>& B, const Point<Dimension>& C) noexcept requires (Dimension == 3)
        {
            Vector<Dimension> U = B - A;
            precision_t U2 = U * U;

            Vector<Dimension> V = C - A;
            precision_t V2 = V * V;

            Vector<Dimension> n = U ^ V;  // [U x V]
            precision_t n2 = n * n;

            Vector<Dimension> m = V2 * U - U2 * V;

            Vector<Dimension> r = (m ^ n) / (2 * n2);

            ///////////////////////////////////////////
            centr_ = A + r;
            radius2_ = r * r;
            radius_ = std::sqrt(radius2_);
        }

        bool is_intersection (const Circle& other) const noexcept // work as sphere
        {
            precision_t dist = square_of_distance_bw_p (centr_, other.centr_);

            precision_t touch_dist = radius2_ + (2 * radius_ * other.radius_) + other.radius2_;

            if ((dist - touch_dist) <= EPSILON_OF_ZERO)
                return true;

            return false;
        }
};
//_________________________________________________________________________________________________________________________________________//


//_______________________________________________________LINE_2D___________________________________________________________________________//
class Line2D
{
    // When line parallel OY k = NAN && b = -x;
    private:
        precision_t k_;
        precision_t b_;

    //_________________________________________________________________________________________________________________________________________//
    public:
        //  Constructor based on 2 points  //
        explicit Line2D(Point<2> p_1, Point<2> p_2) noexcept
        {
            std::optional <std::pair<precision_t, precision_t>> coeffs = get_2Dline_coeffs (p_1, p_2);
        
            if (coeffs == std::nullopt)
            {
                k_ = NAN;
                b_ = -p_1.x();
                return;
            }

            k_ = coeffs->first;
            b_ = coeffs->second;
        }
        
        //  Constructor based on k b coefficients  //
        explicit Line2D(precision_t k_coef, precision_t b_coef) noexcept : k_(k_coef), b_(b_coef) {}

        //  true -- bottom then line  //  false -- upper then line  //  nullopt if parallel OY  //  if on line - use another func for check in exern
        std::optional<bool> is_below_the_line (const Point<2>& point) const noexcept
        {
            if (std::isnan(k_))
                return std::nullopt;

            precision_t y_on_line = k_ * point.x() + b_;

            if (y_on_line < point.y())
                return false;

            return true;
        }

        //  Check point on line  //
        bool is_on_line (const Point<2>& point) const noexcept
        {
            if (std::isnan(k_))
                return flush_to_zero (-b_ - point.x()) == std::nullopt;

            precision_t y_on_line = k_ * point.x() + b_;

            if (flush_to_zero (y_on_line - point.y()) == std::nullopt)
                return true;

            return false;
        }
    //_________________________________________________________________________________________________________________________________________//
};
constexpr std::optional <std::pair<precision_t, precision_t>> get_2Dline_coeffs (const Point<2>& p_1, 
                                                                                 const Point<2>& p_2) noexcept
{
    precision_t dx = p_2.x() - p_1.x();
    precision_t dy = p_2.y() - p_1.y();


    if (std::abs(dx) < EPSILON_OF_ZERO)  // parallel OY
        return std::nullopt;

    precision_t k = dy / dx;
    precision_t b = p_1.y() - k * p_1.x();

    return std::pair{k, b};
}
//_________________________________________________________________________________________________________________________________________//
