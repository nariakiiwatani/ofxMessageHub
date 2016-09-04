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
	virtual To convert(const From &data) const = 0;
};

template<typename From, typename To=From, typename std::enable_if<std::is_same<From,To>::value, std::nullptr_t>::type = nullptr>
class NoConverter
{
public:
	To convert(const From &data) const { return data; }
};

