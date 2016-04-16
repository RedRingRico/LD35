#ifndef __LD_MATHS_HPP__
#define __LD_MATHS_HPP__

#include <DataTypes.hpp>

namespace LD
{
	const LD_FLOAT32 LD_EPSILON = 1.0e-10;
	const LD_FLOAT32 LD_HALF_EPSILON = 1.0e-5;
	const LD_FLOAT32 LD_PI = 3.1415926535897932384626433832795f;

	LD_BOOL IsZero( const LD_FLOAT32 p_Value );
	LD_BOOL IsEqual( const LD_FLOAT32 p_Left, const LD_FLOAT32 p_Right );

	LD_FLOAT32 SquareRoot( const LD_FLOAT32 p_Value );
	LD_FLOAT32 InvSquareRoot( const LD_FLOAT32 p_Value );

	LD_FLOAT32 Absolute( const LD_FLOAT32 p_Value );

	void SineCosine( const LD_FLOAT32 p_Angle, LD_FLOAT32 &p_Sine,
		LD_FLOAT32 &p_Cosine );
}

#endif // __LD_MATHS_HPP__

