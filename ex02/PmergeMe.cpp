/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasano <sasano@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 09:52:45 by sasano            #+#    #+#             */
/*   Updated: 2025/11/03 13:00:40 by sasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>

PmergeMe::PmergeMe() {}

PmergeMe::~PmergeMe() {}

PmergeMe::PmergeMe(const PmergeMe &copy) { *this = copy; }

PmergeMe &PmergeMe::operator=(const PmergeMe &copy)
{
    if (this != &copy)
    {
    }; // 自己代入チェックはしてるけど中身が空 *this を返してチェーン代入はできる コピーできないようにしている
    return (*this);
}

// -----------------------------------------共通部分-------------------------------------------
// ヤコブ順列の構築（挿入順序決定用）
std::vector<size_t> PmergeMe::buildJacobOrder(size_t pairCount)
{
    std::vector<size_t> order; // 挿入順序表
    if (pairCount <= 1)
        return order; // 空または1要素なら空を返す smallのs0は別処理

    // Jacobsthal 数列:jacobNumbers = 1,3,5,11,21,... (< m) を作る
    std::vector<size_t> jacobNumbers;
    jacobNumbers.push_back(1); // J(1)
    size_t a = 1;              // J(1)=1, J(2)=1 とみなして J(n)=J(n-1)+2*J(n-2)
    size_t b = 1;

    while (true)
    {
        size_t next = a + 2 * b;
        if (next >= pairCount)
            break;
        jacobNumbers.push_back(next);
        b = a;
        a = next;
    }

    // 区間 [prev..J(k)] を降順で並べて順番表（order）に追加　→ 1,3,2,5,4,7,6,... の挿入順
    size_t prev = 1;
    for (size_t i = 0; i < jacobNumbers.size(); ++i)
    {
        size_t cur = jacobNumbers[i]; // cur >= prev
        if (cur >= pairCount)         // 念のため
            cur = pairCount - 1;
        size_t jnum = cur; // jnum: cur から prev まで降順にorderに追加
        while (jnum >= prev)
        {
            order.push_back(jnum);
            if (jnum == 0)
                break;
            --jnum;
        }
        prev = cur + 1;
        if (prev > pairCount - 1)
            break;
    }

    // 余った残りの範囲を降順でorderに追加
    if (prev <= pairCount - 1)
    {
        std::size_t k = pairCount - 1;
        while (true)
        {
            order.push_back(k);
            if (k == prev)
                break;
            --k;
        }
    }
    return order; // 範囲は 1..m-1（0 は s0）
}

// ---------------------------------------- vector 用 -----------------------------------------

// ペアを作成
void PmergeMe::makePairsVec(const std::vector<unsigned int> &input, std::vector<PmergeMe::Pair> &pairs, bool &hasStraggler, unsigned int &straggler)
{
    size_t n = input.size();
    hasStraggler = (n % 2 != 0);
    size_t pairCount = n / 2;

    pairs.reserve(pairCount); // 事前に容量を確保

    for (size_t i = 0; i < pairCount; ++i)
    {
        unsigned int first = input[2 * i];
        unsigned int second = input[2 * i + 1];
        if (first < second)
            // pairs.emplace_back(first, second); // (small, big)
            pairs.push_back(PmergeMe::Pair(first, second));
        else
            // pairs.emplace_back(second, first);
            pairs.push_back(PmergeMe::Pair(second, first));
    }

    if (hasStraggler)
    {
        straggler = input[n - 1];
    }
}

// big での昇順比較関数
bool PmergeMe::cmpBigAscVec(const PmergeMe::Pair &a, const PmergeMe::Pair &b)
{
    if (a.second != b.second)
        return a.second < b.second; // big で昇順
    return a.first < b.first;       // big が同じなら small で昇順
}

// big でソート（安定ソート）
void PmergeMe::sortPairsByBigVec(std::vector<Pair> &pairs)
{
    std::stable_sort(pairs.begin(), pairs.end(), cmpBigAscVec);
}

// merge-insert ソート
void PmergeMe::mergeInsertVec(const std::vector<Pair> &pairs, std::vector<unsigned int> &sorted)
{
    if (pairs.empty())
        return;
    sorted.clear();
    sorted.reserve(pairs.size() * 2); // 事前に容量を確保

    // main chain: big を並べる 昇順
    for (size_t i = 0; i < pairs.size(); ++i)
    {
        sorted.push_back(pairs[i].second);
    }

    // s0をb0以下の範囲で挿入
    unsigned int s0 = pairs[0].first;
    unsigned int b0 = pairs[0].second;
    // b0 以下の範囲で二分探索して挿入位置を見つける
    std::vector<unsigned int>::iterator endBound = std::upper_bound(sorted.begin(), sorted.end(), b0);
    std::vector<unsigned int>::iterator pos = std::lower_bound(sorted.begin(), endBound, s0);
    sorted.insert(pos, s0);

    // 残りのペアの small を Jacobsthal 順に挿入
    std::vector<size_t> jacobOrder = buildJacobOrder(pairs.size());
    for (size_t idx = 0; idx < jacobOrder.size(); idx++)
    {
        size_t i = jacobOrder[idx];        // 1..m-1
        unsigned int si = pairs[i].first;  // small
        unsigned int bi = pairs[i].second; // big
        // bi 以下の範囲で二分探索して挿入位置を見つける
        endBound = std::upper_bound(sorted.begin(), sorted.end(), bi);
        pos = std::lower_bound(sorted.begin(), endBound, si);
        sorted.insert(pos, si);
    }
}

std::vector<unsigned int> PmergeMe::sortVec(const std::vector<unsigned int> &in)
{
    std::vector<Pair> pairs;

    if (in.empty()) // 入力が空なら空を返す
        return std::vector<unsigned int>();
    bool hasStraggler = false;  // 奇数個なら true
    unsigned int straggler = 0; // 余りの要素

    // ペアを作成
    makePairsVec(in, pairs, hasStraggler, straggler);

    // big でソート
    sortPairsByBigVec(pairs);

    // merge-insert ソート
    std::vector<unsigned int> merged;
    mergeInsertVec(pairs, merged);

    // 余りの要素を追加
    if (hasStraggler)
    {
        // 追加位置を二分探索で見つけて挿入
        std::vector<unsigned int>::iterator it = std::lower_bound(merged.begin(), merged.end(), straggler);
        merged.insert(it, straggler);
    }

    return merged;
}

// ---------------------------------------- deque 用 -----------------------------------------

// ペアを作成
void PmergeMe::makePairsDeq(const std::deque<unsigned int> &input, std::deque<PmergeMe::Pair> &pairs, bool &hasStraggler, unsigned int &straggler)
{
    size_t n = input.size();
    hasStraggler = (n % 2 != 0);
    size_t pairCount = n / 2;

    // pairs.reserve(pairCount); // 事前に容量を確保 C++98 では deque に reserve はない

    for (size_t i = 0; i < pairCount; ++i)
    {
        unsigned int first = input[2 * i];
        unsigned int second = input[2 * i + 1];
        if (first < second)
            // pairs.emplace_back(first, second); // (small, big)
            pairs.push_back(PmergeMe::Pair(first, second));
        else
            // pairs.emplace_back(second, first);
            pairs.push_back(PmergeMe::Pair(second, first));
    }

    if (hasStraggler)
    {
        straggler = input[n - 1];
    }
}

// big での昇順比較関数
bool PmergeMe::cmpBigAscDeq(const PmergeMe::Pair &a, const PmergeMe::Pair &b)
{
    if (a.second != b.second)
        return a.second < b.second; // big で昇順
    return a.first < b.first;       // big が同じなら small で昇順
}

// big でソート（安定ソート）
void PmergeMe::sortPairsByBigDeq(std::deque<Pair> &pairs)
{
    std::stable_sort(pairs.begin(), pairs.end(), cmpBigAscDeq);
}

// merge-insert ソート
void PmergeMe::mergeInsertDeq(const std::deque<Pair> &pairs, std::deque<unsigned int> &sorted)
{
    if (pairs.empty())
        return;
    sorted.clear();
    // sorted.reserve(pairs.size() * 2); // 事前に容量を確保 C++98 では deque に reserve はない

    // main chain: big を並べる 昇順
    for (size_t i = 0; i < pairs.size(); ++i)
    {
        sorted.push_back(pairs[i].second);
    }

    // s0をb0以下の範囲で挿入
    unsigned int s0 = pairs[0].first;
    unsigned int b0 = pairs[0].second;
    // b0 以下の範囲で二分探索して挿入位置を見つける
    std::deque<unsigned int>::iterator endBound = std::upper_bound(sorted.begin(), sorted.end(), b0);
    std::deque<unsigned int>::iterator pos = std::lower_bound(sorted.begin(), endBound, s0);
    sorted.insert(pos, s0);

    // 残りのペアの small を Jacobsthal 順に挿入
    std::vector<size_t> jacobOrder = buildJacobOrder(pairs.size());
    for (size_t idx = 0; idx < jacobOrder.size(); idx++)
    {
        size_t i = jacobOrder[idx];        // 1..m-1
        unsigned int si = pairs[i].first;  // small
        unsigned int bi = pairs[i].second; // big
        // bi 以下の範囲で二分探索して挿入位置を見つける
        endBound = std::upper_bound(sorted.begin(), sorted.end(), bi);
        pos = std::lower_bound(sorted.begin(), endBound, si);
        sorted.insert(pos, si);
    }
}

std::deque<unsigned int> PmergeMe::sortDeq(const std::deque<unsigned int> &in)
{
    std::deque<Pair> pairs;

    if (in.empty()) // 入力が空なら空を返す
        return std::deque<unsigned int>();
    bool hasStraggler = false;  // 奇数個なら true
    unsigned int straggler = 0; // 余りの要素

    // ペアを作成
    makePairsDeq(in, pairs, hasStraggler, straggler);

    // big でソート
    sortPairsByBigDeq(pairs);

    // merge-insert ソート
    std::deque<unsigned int> merged;
    mergeInsertDeq(pairs, merged);

    // 余りの要素を追加
    if (hasStraggler)
    {
        // 追加位置を二分探索で見つけて挿入
        std::deque<unsigned int>::iterator it = std::lower_bound(merged.begin(), merged.end(), straggler);
        merged.insert(it, straggler);
    }

    return merged;
}
