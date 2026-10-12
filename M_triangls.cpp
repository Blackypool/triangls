#include "Header.h"
import geometry;
// #include "geometry/point_v.cppm"
// #include "geometry/areas.cppm"


//________________________________________________Questions_Tasks__________________________________________________________________________//

//_________________________________________________________________________________________________________________________________________//


int main ()
{
    return 0;
}


//_________________________________________________COMPARE_IN_3D____________________________________________________________________________//
template <std::size_t Dimension>
constexpr bool intersection_in_3D (const Triangl<Dimension>& tr_1, const Triangl<Dimension>& tr_2) noexcept requires (Dimension == 3)  // TRIANGEL + TRIANGEL //
{
    ///////////////  BROAD_PHASE  ///////////////
    #ifdef AABB_SHELL
        if ( !(tr_1.get_AABB() ^ tr_2.get_AABB()) )
            return false;
    #endif
    
    #ifdef SPHERE_SHELL
        if ( !(tr_1.get_sphere() ^ tr_2.get_sphere()) )
            return false;
    #endif
    /////////////////////////////////////////////


    ///////////////  PARALLELISM  ///////////////
    std::pair<bool, bool> is_parallel = tr_1.get_plane().compare_planes(tr_2.get_plane());

    if (is_parallel.first == true)
    {
        if (is_parallel.second == true)  // triangles lie in one plane
            return intersection_in_2D(tr_1, tr_2);

        return false;  // parallel planes
    }  
    // else == intersection of planes => need narrow phase


    ///////////////  NARROW_PHASE  //////////////
    #ifdef MOLLER_ALG
        return complete_intersection_triangls (tr_1, tr_2);
    #endif
    /////////////////////////////////////////////
}

template <std::size_t Dimension>            
constexpr bool intersection_in_3D (const Triangl<Dimension>& trngl, const Point<Dimension>& pnt)   noexcept requires (Dimension == 3)  // TRIANGEL + POINT //
{
    ///////////////  BROAD_PHASE  ///////////////
    if ( !(trngl.get_plane().is_on_plane(pnt)) )
        return false;
    /////////////////////////////////////////////


    ///////////////  NARROW_PHASE  //////////////
    return intersection_in_2D (trngl, pnt);
    /////////////////////////////////////////////
}
//_________________________________________________________________________________________________________________________________________//


//________________________________________________COMPARE_IN_2D____________________________________________________________________________//
constexpr bool intersection_in_2D (const Triangl<3>& tr_1, const Triangl<3>& tr_2) noexcept // TRIANGEL + TRIANGEL //
{
    bool is_A_in_tr = intersection_in_2D (tr_1, tr_2.get_A());
    bool is_B_in_tr = intersection_in_2D (tr_1, tr_2.get_B());
    bool is_C_in_tr = intersection_in_2D (tr_1, tr_2.get_C());

    return is_A_in_tr || is_B_in_tr || is_C_in_tr;  // if there is at least one
}

constexpr bool intersection_in_2D (const Triangl<3>& trngl, const Point<3>& pnt) noexcept  // TRIANGEL + POINT //  // 15+ 15* 7cmp
{
    Vector<3> normal = trngl.normal();
    auto get_signed_sq_area = [=](const Point<3>& p1, const Point<3>& p2)
    {
        Vector<3> p1_p2 = p2 - p1;
        Vector<3> p1_P = pnt - p1;

        Vector<3> perpend = p1_p2 ^ p1_P;   // do normal along side and line to the point

        return normal.dot(perpend);         // check sign of parallellism
    };

    const Point<3>& A = trngl.get_A();
    const Point<3>& B = trngl.get_B();
    const Point<3>& C = trngl.get_C();

    precision_t sq_area_1 = get_signed_sq_area (A, B);
    precision_t sq_area_2 = get_signed_sq_area (B, C);
    precision_t sq_area_3 = get_signed_sq_area (C, A);

    bool has_neg = (sq_area_1 < -EPSILON_OF_ZERO) || (sq_area_2 < -EPSILON_OF_ZERO) || (sq_area_3 < -EPSILON_OF_ZERO);
    bool has_pos = (sq_area_1 > EPSILON_OF_ZERO)  || (sq_area_2 > EPSILON_OF_ZERO)  || (sq_area_3 > EPSILON_OF_ZERO);

    return !(has_neg && has_pos); // if there are no both negative and positive aspects
}
//_________________________________________________________________________________________________________________________________________//


//_______________________________________________NARROW_PHASE______________________________________________________________________________//
constexpr bool complete_intersection_triangls (const Triangl<3>& tr_1, const Triangl<3>& tr_2) noexcept
{
    // points lie on the same side of the line of intersection of the planes
    if (check_sign_of_len (tr_1, tr_2.get_plane())) return false;
    if (check_sign_of_len (tr_2, tr_1.get_plane())) return false;
    
    Vector<3> normal_1 = tr_1.normal();
    Vector<3> normal_2 = tr_2.normal();

    Vector<3> line_intersection = normal_1 ^ normal_2;

    // из расстояний находим через отношения точки которые лежат на прямой пересечения
    // рассматриваем 2д случай
}

// возвращать пару: структуру с длинами и булево?
template <std::size_t Dimension>
constexpr bool check_sign_of_len (const Triangl<Dimension>& tr, const Plane3D& plane) noexcept // return true if all len from points of tr have one sign
{
    precision_t len_A = plane.distance(tr.get_A());
    precision_t len_B = plane.distance(tr.get_B());
    precision_t len_C = plane.distance(tr.get_C());

    if (is_nearly_zero(len_A)) return false;
    if (is_nearly_zero(len_B)) return false;
    if (is_nearly_zero(len_C)) return false;

    if (len_A > 0)
    {
        if (len_B > 0)
        {
            if (len_C > 0)
                return true;
            
            return false;
        }
        return false;
    }

    if (len_A < 0)
    {
        if (len_B < 0)
        {
            if (len_C < 0)
                return true;
            
            return false;
        }
        return false;
    }

    return false;
}
//_________________________________________________________________________________________________________________________________________//
