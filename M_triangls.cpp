#include "Header.h"
// import geometry;
#include "geometry/point_v.cppm"
#include "geometry/areas.cppm"


//________________________________________________Questions_Tasks__________________________________________________________________________//
// добавить AABB для больших наборов треугольников
// только как эти наборы строить тогда

// SAT for 2D need do for 2d intersection
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

    if (is_parallel.first == true)  [[unlikely]]  // overall, this is an unlikely, both in random encounters and in video games when AABB matches
    {
        if (is_parallel.second == true)  // triangles lie in one plane // cant predict w\o additional information
            return intersection_in_2D(tr_1, tr_2);

        return false;  // parallel planes
    }  
    // else == intersection of planes => need narrow phase


    ///////////////  NARROW_PHASE  //////////////
    return complete_intersection_triangls (tr_1, tr_2);
    /////////////////////////////////////////////
}

template <std::size_t Dimension>            
constexpr bool intersection_in_3D (const Triangl<Dimension>& trngl, const Point<Dimension>& pnt)   noexcept requires (Dimension == 3)  // TRIANGEL + POINT //
{
    ///////////////  BROAD_PHASE  ///////////////
    if ( !(trngl.get_plane().is_on_plane(pnt)) )  [[likely]]
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
    // SAT for 2D need todo
    return false;
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
#ifdef MOLLER_ALG

struct All_dist
{
    precision_t len_A, len_B, len_C;
};
enum who_is_zero  // who lie on line of intersection of planes //
{
    NO_ZERO = 0x00000000,

    A_LIE = 0x00000001,
    B_LIE = 0x00000010,
    C_LIE = 0x00000100,

    AB_LIE = 0x00000011,
    AC_LIE = 0x00000101,
    BC_LIE = 0x00000110,

    // ABC_LIE = 0x00000111,  // is no exist //
};

constexpr bool complete_intersection_triangls (const Triangl<3>& tr_1, const Triangl<3>& tr_2) noexcept
{
    All_dist all_dist_for_tr1{};
    All_dist all_dist_for_tr2{};

    // points lie on the same side of the line of intersection of the planes
    std::pair< bool, int > is_same_side_1 = check_sign_of_len (tr_1, tr_2.get_plane(), all_dist_for_tr1);
    std::pair< bool, int > is_same_side_2 = check_sign_of_len (tr_2, tr_1.get_plane(), all_dist_for_tr2);

    if (is_same_side_1.first) return false;
    if (is_same_side_2.first) return false;
    //_______________________________________//


    // check zero lens situations //

    //_______________________________________//


    // get points on line of triangles
    std::pair< Point<3>, Point<3> > two_p_tr1_on_l = get_points_on_line (all_dist_for_tr1, tr_1);
    std::pair< Point<3>, Point<3> > two_p_tr2_on_l = get_points_on_line (all_dist_for_tr2, tr_2);
    //_______________________________________//
    

    // check intersection of 2 segments on line
    return check_four_p_intersection (two_p_tr1_on_l.first, two_p_tr1_on_l.second, two_p_tr2_on_l.first, two_p_tr2_on_l.second);
}

constexpr std::pair< bool, int > check_sign_of_len (const Triangl<3>& tr, const Plane3D& plane, All_dist& all_dis) noexcept // return true if all len from points of tr have one sign
{
    all_dis.len_A = plane.distance(tr.get_A());
    all_dis.len_B = plane.distance(tr.get_B());
    all_dis.len_C = plane.distance(tr.get_C());

    //______________DO_ZERO__________________//
    int flag = NO_ZERO;
    // A = 0 //
    if (is_nearly_zero(all_dis.len_A))
    {
        all_dis.len_A = 0;
        flag |= A_LIE;

        if (is_nearly_zero(all_dis.len_B))  // A = 0 && B = 0 //
        {
            all_dis.len_B = 0;
            flag |= B_LIE;

            return {false, flag};  // A = 0 && B = 0 && C != 0 // C cant be 0, because this is triangle after initialization
        }

        if (is_nearly_zero(all_dis.len_C))  // A = 0 && B != 0 && C = 0 //
        {
            flag |= C_LIE;
            all_dis.len_C = 0;
        }

        return {false, flag};  // A = 0 && B != 0 && C != 0 (+ prev if)//
    }

    //  A != 0  //
    if (is_nearly_zero(all_dis.len_B))  // A != 0 && B = 0 //
    {
        all_dis.len_B = 0;
        flag |= B_LIE;

        if (is_nearly_zero(all_dis.len_C))  // A != 0 && B = 0 && C = 0//
        {
            all_dis.len_C = 0;
            flag |= C_LIE;
        }

        return {false, flag};    // A != 0 && B = 0 && C != 0 (+prev if) //
    }

    //  A !=0 && B != 0  //
    if (is_nearly_zero(all_dis.len_C))
    {
        all_dis.len_C = 0;
        flag |= C_LIE;

        return {false, flag};  //  A !=0 && B != 0 && C = 0 //
    }
    //_______________________________________//

    //  A !=0 && B != 0 && C != 0  //
    if (all_dis.len_A > 0)
    {
        if (all_dis.len_B > 0)
        {
            if (all_dis.len_C > 0)
                return {true, NO_ZERO};
            
            return {false, NO_ZERO};
        }
        return {false, NO_ZERO};
    }

    // if (all_dis.len_A < 0)
    if (all_dis.len_B < 0)
    {
        if (all_dis.len_C < 0)
            return {true, NO_ZERO};
        
        return {false, NO_ZERO};
    }

    return {false, NO_ZERO};
}

#define RET_W_SOLO_C return {get_p_out_formula (tr.get_C(), all_dist.len_C, all_dist.len_A, tr.get_A()),\
                             get_p_out_formula (tr.get_C(), all_dist.len_C, all_dist.len_B, tr.get_B()) };

