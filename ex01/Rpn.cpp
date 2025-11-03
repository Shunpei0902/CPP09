/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Rpn.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasano <sasano@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 08:13:42 by sasano            #+#    #+#             */
/*   Updated: 2025/11/02 18:35:45 by sasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Rpn.hpp"

static bool isDigitToken(const std::string &token)
{
    return token.size() == 1 && std::isdigit(static_cast<unsigned char>(token[0]));
}

static bool isOperatorToken(const std::string &token)
{
    return token.size() == 1 && std::strchr("+-*/", token[0]) != NULL;
}

bool Rpn::evaluate(const std::string &expression, long &out)
{
    std::istringstream iss(expression);
    std::string token;
    std::stack<long> stack;

    while (iss >> token)
    {
        if (isDigitToken(token))
        {
            stack.push(token[0] - '0'); // 文字を数値に変換してプッシュ
        }
        else if (isOperatorToken(token))
        {
            if (stack.size() < 2)
                return false; // オペランドが足りない

            long b = stack.top();
            stack.pop();
            long a = stack.top();
            stack.pop();
            long result = 0;

            switch (token[0])
            {
            case '+':
                result = a + b;
                break;
            case '-':
                result = a - b;
                break;
            case '*':
                result = a * b;
                break;
            case '/':
                if (b == 0)
                    return false; // ゼロ除算エラー
                result = a / b;
                break;
            default:
                return false; // 不明な演算子
            }
            stack.push(result);
        }
        else
        {
            return false; // 不明なトークン
        }
    }
    if (stack.size() != 1)
        return false; // 最終的にスタックに 1 つの結果がない
    out = stack.top();

    return true;
}