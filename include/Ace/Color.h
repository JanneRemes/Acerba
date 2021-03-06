#pragma once

#include <Ace/IntTypes.h>

namespace ace
{
	struct Color32;
	struct Color;

	struct Color
	{
		static const UInt32 Red = 0xFF000000U;
		static const UInt32 Green = 0x00FF0000U;
		static const UInt32 Blue = 0x0000FF00U;

		UInt8 r, g, b, a;

		Color(UInt8 r = 0, UInt8 g = 0, UInt8 b = 0, UInt8 a = 0) : r(r), g(g), b(b), a(a)
		{

		}

		Color(UInt32 hex)
		{
			r = (hex & 0xFF000000) >> 24;
			g = (hex & 0x00FF0000) >> 16;
			b = (hex & 0x0000FF00) >> 8;
			a = (hex & 0x000000FF) >> 0;
		}
	
		Color operator+(const Color& c) const
		{
			return Color( r + c.r, g + c.g, b + c.b, a + c.a);
		}

		Color operator-(const Color& c) const
		{
			return Color(r - c.r, g - c.g, b - c.b, a - c.a);
		}
		

		Color operator*(float scalar) const
		{
			return Color( r * scalar, g * scalar, b * scalar, a * scalar );
		}

		Color operator/(float scalar) const
		{
			return Color( r / scalar, g / scalar, b / scalar, a / scalar );
		}
		

		operator Color32() const;
	};

	struct Color32
	{
		static const UInt32 Red = 0xFF000000U;
		static const UInt32 Green = 0x00FF0000U;
		static const UInt32 Blue = 0x0000FF00U;

		float r, g, b, a;
		Color32(float r = 0, float g = 0, float b = 0, float a = 0) : r(r), g(g), b(b), a(a)
		{
			
		}

		Color32(UInt32 hex)
		{
			Color c(hex);
			*this = c; //<--Don't worry about this :D
		}
		Color32 operator+(const Color32& c32) const
		{
			return{ r + c32.r, g + c32.g, b + c32.b, a + c32.a };
		}

		Color32 operator-(const Color32& c32) const
		{
			return{ r - c32.r, g - c32.g, b - c32.b, a - c32.a };
		}

		Color32 operator*(float scalar) const
		{
			return{ r * scalar, g * scalar, b * scalar, a * scalar };
		}

		Color32 operator/(float scalar) const
		{
			return{ r / scalar, g / scalar, b / scalar, a / scalar };
		}

		operator Color() const;
	};

	/**
		@brief Conversion from Color to Color32
		@return Color32
	*/
	inline Color::operator ace::Color32() const
	{
		return Color32(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
	}
	
	/**
		@brief Conversion from Color32 to Color
		@return Color
	*/
	inline Color32::operator ace::Color() const
	{
		return Color(r * 255.f, g * 255.f, b * 255.f, a * 255.f);
	}

}