#pragma once

#include <string.h>
#include <assert.h>

#include "types.h"
#include "murmurhash.h"

class CDynBitAry
{
public:

	friend class CDynBitAry;

	CDynBitAry()
	: m_aryByte(nullptr)
	, m_cBit(0)
	, m_cByte(0)
	, m_hash(0)
	, m_fIsHashDirty(true)
	{
	}

	~CDynBitAry()
	{
		delete [] m_aryByte;
	}
	
	void SetSize(size_t cBit)
	{
		m_fIsHashDirty = true;
		
		m_cBit = cBit;
		
		// https://stackoverflow.com/questions/3407012/c-rounding-up-to-the-nearest-multiple-of-a-number
		// round cBit to the nearest multiple of 8
		
		size_t cBitNearestMultipleOfEight = (cBit + 8 - 1) & ~(8 - 1);

		// divide number of bits by 8 to get bytes

		m_cByte = cBitNearestMultipleOfEight >> 3;

		if(m_aryByte)
		{
			delete [] m_aryByte;
		}

		m_aryByte = new u8[m_cByte];

		memset(m_aryByte, 0, sizeof(u8) * m_cByte);
	}

	bool At(size_t iBit) const
	{
		size_t iByte = iBit >> 3;
		size_t iBitInByte =  iBit - (iByte << 3);
		u8 mask = 1 << iBitInByte;

		return (m_aryByte[iByte] & mask) != 0;
	}

	void Set(size_t iBit)
	{
		size_t iByte = iBit >> 3;
		size_t iBitInByte =  iBit - (iByte << 3);
		u8 mask = 1 << iBitInByte;

		m_aryByte[iByte] |= mask;

		m_fIsHashDirty = true;
	}

	bool FEquals(const CDynBitAry * other) const
	{
		if(C() != other->C())
			return false;

		if(!m_fIsHashDirty && !other->m_fIsHashDirty && m_hash != other->m_hash)
			return false;

		for(size_t iByte = 0; iByte < m_cByte; ++iByte)
		{
			if(m_aryByte[iByte] != other->m_aryByte[iByte])
			{
				return false;
			}
		}

		return true;
	}

	size_t NHash() const
	{
		// hash is special. even if we are const, still update m_fIsHashDirty and m_hash
		// BB(matthewd) hmm... maybe we can just call ComputeHash before we need NHash
		
		if(m_fIsHashDirty)
		{
			*(const_cast<bool*>(&m_fIsHashDirty)) = false;
			
			MurmurHash3_x86_32(m_aryByte, m_cByte, const_cast<size_t*>(&m_hash));
		}

		return m_hash;
	}

	size_t C() const
	{
		return m_cBit;
	}

	void Union(const CDynBitAry * other)
	{
		assert(C() == other->C());
		
		for(size_t iByte = 0; iByte < m_cByte; ++iByte)
		{
			m_aryByte[iByte] |= other->m_aryByte[iByte];
		}

		m_fIsHashDirty = true;
	}
	
private:

	u8 * m_aryByte;
	size_t m_cBit;
	size_t m_cByte;

	size_t m_hash;
	bool m_fIsHashDirty;
};
