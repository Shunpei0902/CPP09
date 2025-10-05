/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sasano <shunkotkg0141@gmail.com>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/05 08:22:37 by sasano            #+#    #+#             */
/*   Updated: 2025/10/05 08:38:26 by sasano           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Rpn.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <RPN expression>" << std::endl;
        return 1;
    }

    long long result;
    if (Rpn::evaluate(argv[1], result))
    {
        std::cout << result << std::endl;
        return 0;
    }
    else
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
}