#ifndef __LD_MD5_HPP__
#define __LD_MD5_HPP__

#include <DataTypes.hpp>
#include <string>

namespace LD
{
	typedef struct _tagMD5_DIGEST
	{
		unsigned char Digest[ 16 ];
	}MD5_DIGEST,*PMD5_DIGEST;

	LD_BOOL MD5Equal( const MD5_DIGEST &p_Left, const MD5_DIGEST &p_Right );
	void MD5Zero( MD5_DIGEST &p_Digest );
	std::string MD5AsString( const MD5_DIGEST &p_Digest );

	bool operator<( const MD5_DIGEST &p_Left, const MD5_DIGEST &p_Right );
}

#endif // __LD_MD5_HPP__

