/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Card.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ulmagner <ulmagner@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 16:22:01 by ulmagner          #+#    #+#             */
/*   Updated: 2025/07/31 20:16:33 by ulmagner         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef Card_hpp
#define Card_hpp
#include <ctime>
#include <cstdlib>
#include <map>
#include <algorithm>
#include <string>
#include <exception>
#include <iostream>

class Card {
    private:
		int _value;
		std::string _suit;
    public:
		Card() : _value(0), _suit("") {}
        Card( int value, const std::string& color ) : _value(value), _suit(color) {}
        ~Card( void ) {}
		std::string toString() const {
			static const char* names[] = {
				"", "A", "2", "3", "4", "5", "6", "7",
				"8", "9", "10", "J", "Q", "K"
			};
			return names[_value] + std::string(" of ") + _suit;
		}
};

#endif //Card_hpp
