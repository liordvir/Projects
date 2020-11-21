#include <iostream>
#include "Gcalc.h"
enum Numbers {ONE = 1, TWO = 2, THREE = 3};


using std::cout;
using std::cin;
using std::endl;
using namespace final;

int main(int argc, char* argv[]) {
    if(argc == ONE) {
        gcalc_user();
    } else if (argc == THREE) {
        batch(argv[ONE], argv[TWO]);
    }
    else {
        std::cerr << "Error: Must provide 2 or no files";
    }
    return 0;
}
