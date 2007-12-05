#ifndef _MEMORY_POOL
#define _MEMORY_POOL

namespace MemPool
{
	typedef BYTE TByte;

	typedef struct MemoryBlock
	{
		TByte*       pData;			// Pointer to the actual Data
		unsigned int DataSize;	    // Size of the Data Block
	}
	MemoryBlock, *PMemoryBlock;

	class CMemPool
	{
	public:
        // the default unit size is 11K which is bigger than most packet size
#ifdef _DEBUG
		CMemPool(int code, unsigned int poolsize = 1024 * 1024 * 5, unsigned int unitsize = 1024 * 11); // Constructor
#else
		CMemPool(unsigned int poolsize = 1024 * 1024 * 5, unsigned int unitsize = 1024 * 11); // Constructor
#endif
		
		virtual ~CMemPool(); // Destructor
	
		// Attribute
	public:

		//TByte*       GetMemPoolEntry() const;
		unsigned int GetMemPoolSize() const;
		unsigned int GetCurUsedSize() const;
		bool         IsFree() const; 

#ifdef _DEBUG
		int      m_nCode;
#endif

		// Operation
	public:

		virtual TByte* GetMemory(unsigned int size);      // Get Memory from the Memory Pool
		virtual void   FreeMemory();                      // Free Memory to Memory Pool

	protected:

		virtual void         InitMemPool(); // Init the Memory Pool
		virtual void         ClearMemPool();
		virtual PMemoryBlock RequestBlock(unsigned int blocksize);
		virtual unsigned int CalcBlockSize(unsigned int blocksize);

	private:

		CTypedPtrList<CPtrList, MemoryBlock*> m_MemoryUnitList;     //
		POSITION     m_CurUnitPos;       // The Current Position of the Memory Unit List
		TByte*       m_pPoolEntry;       // The Pool Entry

		unsigned int m_nBlockUnitSize;   // Unit block size
		unsigned int m_nPoolSize;        // The Memory Pool size

		unsigned int m_nCurUsedSize;     // Current Used Size

#ifdef _THREAD_SAFE
		CRITICAL_SECTION m_CriticalSection;
#endif
	};
};

#endif  // _MEMORY_POOL