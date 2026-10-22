module;
#include "Header.h"

export module geometry:areas;

import :point_v;

export{
//_____________________________________________________PLANE_3D____________________________________________________________________________//
class alignas(GEOM_ALIGNMENT) Plane3D
{
    //  Ax + By + Cz + D = 0  //  coeffs is normilized  //  A > 0  //  D = po
    private:
        precision_t A_, B_, C_, D_;

        constexpr void need_negative_li() noexcept
        {
            if (!is_nearly_zero(A_))
            {
                if (A_ < 0)
                    negative_all();
                return;
            }

            if (!is_nearly_zero(B_))
            {
                if (B_ < 0)
                    negative_all();
                return;
            }

            if (!is_nearly_zero(C_))
            {
                if (C_ < 0)
                    negative_all();
                return;
            }
        }

        constexpr void negative_all() noexcept
        {
            A_ *= -1;
            B_ *= -1;
            C_ *= -1;
            D_ *= -1;
        }

        constexpr void default_construct_plane()  //  Plane on 4 point  //
        {
            if (is_nearly_zero(A_) && is_nearly_zero(B_) && is_nearly_zero(C_))  [[unlikely]]
                Th_row(!is_nearly_zero(D_), std::invalid_argument, euqlid_ruined, "wrong D coeff in full zero A, B, C\n");

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
        constexpr explicit Plane3D() noexcept = default;

        //  Plane on 3 point  //
        constexpr explicit Plane3D(const Point<3>& p1, const Point<3>& p2, const Point<3>& p3)
        {
            Vector<3> normal = (p2 - p1) ^ (p3 - p1);

            A_ = normal.x();
            B_ = normal.y();
            C_ = normal.z();
            
            D_ = -(A_ * p1.x() + B_ * p1.y() + C_ * p1.z());

            default_construct_plane();
        }

        //  Plane on 4 point  //
        constexpr explicit Plane3D(const precision_t A_A, const precision_t B_B, const precision_t C_C, const precision_t D_D) : A_(A_A), B_(B_B), C_(C_C), D_(D_D)
        {
            default_construct_plane();
        }

        // T/T  -- absolute equival
        // T/F  -- parallel              //  compare_planes  //
        // F/F  -- mismatch
        constexpr std::pair<bool, bool> compare_planes (const Plane3D& sec_plane) const noexcept
        {
            if (is_nearly_zero(A_ - sec_plane.A_) && 
                is_nearly_zero(B_ - sec_plane.B_) && 
                is_nearly_zero(C_ - sec_plane.C_)   )
            {
                if (is_nearly_zero(D_ - sec_plane.D_))
                    return std::pair{true, true};  // equival

                return std::pair{true, false};  // parallel
            }

            return std::pair{false, false};  // mismatch
        }

        //  is point lie on plane
        constexpr bool is_on_plane (const Point<3>& p) const noexcept
        {
            precision_t zero = A_ * p.x() + B_ * p.y() + C_ * p.z() + D_;

            if (is_nearly_zero (zero))
                return true;

            return false;
        }

        // min po from plane to given point
        constexpr precision_t distance (const Point<3>& p) const noexcept
        {
            return (A_ * p.x() + B_ * p.y() + C_ * p.z() + D_);
        }

        // ret vector of normal
        constexpr Vector<3> normal() const noexcept
        {
            return Vector<3>{A_, B_, C_};
        }
};
//_________________________________________________________________________________________________________________________________________//


//__________________________________________________Parallelepiped_________________________________________________________________________//
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
            int c1 = (another.min_left_.x() <= max_right_.x());
            int c2 = (another.min_left_.y() <= max_right_.y());
            int c3 = (another.min_left_.z() <= max_right_.z());
            int c4 = (another.max_right_.x() >= min_left_.x());
            int c5 = (another.max_right_.y() >= min_left_.y());
            int c6 = (another.max_right_.z() >= min_left_.z());

            return (c1 * c2 * c3 * c4 * c5 * c6) != 0;
        }
};
//_________________________________________________________________________________________________________________________________________//


//______________________________________________________CIRCLE_____________________________________________________________________________//
template <std::size_t Dimension>
class alignas(GEOM_ALIGNMENT) Circle
{
    private:
        Point<Dimension> centr_;
        precision_t radius_;
        precision_t radius2_;       // save square of radius for optimize sqrt

    //_________________________________________________________________________________________________________________________________________//
    public:
        //  Zero constructor  //
        constexpr explicit Circle() noexcept = default;

        //  point + radius  //
        constexpr explicit Circle(Point<Dimension> heart, precision_t diameter_in_half) noexcept : 
        centr_(heart), radius_(diameter_in_half), radius2_(diameter_in_half * diameter_in_half) {}

        //  3 Points 3D  //
        constexpr explicit Circle(const Point<Dimension>& A, const Point<Dimension>& B, const Point<Dimension>& C) noexcept requires (Dimension == 3)
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

        //  is_intersection
        constexpr bool operator^(const Circle& other) const noexcept // work as sphere
        {
            precision_t dist = square_of_distance_bw_p (centr_, other.centr_);

            precision_t touch_dist = radius2_ + (2 * radius_ * other.radius_) + other.radius2_;

            if ((dist - touch_dist) <= EPSILON_OF_ZERO)  // need sign
                return true;

            return false;
        }
};
//_________________________________________________________________________________________________________________________________________//


