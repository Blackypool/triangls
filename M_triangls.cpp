#include "Header.h"

// нужно ли прямой знать о точках? -- да чтобы проверить какие точки брать для бесконечного пересечения
// или не надо и просто в уравнение подставить?
// добавить функцию, которая получает точку и проверяет лежит ли она на прямой

// обрабатывать параллельные oy прямые через std::nullopt

int main ()
{

    return 0;
}


// 3 points + S + 3 lines
class Triangl
{};


template <is_Decard_dim TypeCoord>
class Line2D
{
    private:
        Point<TypeCoord, 2> point_one_;  // крайние точки если они есть
        Point<TypeCoord, 2> point_two_;

        double k_;
        double b_;

        double length_;

    //_________________________________________________________________________________________________________________________________________//
    public:

    //_________________________________________________________________________________________________________________________________________//
        //  Constructor based on 2 points  //
        explicit Line2D(Point<TypeCoord, 2> p_1, Point<TypeCoord, 2> p_2) : point_one_(p_1), point_two_(p_2)
        {
            // вызов функции поиска коэфициентов из двух точек
            // search len

            std::pair coeffs = get_line_coeffs (point_one_, point_two_);
            length_ = distance (point_one_, point_two_);

            if (coeffs == std::nullopt)
            {
                // x = const
                return;
            }

            k_ = coeffs->first;
            b_ = coeffs->second;
        }

        //  Constructor based on k b coefficients  //
        explicit Line2D(double k_coef, double b_coef) : k_(k_coef), b_(b_coef)
        {
            point_one_({0, 0});
            point_two_({0, 0});

            do_zero (length_);
        }

        //  Destructor  //
        ~Line2D() = default;
    //_________________________________________________________________________________________________________________________________________//
};



//________________________________________________Questions_Tasks__________________________________________________________________________//
// func get len based on 2 points (not only for class need)
// search k and b, by 2 poinrts is also universal


// S_func (massive of points)
// поиск площади по точкам: перегрузка в зависимости от количества переданных параметров площадь считается по разному
// или площадь это сумма треугольничков которые через векторное произведение считаются

// func
// нахождение точек пересечения по двум прямым 
// if есть бесконечное пересечение, тогда берем 2 точки лежащие на обоих прямых
// (то есть берем последовательно 4 точки двух линий и проверяем лежит ли каждая на обеих прямых одновременно)


// 1 - строим все прямые отдельных треугольников y=kx+b                                                        {        ро`        }
// 2 - находим все точки пересечения разных прямых треугольников +delete лишние точки, лежащие дальше чем ро ( .------.------------.)
//                                                                                                               ро
// -> получаем массив точек: удаляем одинаковые
// отправляем точки в функцию расчета площади
//_________________________________________________________________________________________________________________________________________//


template <is_Decard_dim TypeCoord, std::size_t Dimension>
constexpr double distance (const Point<TypeCoord, Dimension>& p_1, 
                           const Point<TypeCoord, Dimension>& p_2)
{
    double sum_sq{};

    for (std::size_t i = 0; i < Dimension; ++i)
    {
        double diff = static_cast<double>(p_1[i]) - static_cast<double>(p_2[i]);
        sum_sq = sum_sq + (diff * diff);
    }

    return std::sqrt(sum_sq);
}


template <is_Decard_dim TypeCoord> // k // // b //
constexpr std::optional <std::pair<double, double>> get_line_coeffs (const Point<TypeCoord, 2>& p_1, 
                                                                     const Point<TypeCoord, 2>& p_2)
{
    double dx = static_cast<double>(p_2.x()) - static_cast<double>(p_1.x());
    double dy = static_cast<double>(p_2.y()) - static_cast<double>(p_1.y());

    if (std::abs(dx) < EPSILON_OF_ZERO)  // parallel OY
        return std::nullopt;

    double k = dy / dx;
    double b = static_cast<double>(p_1.y()) - k * static_cast<double>(p_1.x());

    return std::pair{k, b};
}
