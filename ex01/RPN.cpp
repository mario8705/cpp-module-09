#include "RPN.hpp"
#include <iostream>

RPN::RPN()
{
}

RPN::~RPN()
{
}

bool RPN::EvaluateExpression(const std::string &expr)
{
    std::string::const_iterator it = expr.begin();

    while (it != expr.end())
    {
        if (std::isdigit(*it))
        {
            m_stack.push(static_cast<double>(*it - '0'));
        }
        else
        {
            if (*it != '+' && *it != '-' && *it != '*' && *it != '/')
            {
                std::cout << "Unknown operator: " << *it << std::endl;
                return false;
            }

            if (m_stack.size() < 2)
            {
                std::cout << "Not enough operands" << std::endl;
                return false;
            }

            double rhs = m_stack.top();
            m_stack.pop();
            double lhs = m_stack.top();
            m_stack.pop();

            if (*it == '/' && rhs == 0)
            {
                std::cout << "Division by zero" << std::endl;
                return false;
            }

            switch (*it)
            {
            case '+':
                m_stack.push(lhs + rhs);
                break;
            case '-':
                m_stack.push(lhs - rhs);
                break;
            case '*':
                m_stack.push(lhs * rhs);
                break;
            case '/':
                m_stack.push(lhs / rhs);
                break;
            default:
                break;
            }
        }
        ++it;
    }
    return true;
}

void RPN::PrintResult(std::ostream &out)
{
    if (m_stack.empty())
    {
        std::cout << "No result to print" << std::endl;
        return;
    }
    if (m_stack.size() > 1)
    {
        std::cout << "More than one result in the stack, printing the last one" << std::endl;
    }
    out << m_stack.top() << std::endl;
}
