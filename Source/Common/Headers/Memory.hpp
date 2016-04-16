#ifndef __LD_MEMORY_HPP__
#define __LD_MEMORY_HPP__

#include <DataTypes.hpp>

namespace LD
{
	template< typename T >
	void SafeDelete( T *&p_pPointer )
	{
		if( p_pPointer )
		{
			delete p_pPointer;
			p_pPointer = LD_NULL;
		}
	}

	template< typename T >
	void SafeDeleteArray( T *&p_pPointer )
	{
		if( p_pPointer )
		{
			delete [ ] p_pPointer;
			p_pPointer = LD_NULL;
		}
	}
}

#endif // __LD_MEMORY_HPP__

