#include "../../xlns16.cpp"
#include "../../xlns32.cpp"
#include <iostream>

int main() {
    // XLNS16 example
    xlns16_float a16(3.14f);
    xlns16_float b16(2.71f);
    xlns16_float c16 = a16 * b16;
    
    std::cout << "XLNS16: " << a16.getfloat() << " * " 
              << b16.getfloat() << " = " << c16.getfloat() << std::endl;
    
    // XLNS32 example
    xlns32_float a32(3.14);
    xlns32_float b32(2.71);
    xlns32_float c32 = a32 * b32;
    
    std::cout << "XLNS32: " << a32.getdouble() << " * " 
              << b32.getdouble() << " = " << c32.getdouble() << std::endl;
    
    return 0;
}