#include "../Header.h"
#define KEY_WORD "vertex"
// precision_t = 11 after point

// g++ -std=c++23 stl_to_csv.cpp -o a.out
// ./a.out Grogu.stl triangles.csv

int main(int argc, char* argv[])
{
    Th_row(argc != 3, std::invalid_argument, syntax_err, "need name of files {w input tests || w ready tests}\n");

    std::ifstream Stl_f(argv[1]);
    Th_row(!Stl_f.is_open(), std::runtime_error, file_errorr, "problems with openning Stl_f file\n");

    std::ofstream Csv_f(argv[2]);
    Th_row(!Csv_f.is_open(), std::runtime_error, file_errorr, "problems with openning Csv_f file\n");
    
    precision_t _X{}, _Y{}, _Z{};    
    std::size_t num_tr = 0;
    std::size_t counter = 0;  // 1/2/3 //
    std::string token{};
    
    Csv_f << std::format("{:011}\n", num_tr);  // for future rewrite
    while (Stl_f >> token)
    {
        if (token != KEY_WORD)
            continue;

        Stl_f >> _X >> _Y >> _Z;
        Csv_f << std::format("{:.11f},{:.11f},{:.11f}", _X, _Y, _Z);
        // Csv_f << _X << ',' << _Y << ',' << _Z;
        counter++;

        if (counter == 3)
        {
            Csv_f << '\n';
            counter = 0;
            num_tr++;
        }
        else Csv_f << ',';
    }
    Csv_f.seekp(0, std::ios::beg);  //  update from start of file  //  beg/cur/end -- where start offset
    Csv_f << std::format("{:011}", num_tr);

    return 0;
}