//______________________________________________________LINE_2D____________________________________________________________________________//
constexpr std::optional <std::pair<precision_t, precision_t>> get_2Dline_coeffs (const Point<2>& p_1, 
                                                                                 const Point<2>& p_2) noexcept
{
    precision_t dx = p_2.x() - p_1.x();
    precision_t dy = p_2.y() - p_1.y();

    if (is_nearly_zero(dx))  [[unlikely]] // parallel OY
        return std::nullopt;

    precision_t k = dy / dx;
    precision_t b = p_1.y() - k * p_1.x();

    return std::pair{k, b};
}
class alignas(GEOM_ALIGNMENT) Line2D
{
    // When line parallel OY k = NAN && b = -x;
    private:
        precision_t k_;
        precision_t b_;

    //_________________________________________________________________________________________________________________________________________//
    public:
        //  Constructor based on 2 points  //
        constexpr explicit Line2D(Point<2> p_1, Point<2> p_2) noexcept
        {
            std::optional <std::pair<precision_t, precision_t>> coeffs = get_2Dline_coeffs (p_1, p_2);
        
            if (coeffs == std::nullopt)  [[unlikely]]
            {
                k_ = NAN;
                b_ = -p_1.x();
                return;
            }

            k_ = coeffs->first;
            b_ = coeffs->second;
        }
        
        //  Constructor based on k b coefficients  //
        constexpr explicit Line2D(precision_t k_coef, precision_t b_coef) noexcept : k_(k_coef), b_(b_coef) {}

        //  true -- bottom then line  //  false -- upper then line  //  nullopt if parallel OY  //  if on line - use another func for check in exern
        constexpr std::optional<bool> is_below_the_line (const Point<2>& point) const noexcept
        {
            if (std::isnan(k_))  [[unlikely]]
                return std::nullopt;

            precision_t y_on_line = k_ * point.x() + b_;

            if (y_on_line < point.y())
                return false;

            return true;
        }

        //  Check point on line  //
        constexpr bool is_on_line (const Point<2>& point) const noexcept
        {
            if (std::isnan(k_))  [[unlikely]]
                return is_nearly_zero (-b_ - point.x());

            precision_t y_on_line = k_ * point.x() + b_;

            if (is_nearly_zero (y_on_line - point.y()))
                return true;

            return false;
        }
    //_________________________________________________________________________________________________________________________________________//
};
//_________________________________________________________________________________________________________________________________________//


//______________________________________________________LINE_3D____________________________________________________________________________//
class Line3D
{
    private:
        Point<3> point_on_line_;
        Vector<3> slide_;

    public:
        constexpr explicit Line3D() noexcept = default;
        constexpr Line3D(const Point<3>& p, const Vector<3>& v) noexcept : point_on_line_(p), slide_(v) {}

        constexpr const Point<3> get_point ()   const noexcept {return point_on_line_;}
        constexpr const Vector<3> get_vector () const noexcept {return slide_;}
};
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

        #ifdef AABB_SHELL
            Parallelepiped around_box_{};  // Parallelepiped HIT_BOX
        #endif

        #ifdef SPHERE_SHELL
            Circle<Dimension> around_circle_{};  // Sphere HIT_BOX
        #endif

        constexpr bool is_triangle_in_3D (Point<3> A, Point<3> B, Point<3> C) noexcept requires (Dimension == 3)
        {
            Vector<Dimension> U = B - A;
            Vector<Dimension> V = C - A;

            Vector<Dimension> n = U ^ V;  // [U x V]
            precision_t n2 = n * n;

            if (is_nearly_zero (n2))  [[unlikely]]
                return false;

            return true;  // all good
        }

        constexpr bool is_triangle_in_2D (Point<2> A, Point<2> B, Point<2> C) noexcept requires (Dimension == 2)
        {
            Line2D line(A, B);
            if (line.is_on_line(C))  [[unlikely]]
                return false;

            return true;  // all good
        }

    //_________________________________________________________________________________________________________________________________________//
    public:
        constexpr Triangl() noexcept requires (Dimension > 1) = default;
        constexpr Triangl(const Point<Dimension> p_A, const Point<Dimension> p_B, const Point<Dimension> p_C) requires (Dimension > 1) : 
                                   point_A_(p_A), point_B_(p_B), point_C_(p_C)
        {
            if constexpr (Dimension == 3)
                if (is_triangle_in_3D (p_A, p_B, p_C))  [[likely]]
                {
                    #ifdef PLANE_OF_TRIANGL
                        plane_of_ = Plane3D{p_A, p_B, p_C};  // can except!!
                    #endif

                    #ifdef AABB_SHELL
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
                if (is_triangle_in_2D (p_A, p_B, p_C))  [[likely]]
                    return;  // all good

            static_assert(true, "triangle is unreal");  // need points printf!
            throw std::invalid_argument("triangle is unreal");
        }                            

        constexpr const Point<Dimension> get_A () const noexcept {return point_A_;}
        constexpr const Point<Dimension> get_B () const noexcept {return point_B_;}
        constexpr const Point<Dimension> get_C () const noexcept {return point_C_;}

        constexpr const Line3D get_AB () const noexcept requires (Dimension == 3) { return {point_A_, point_B_ - point_A_}; }
        constexpr const Line3D get_BC () const noexcept requires (Dimension == 3) { return {point_B_, point_C_ - point_B_}; }
        constexpr const Line3D get_AC () const noexcept requires (Dimension == 3) { return {point_A_, point_C_ - point_A_}; }

        #ifdef PLANE_OF_TRIANGL
            constexpr const Plane3D get_plane ()  const noexcept {return plane_of_;}
            constexpr Vector<3> normal()          const noexcept {return plane_of_.normal();}
        #endif

        #ifdef AABB_SHELL
            constexpr const Parallelepiped get_AABB () const noexcept {return around_box_;}
        #endif

        #ifdef SPHERE_SHELL
            constexpr const Circle<Dimension> get_sphere () const noexcept {return around_circle_;}
        #endif
};
//_________________________________________________________________________________________________________________________________________//
}