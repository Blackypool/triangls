#include "Header.h"
// import geometry;
#include "geometry/point_v.cppm"
#include "geometry/areas.cppm"


//________________________________________________Questions_Tasks__________________________________________________________________________//
// need do define on 1/2/3 -> 1D/2D/3D ??
// как хранить кто с кем пересекается?
// в виде графа мейби??? как рукопажатия?
// инициализировать пересечения до или при пролете и просмотре?
// типо увидел 2пи пространство - посмотрел какие треугольники там есть и проверяешь в моменте пересекаютс ли???
//_________________________________________________________________________________________________________________________________________//


int main ()
{
    return 0;
}

template <std::size_t Dimension>
constexpr bool compare_tr (const Triangl<Dimension>& tr_1, const Triangl<Dimension>& tr_2) noexcept requires (Dimension == 3)
{
    // чек AABB
    // чек параллельные плоскости (они менее вероятны как будто бы но и проверить надо)
    // при совпадении плосокстей полность вызываем функцию для 2д чека
    // меллер
}
//_________________________________________________________________________________________________________________________________________//
// из расстояний находим через отношения точки которые лежат на прямой пересечения

// рассматриваем 2д случай

template <std::size_t Dimension>
constexpr bool Moller_alg (const Triangl<Dimension>& tr_1, const Triangl<Dimension>& tr_2) noexcept requires (Dimension == 3)
{
    // check parallel planes!!! -> check совпадают ли плоскости полностью == compare_planes

    // points lie on the same side of the line of intersection of the planes
    if (check_sign_of_len (tr_1, tr_2.get_plane())) return false;
    if (check_sign_of_len (tr_2, tr_1.get_plane())) return false;
    
    Vector<3> normal_1 = tr_1.normal();
    Vector<3> normal_2 = tr_2.normal();

    Vector<3> line_intersection = normal_1 ^ normal_2;

    

    
}

// возвращать пару: структуру с длинами и булево
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

    return false
}
