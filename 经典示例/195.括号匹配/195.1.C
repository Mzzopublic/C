#include <iostream>
#include <string>
#include <stack>
#include <stdexcept>

const int MaxLength = 100;  // 最大的字符串长度

// 括号匹配函数
void PrintMatchedPairs(const std::string& expr)
{
    std::stack<int> s;  // 用于存储左括号的索引
    for (size_t i = 0; i < expr.length(); ++i) {
        if (expr[i] == '(') {
            s.push(i);  // 遇到左括号，将其索引压入栈
        } else if (expr[i] == ')') {
            if (s.empty()) {
                std::cout << "没有对应第" << (i + 1) << "个右括号的左括号" << std::endl;
            } else {
                int j = s.top();
                s.pop();
                std::cout << (i + 1) << "\t" << (j + 1) << std::endl;  // 打印匹配的括号对
            }
        }
    }

    // 堆栈中所剩下的(都是未匹配的
    while (!s.empty()) {
        int j = s.top();
        s.pop();
        std::cout << "没有对应第" << (j + 1) << "个左括号的右括号" << std::endl;
    }
}

// 主函数
int main()
{
    try {
        std::string expr;
        std::cout << "请输入符号个数小于" << MaxLength << "的表达式：" << std::endl;
        std::getline(std::cin, expr);

        if (expr.length() > MaxLength) {
            throw std::runtime_error("输入的表达式超过了最大长度限制");
        }

        std::cout << "括号对是：" << std::endl;
        PrintMatchedPairs(expr);
    } catch (const std::exception& e) {
        std::cerr << "发生错误: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}