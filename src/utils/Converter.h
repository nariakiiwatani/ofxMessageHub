//
//  Converter.h
//
//  Created by Iwatani Nariaki on 2016/09/03.
//
//

#pragma once

template<typename From, typename To>
class Converter
{
public:
	virtual bool convert(const From &from, To &to) const=0;
};

template<typename From, typename To=From, typename std::enable_if<std::is_same<From,To>::value, std::nullptr_t>::type = nullptr>
class NoConverter
{
public:
	bool convert(const From &from, To &to) const {
		to = from;
		return true;
	}
};

