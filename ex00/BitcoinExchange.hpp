/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasano <shunkotkg0141@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:11:03 by sasano            #+#    #+#             */
/*   Updated: 2025/10/04 21:17:55 by sasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange
{
private:
    std::map<std::string, float> _rates; // 日付と価格のマップ

    static bool isLeap(int year);
    static int daysInMonth(int year, int month);

public:
    BitcoinExchange();
    ~BitcoinExchange();

    // data.csv を読み込んで _rates を初期化
    void loadDatabase(const std::string &csvPath);

    // 指定された日付の価格を取得、存在しない場合は最も近い過去の日付の価格を返す
    bool findRateOnOrBefore(const std::string &date, float &rate) const;

    // 入力チェック
    static bool isValidDate(const std::string &input);
    static bool parseValue(const std::string &input, float &value); // 文字列を float に変換
    static std::string trim(const std::string &str);
};

#endif