#define RET_W_SOLO_B return {get_p_out_formula (tr.get_B(), all_dist.len_B, all_dist.len_A, tr.get_A()),\
                             get_p_out_formula (tr.get_B(), all_dist.len_B, all_dist.len_C, tr.get_C()) };

#define RET_W_SOLO_A return {get_p_out_formula (tr.get_A(), all_dist.len_A, all_dist.len_B, tr.get_B()),\
                             get_p_out_formula (tr.get_A(), all_dist.len_A, all_dist.len_C, tr.get_C()) };

constexpr std::pair< Point<3>, Point<3> > get_points_on_line (All_dist& all_dist, const Triangl<3>& tr) noexcept
{
    // find point that lies solo on one side of the plane
    if ((all_dist.len_A - EPSILON_OF_ZERO) < 0)
    {
        if ((all_dist.len_B - EPSILON_OF_ZERO) < 0)
            RET_W_SOLO_C;
        
        if ((all_dist.len_C - EPSILON_OF_ZERO) < 0)
            RET_W_SOLO_B;

        RET_W_SOLO_A;
    }

    // if (all_dist.len_A > 0)
    if ((all_dist.len_B - EPSILON_OF_ZERO) > 0)
        RET_W_SOLO_C;
        
    if ((all_dist.len_C - EPSILON_OF_ZERO) > 0)
        RET_W_SOLO_B;

    RET_W_SOLO_A;
}

constexpr Point<3> get_p_out_formula (const Point<3>& p_A, const precision_t d0, const precision_t d1, const Point<3>& p_B)
{
    return p_A + (d0 / (d0 - d1)) * (p_B - p_A);
}

constexpr bool check_four_p_intersection (const Point<3>& Ap_1, const Point<3>& Ap_2, const Point<3>& Bp_1, const Point<3>& Bp_2) noexcept
{
    if ( is_nearly_zero (Ap_1.z() - Ap_2.z()) )  [[unlikely]]
    {
        if ( is_nearly_zero (Ap_1.y() - Ap_2.y()) )  [[unlikely]]
        {
            if ( is_nearly_zero (Ap_1.x() - Ap_2.x()) )  [[unlikely]]  // point is one
            {
                precision_t B_max = std::max(Bp_1.x(), Bp_2.x());
                precision_t B_min = std::min(Bp_1.x(), Bp_2.x());
                return ((Ap_1.x() - B_min - EPSILON_OF_ZERO) > 0) && ((Ap_1.x() < B_max - Ap_1.x() - EPSILON_OF_ZERO) > 0);
            }
            return cmp_four_points_on_one_axe (Ap_1.x(), Ap_2.x(), Bp_1.x(), Bp_2.x());  // can cmp on Y axe
        }
        return cmp_four_points_on_one_axe (Ap_1.y(), Ap_2.y(), Bp_1.y(), Bp_2.y());  // can cmp on Y axe
    }
    return cmp_four_points_on_one_axe (Ap_1.z(), Ap_2.z(), Bp_1.z(), Bp_2.z());  // can cmp on Z axe
}

constexpr bool cmp_four_points_on_one_axe (const precision_t& Ap_1, const precision_t& Ap_2, 
                                           const precision_t& Bp_1, const precision_t& Bp_2) noexcept  // return true if min < max
{
    const precision_t A_min = std::min(Ap_1, Ap_2);
    const precision_t A_max = std::max(Ap_1, Ap_2);

    const precision_t B_min = std::min(Bp_1, Bp_2);
    const precision_t B_max = std::max(Bp_1, Bp_2);

    return ((A_max - B_min - EPSILON_OF_ZERO) > 0) && ((B_max - A_min - EPSILON_OF_ZERO) > 0);
}
#endif  //  MOLLER_ALG

#ifdef SUB_OF_MINKOVSKI
constexpr bool complete_intersection_triangls (const Triangl<3>& tr_1, const Triangl<3>& tr_2) noexcept
{





}
#endif
//_________________________________________________________________________________________________________________________________________//
