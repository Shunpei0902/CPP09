/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasano <shunkotkg0141@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 09:08:58 by sasano            #+#    #+#             */
/*   Updated: 2025/10/06 01:05:00 by sasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include <deque>
#include <string>
#include <climits>
#include <cstdlib>
#include <cctype>
#include <sys/time.h>
#include "PmergeMe.hpp"

static bool parsePositiveUInt(const std::string &str, unsigned int &value)
{
    // 正の整数のみ（先頭+記号なし、0は禁止）
    if (str.empty() || str[0] == '0')
        return false;
    const unsigned char *p = (const unsigned char *)str.c_str();
    // 全て数字かチェック
    for (const unsigned char *q = p; *q; ++q)
    {
        if (!std::isdigit(*q))
            return false;
    }
    // 変換して範囲内かチェック
    unsigned long val = std::strtoul(str.c_str(), NULL, 10);
    if (val > UINT_MAX)
        return false;
    value = static_cast<unsigned int>(val);
    return true;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    std::vector<unsigned int> vecInput;
    std::deque<unsigned int> deqInput;

    // 入力の検証と格納
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        unsigned int value;
        if (!parsePositiveUInt(arg, value))
        {
            std::cerr << "Error'" << arg << "'" << std::endl;
            return 1;
        }

        vecInput.push_back(static_cast<unsigned int>(value));
        deqInput.push_back(static_cast<unsigned int>(value));
    }

    // ソート前の表示
    std::cout << "Before: ";
    for (size_t i = 0; i < vecInput.size(); ++i)
    {
        std::cout << vecInput[i] << (i + 1 == vecInput.size() ? "" : " ");
    }
    std::cout << std::endl;

    // vector のソートと時間計測
    struct timeval start, end;
    gettimeofday(&start, NULL);
    std::vector<unsigned int> sortedVec = PmergeMe::sortVec(vecInput);
    gettimeofday(&end, NULL);
    long vecTime = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    // deque のソートと時間計測
    gettimeofday(&start, NULL);
    std::deque<unsigned int> sortedDeq = PmergeMe::sortDeq(deqInput);
    gettimeofday(&end, NULL);
    long deqTime = (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_usec - start.tv_usec);

    // ソート後の表示
    std::cout << "After:  ";
    for (size_t i = 0; i < sortedVec.size(); ++i)
    {
        std::cout << sortedVec[i] << (i + 1 == sortedVec.size() ? "" : " ");
    }
    std::cout << std::endl;

    // 処理時間の表示
    std::cout << "Time to process a range of " << vecInput.size() << " elements with std::vector : " << vecTime << " us" << std::endl;
    std::cout << "Time to process a range of " << deqInput.size() << " elements with std::deque  : " << deqTime << " us" << std::endl;
    return 0;
}