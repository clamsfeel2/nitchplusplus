#include <string>
#include <array>
struct Printer {
    struct Row { bool show; std::string icon; std::string label; std::string value; std::string color; };
    static void Print();
    static std::string PrintColors(std::array<Printer::Row, 9>& rows);
};
