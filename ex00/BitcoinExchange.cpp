/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasano <sasano@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:10:54 by sasano            #+#    #+#             */
/*   Updated: 2025/11/02 17:29:04 by sasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

#include <fstream>   // std::ifstream
#include <sstream>   // std::istringstream
#include <stdexcept> // std::runtime_error
#include <cctype>    // std::isspace
#include <cstdlib>   // std::strtof

BitcoinExchange::BitcoinExchange() : _rates()
{
}

BitcoinExchange::~BitcoinExchange() {}

// 文字列の前後の空白をトリム
std::string BitcoinExchange::trim(const std::string &s)
{
    const std::string ws = " \t\r\n\f\v";
    std::string::size_type b = s.find_first_not_of(ws);
    if (b == std::string::npos)
        return "";
    std::string::size_type e = s.find_last_not_of(ws);
    return s.substr(b, e - b + 1);
}

// うるう年の判定
bool BitcoinExchange::isLeap(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// 月の日数を返す
int BitcoinExchange::daysInMonth(int year, int month)
{
    static const int daysPerMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && isLeap(year))
        return 29;
    return daysPerMonth[month - 1];
}

// 入力日付の妥当性チェック
bool BitcoinExchange::isValidDate(const std::string &input)
{
    // "YYYY-MM-DD" 長さ10、ハイフン位置固定、他は数字
    // フォーマット YYYY-MM-DD のみ許可
    if (input.size() != 10 || input[4] != '-' || input[7] != '-')
        return false;

    int year, month, day;
    try
    {
        year = std::strtol(input.substr(0, 4).c_str(), NULL, 10);
        month = std::strtol(input.substr(5, 2).c_str(), NULL, 10);
        day = std::strtol(input.substr(8, 2).c_str(), NULL, 10);
    }
    catch (const std::exception &)
    {
        return false; // 数字変換に失敗
    }

    if (month < 1 || month > 12)
        return false;
    int dim = daysInMonth(year, month);
    if (day < 1 || day > dim)
        return false;
    return true;
}

// 文字列を float に変換、成功すれば true、失敗すれば false を返す 変換した値は value に格納
bool BitcoinExchange::parseValue(const std::string &input, float &value)
{
    std::string s = trim(input);
    if (s.empty())
        return false;
    // "+" は許容、"-" はエラーにしたいが解析自体は行って true/false で返す
    std::istringstream iss(s);
    float v;
    iss >> v;
    if (iss.fail() || !iss.eof())
        return false; // 変換失敗または余分な文字がある
    value = v;
    return true;
}

// CSV ファイルを読み込んで _rates を初期化
void BitcoinExchange::loadDatabase(const std::string &csvPath)
{
    std::ifstream file(csvPath.c_str());
    if (!file || !file.is_open())
    {
        throw std::runtime_error("Could not open file: " + csvPath);
    }

    std::string line;
    bool isFirstLine = true;

    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        if (isFirstLine)
        {
            isFirstLine = false;
            if (line == "date,exchange_rate")
                continue; // ヘッダ行をスキップ
            throw std::runtime_error("Missing or invalid header line");
        }

        // CSV の各行をパース "YYYY-MM-DD,rate"
        std::string::size_type comma = line.find(',');
        if (comma == std::string::npos)
        {
            throw std::runtime_error("Invalid line format: " + line);
        }

        std::istringstream ss(line);
        std::string date, rateStr;

        if (!std::getline(ss, date, ',') || !std::getline(ss, rateStr))
        {
            throw std::runtime_error("Invalid line format: " + line);
        }

        date = trim(date);
        rateStr = trim(rateStr);

        if (!isValidDate(date))
        {
            throw std::runtime_error("Invalid date format: " + date);
        }

        std::istringstream rateStream(rateStr);
        float rate;
        if (!(rateStream >> rate) || !rateStream.eof())
        {
            throw std::runtime_error("Invalid rate value: " + rateStr);
        }
        _rates[date] = rate; // マップに格納 (同日がある場合は上書きされる)
    }
}

bool BitcoinExchange::findRateOnOrBefore(const std::string &date, float &rate) const
{
    // 指定された日付の価格を取得、存在しない場合は最も近い過去の日付の価格を返す
    // 存在しない場合は例外を投げる
    if (_rates.empty())
    {
        return false; // データベースが空
    }
    std::map<std::string, float>::const_iterator it = _rates.lower_bound(date);
    if (it != _rates.end() && it->first == date)
    {
        rate = it->second; // 指定日が存在する場合
        return true;
    }
    if (it == _rates.begin())
    {
        return false; // 指定日より前の日付がない
    }
    --it; // 最も近い過去の日付に移動
    rate = it->second;
    return true;
}