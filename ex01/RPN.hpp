#ifndef RPN_H
#define RPN_H
#include <string>
#include <stack>

class RPN
{
public:
    RPN();
    ~RPN();

    bool EvaluateExpression(const std::string &expr);
    void PrintResult(std::ostream &out);

private:
    std::stack<double> m_stack;
};

#endif // RPN_H

