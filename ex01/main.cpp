#include <iostream>
#include "RPN.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " <expression>" << std::endl;
        return 1;
    }

    RPN rpn;
    
    if (rpn.EvaluateExpression(argv[1]))
        rpn.PrintResult(std::cout);
    
    return 0;
}
