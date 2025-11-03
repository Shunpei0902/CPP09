/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasano <sasano@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/04 20:16:07 by sasano            #+#    #+#             */
/*   Updated: 2025/11/02 17:29:38 by sasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <fstream>
#include "BitcoinExchange.hpp"

static void processInput(const BitcoinExchange &btcEx, const std::string &inputPath)
{
    std::ifstream inputFile(inputPath.c_str());
    if (!inputFile || !inputFile.is_open())
    {
        std::cerr << "Error: Could not open input file " << inputPath << std::endl;
        return;
    }

    std::string line;
    bool isFirstLine = true;

    while (std::getline(inputFile, line))
    {
        if (line.empty())
            continue;

        if (isFirstLine)
        {
            isFirstLine = false;
            std::string header = BitcoinExchange::trim(line);
            if (line == "date | value")
                continue; // ヘッダ行をスキップ
            std::cerr << "Warning: Missing or invalid header line" << std::endl;
            return;
        }

        // 行をパース "YYYY-MM-DD | value"
        std::string::size_type bar = line.find('|');
        if (bar == std::string::npos)
        {
            std::cerr << "Error: Invalid line format: " << line << std::endl;
            continue;
        }

        // 日付と値をトリムして取得
        std::string date = BitcoinExchange::trim(line.substr(0, bar));
        std::string valueStr = BitcoinExchange::trim(line.substr(bar + 1));

        // date の妥当性チェック
        if (!BitcoinExchange::isValidDate(date))
        {
            std::cerr << "Error: Invalid date format: " << date << std::endl;
            continue;
        }

        // value の妥当性チェックと変換
        float value;
        if (!BitcoinExchange::parseValue(valueStr, value))
        {
            std::cerr << "Error: Invalid value: " << valueStr << std::endl;
            continue;
        }
        if (value < 0.0)
        {
            std::cerr << "Error: Negative value not allowed: " << value << std::endl;
            continue;
        }
        if (value > 1000.0)
        {
            std::cerr << "Error: Value exceeds maximum limit (1000): " << value << std::endl;
            continue;
        }

        // 指定日のレートを取得
        float rate;
        if (!btcEx.findRateOnOrBefore(date, rate))
        {
            std::cerr << "Error: No exchange rate available for date: " << date << std::endl;
            continue;
        }

        // レートを適用して最終的な値を計算
        float finalValue = value * rate;
        std::cout << date << " => " << value << " = " << finalValue << std::endl;
    }
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    BitcoinExchange btcEx;
    try
    {
        btcEx.loadDatabase("data.csv");
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading database: " << e.what() << std::endl;
        return 1;
    }

    processInput(btcEx, argv[1]);
    return 0;
}
