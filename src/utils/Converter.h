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
	using FromType = From;
	using ToType = To;
	virtual To convert(const From &data) const = 0;
};

