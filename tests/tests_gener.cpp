#include "../Header.h"

// g++ -std=c++23 tests_gener.cpp -o app
// ./app triangles.csv Cooked_tests.cpp

void prolog_tests(std::ofstream& cooked_tests, std::size_t num_triangles);

int main (int argc, char* argv[])
{
    Th_row(argc != 3, std::invalid_argument, syntax_err, "need name of files {w input tests || w ready tests}\n");

    std::ifstream Uncooked_tests(argv[1]);
    Th_row(!Uncooked_tests.is_open(), std::runtime_error, file_errorr, "problems with openning Uncooked_tests file\n");

    std::ofstream Cooked_tests(argv[2]);
    Th_row(!Cooked_tests.is_open(), std::runtime_error, file_errorr, "problems with openning Coooked_tests file\n");
    
    //________________________________________________________________
    precision_t A_X{}, A_Y{}, A_Z{};
    precision_t B_X{}, B_Y{}, B_Z{};
    precision_t C_X{}, C_Y{}, C_Z{};

    std::size_t num_triangles{};
    Uncooked_tests >> num_triangles;
    //________________________________________________________________

    //  Prolog  //
    prolog_tests(Cooked_tests, num_triangles);

    //  Body  //______________________________________________________
    char comma{};
    for (std::size_t i = 0; i < num_triangles; i++)
    {
        Uncooked_tests  >> A_X >> comma >> A_Y >> comma >> A_Z >> comma
                        >> B_X >> comma >> B_Y >> comma >> B_Z >> comma
                        >> C_X >> comma >> C_Y >> comma >> C_Z;

        Cooked_tests << "    tr_s[" << i << "] = {" << A_X << "f, " << A_Y << "f, " << A_Z << "f}, " 
                                                "{" << B_X << "f, " << B_Y << "f, " << B_Z << "f}, "  
                                                "{" << C_X << "f, " << C_Y << "f, " << C_Z << "f}};\n";
    }
    //________________________________________________________________

    //  Epilog  //
    Cooked_tests << "\n//_____________________________________________________________________\n\nreturn tr_s;\n}";
    return 0;
}

void prolog_tests(std::ofstream& cooked_tests, std::size_t num_triangles)  // R"()";
{
    cooked_tests << R"(
#include "Header.h"

import geometry;

//________________________________________________________________
static constexpr std::size_t NUM_TRIANGLES = )" << num_triangles << R"(;

class All_Triangels
{
    private:
    std::array<Triangl<3>, NUM_TRIANGLES> all_triangls_{};

    public:
    //________________________________________________________________
        constexpr All_Triangels() noexcept = default;
    //________________________________________________________________

        constexpr Triangl<3>&       operator[](std::size_t i)       noexcept { return all_triangls_[i]; }
        constexpr const Triangl<3>& operator[](std::size_t i) const noexcept { return all_triangls_[i]; }

        constexpr std::size_t size() const noexcept { return all_triangls_.size(); }
};
constexpr All_Triangels run_generated_tests ()
{
    All_Triangels tr_s{};

    //_____________________________________________________________________
    )";
}