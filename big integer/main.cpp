#include <iostream>
#include <stdexcept>

#include "big_integer.h"
#include "equation_solver.h"

using equation_solver::big_num_arithmetic::GenerateEquation;

enum OperationType {
    kInt64Cast,
    kInc,
    kDec,
    kPlus,
    kMinus,
    kMult,
    kDiv,
    kEq, kNe,
    kLt, kLe,
    kGt, kGe,
    kMod,
    kEquationSolve,
    kUnknownOperation
};

OperationType GetOperationType(const std::string &str) {
    if (str == "int64_t") return kInt64Cast;
    if (str == "++")      return kInc;
    if (str == "--")      return kDec;
    if (str == "+")       return kPlus;
    if (str == "-")       return kMinus;
    if (str == "*")       return kMult;
    if (str == "/")       return kDiv;
    if (str == "==")      return kEq;
    if (str == "!=")      return kNe;
    if (str == "<")       return kLt;
    if (str == "<=")      return kLe;
    if (str == ">")       return kGt;
    if (str == ">=")      return kGe;
    if (str == "%")       return kMod;
    if (str == "equation_solve") return kEquationSolve;

    return kUnknownOperation;
}

int main() {
    BigInteger::SetInternalBase(4096);
    std::string operation;

    std::cout << "Input operation : " << std::endl;
    std::cin >> operation;

    OperationType op = GetOperationType(operation);
    std::string arg1, arg2, arg3;

    try {
        if (op == kUnknownOperation) {
            std::cout << "Unknown operation\n";
        } else if (op < kPlus) {
            std::cout << "Input value to modify : ";
            std::cin >> arg1;
            BigInteger a = BigInteger::FromString(arg1, 10);
            std::cout << "Result : ";
            switch (op) {
                case kInt64Cast:
                    std::cout << int64_t(a) << "\n";
                    break;
                case kInc:
                    a++;
                    std::cout << a.ToString(10) << std::endl;
                    break;
                case kDec:
                    a--;
                    std::cout << a.ToString(10) << std::endl;
                    break;
            }
        } else if (kPlus <= op && op <= kMod) {
            std::cout << "Input left value : ";
            std::cin >> arg1;
            std::cout << "Input right value : ";
            std::cin >> arg2;

            BigInteger l = BigInteger::FromString(arg1, 10);
            BigInteger r = BigInteger::FromString(arg2, 10);
            std::cout << "Result : ";
            switch (op) {
                case kPlus :
                    std::cout << (l += r).ToString(10) << "\n";
                    break;
                case kMinus :
                    std::cout << (l -= r).ToString(10) << "\n";
                    break;
                case kMult :
                    std::cout << (l *= r).ToString(10) << "\n";
                    break;
                case kDiv :
                    std::cout << (l /= r).ToString(10) << "\n";
                    break;
                case kEq :
                    std::cout << std::boolalpha << (l == r) << "\n";
                    break;
                case kNe :
                    std::cout << std::boolalpha << (l != r) << "\n";
                    break;
                case kLe :
                    std::cout << std::boolalpha << (l <= r) << "\n";
                    break;
                case kLt :
                    std::cout << std::boolalpha << (l < r) << "\n";
                    break;
                case kGe :
                    std::cout << std::boolalpha << (l >= r) << "\n";
                    break;
                case kGt :
                    std::cout << std::boolalpha << (l > r) << "\n";
                    break;
                case kMod:
                    uint32_t mod = l % uint32_t(int64_t(r));
                    std::cout << mod << "\n";
                    break;
            }
        } else {
            std::cout << "Input a koef : ";
            std::cin >> arg1;
            std::cout << "Input x1 root : ";
            std::cin >> arg2;
            std::cout << "Input x2 root : ";
            std::cin >> arg3;

            BigInteger a = BigInteger::FromString(arg1, 10);
            BigInteger x1 = BigInteger::FromString(arg2, 10);
            BigInteger x2 = BigInteger::FromString(arg3, 10);

            equation_solver::QuadraticEquation eq =
                    GenerateEquation(a, x1, x2);

            std::cout << std::boolalpha
                      << equation_solver::Solve(eq, x1, x2) << "\n";
        }
    } catch (big_num_arithmetic::DivisionByZeroError) {
        std::cout << "Division by zero error" << std::endl;
    } catch (std::logic_error &e) {
        std::cout << e.what() << std::endl;
    } catch (std::runtime_error &e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "Undefined behaviour\n";
    }
    return 0;
}
