/*
  ==============================================================================

    util.h
    Created: 21 Nov 2017 4:26:21pm
    Author:  secci

  ==============================================================================
*/

#pragma once

class CMemoryStream
{
	enum SEEK_ORIGIN
	{
		SO_BEGINNING = 0,
		SO_CURRENT = 1,
		SO_END = 2
	};
public:

	explicit CMemoryStream(int nMemoryDelta = DEFAULT_MEMORY_DELTA) :m_nCapacity(0), m_pMemory(NULL), m_nSize(0), m_nPosition(0)
	{
		SetMemoryDelta(nMemoryDelta);
	}

	~CMemoryStream()
	{
		Clear();
	}
	int Seek(int nOffset, SEEK_ORIGIN nSeekOrigin)
	{
		switch (nSeekOrigin)
		{
		case SO_BEGINNING:
			m_nPosition = (int)nOffset;
			break;
		case SO_CURRENT:
			m_nPosition += (int)nOffset;
			break;
		case SO_END:
			m_nPosition = m_nSize + (int)nOffset;
			break;
		}

		return m_nPosition;
	}

public:
	enum { DEFAULT_MEMORY_DELTA = 1024*1024*4 }; // 默认内存步长
	enum { MIN_MEMORY_DELTA = 256 };      // 最小内存步长

private:
	unsigned char* m_pMemory;      // 缓冲区指针
	int   m_nSize;        // 当前流的大小
	int   m_nPosition;    // 当前位置
	int   m_nCapacity;    // 缓冲区大小.真正大大小.包含给未来分配的长度
	int   m_nMemoryDelta; // 增长的步长

private:
	void  SetPointer(void *pMemory, int nSize)
	{
		m_pMemory = (unsigned char*)pMemory;
		m_nSize = nSize;
	}
	void  SetMemoryDelta(int nNewMemoryDelta)
	{
		if (nNewMemoryDelta != DEFAULT_MEMORY_DELTA)
		{
			if (nNewMemoryDelta  < MIN_MEMORY_DELTA)
				nNewMemoryDelta = MIN_MEMORY_DELTA;

			//  2^N
			for (int i = sizeof(int) * 8 - 1; i >= 0; i--)
				if (((1 << i) & nNewMemoryDelta) != 0)
				{
					nNewMemoryDelta &= (1 << i);
					break;
				}
		}

		m_nMemoryDelta = nNewMemoryDelta;
	}
	void  SetCapacity(int nNewCapacity)
	{
		SetPointer(Realloc(nNewCapacity), m_nSize);
		m_nCapacity = nNewCapacity;
	}
	char* Realloc(int& nNewCapacity)
	{
		unsigned char* pResult;

		if (nNewCapacity  > 0 && nNewCapacity != m_nSize)
			nNewCapacity = (nNewCapacity + (m_nMemoryDelta - 1)) & ~(m_nMemoryDelta - 1);

		pResult = m_pMemory;
		if (nNewCapacity != m_nCapacity)
		{
			if (nNewCapacity == 0)
			{
				free(m_pMemory);
				pResult = NULL;
			}
			else
			{
				if (m_nCapacity == 0)
					pResult = (unsigned char*)malloc(nNewCapacity);
				else
					pResult = (unsigned char*)realloc(m_pMemory, nNewCapacity);
			}
		}

		return (char*)pResult;
	}

public:
	int GetPosition() const { return const_cast <CMemoryStream& >(*this).Seek(0, SO_CURRENT); }

	void SetPosition(int nPos) { Seek(nPos, SO_BEGINNING); }

	void* GetMemory() { return m_pMemory; }
	void* GetCurrentPtr() { return m_pMemory + m_nPosition; }

	int Read(void *pBuffer, int nCount)
	{
		int nResult = 0;

		if (m_nPosition >= 0 && nCount >= 0)
		{
			nResult = m_nSize - m_nPosition;
			if (nResult  > 0)
			{
				if (nResult  > nCount) nResult = nCount;
				memmove(pBuffer, m_pMemory + (unsigned int)m_nPosition, nResult);
				m_nPosition += nResult;
			}
		}

		return nResult;
	}

	int Write(const void *pBuffer, int nCount)
	{
		int nResult = 0;
		int nPos;

		if (m_nPosition >= 0 && nCount >= 0)
		{
			nPos = m_nPosition + nCount;
			if (nPos  > 0)
			{
				if (nPos  > m_nSize)
				{
					if (nPos  > m_nCapacity)
						SetCapacity(nPos);
					m_nSize = nPos;
				}
				memmove(m_pMemory + (unsigned int)m_nPosition, pBuffer, nCount);
				m_nPosition = nPos;
				nResult = nCount;
			}
		}

		return nResult;
	}

	bool ReadBuffer(void *pBuffer, int nCount)
	{
		if (nCount != 0 && Read(pBuffer, nCount) != nCount)
			return false;
		else
			return true;
	}

	bool WriteBuffer(void *pBuffer, int nCount)
	{
		if (nCount != 0 && Write(pBuffer, nCount) != nCount)
			return false;
		else
			return true;
	}

	int GetSize() const
	{
		int nPos, nResult;

		nPos = const_cast <CMemoryStream& >(*this).Seek(0, SO_CURRENT);
		nResult = const_cast <CMemoryStream& >(*this).Seek(0, SO_END);
		const_cast <CMemoryStream& >(*this).Seek(nPos, SO_BEGINNING);

		return nResult;
	}

	void SetSize(int nSize)
	{
		int nOldPos = m_nPosition;

		SetCapacity((int)nSize);
		m_nSize = (int)nSize;
		if (nOldPos  > nSize) Seek(0, SO_END);
	}

	void Clear()
	{
		SetCapacity(0);
		m_nSize = 0;
		m_nPosition = 0;
	}

	void Zero()
	{
		m_nSize = 0;
		m_nPosition = 0;
	}
	void writeRGB(float a, float b, float c, bool onlyRead)
	{
		if (onlyRead)
		{
			char buf[16] = { 0 };
			snprintf(buf, 16, "%.1f ", a);
			Write(buf, strlen(buf));
		}
		else
		{
			char buf[16] = { 0 };
			snprintf(buf, 16, "%.1f %.1f %.1f ", a, b, c);
			Write(buf, strlen(buf));
		}

	}
	void writeBreak()
	{
		char * b = "\r\n";
		Write(b, strlen(b));
	}
	void wirteZero()
	{
		int a = 0;
		Write(&a, 1);
	}
};