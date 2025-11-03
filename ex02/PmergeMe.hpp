/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasano <sasano@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 08:56:07 by sasano            #+#    #+#             */
/*   Updated: 2025/11/02 17:42:35 by sasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <utility> // std::pair
#include <algorithm>

class PmergeMe
{
public:
    typedef std::pair<unsigned int, unsigned int> Pair; // small, big
private:
    // ヤコブ順列の構築（挿入順序決定用）
    static std::vector<size_t> buildJacobOrder(size_t pairCount);

    // vector 用
    static void makePairsVec(const std::vector<unsigned int> &input, std::vector<Pair> &pairs, bool &hasStraggler, unsigned int &straggler);
    static bool cmpBigAscVec(const PmergeMe::Pair &a, const PmergeMe::Pair &b);
    static void sortPairsByBigVec(std::vector<Pair> &pairs); // bigで昇順
    static void mergeInsertVec(const std::vector<Pair> &pairs, std::vector<unsigned int> &sorted);

    // deque 用
    static void makePairsDeq(const std::deque<unsigned int> &input, std::deque<Pair> &pairs, bool &hasStraggler, unsigned int &straggler);
    static bool cmpBigAscDeq(const PmergeMe::Pair &a, const PmergeMe::Pair &b);
    static void sortPairsByBigDeq(std::deque<Pair> &pairs); // bigで昇順
    static void mergeInsertDeq(const std::deque<Pair> &pairs, std::deque<unsigned int> &sorted);

public:
    PmergeMe();
    ~PmergeMe();
    PmergeMe(const PmergeMe &copy);
    PmergeMe &operator=(const PmergeMe &copy);

    // 入力: 正の整数列（未ソート）
    // 出力: 昇順ソート済み vector
    static std::vector<unsigned int> sortVec(const std::vector<unsigned int> &in);
    static std::deque<unsigned int> sortDeq(const std::deque<unsigned int> &in);
};

#endif