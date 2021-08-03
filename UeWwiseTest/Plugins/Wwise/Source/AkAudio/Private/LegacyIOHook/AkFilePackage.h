/********************************************************************************
The content of the files in this repository include portions of the AUDIOKINETIC
Wwise Technology released in source code form as part of the SDK package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use these files in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Copyright (c) 2021 Audiokinetic Inc.
********************************************************************************/

/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided 
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

  Version: <VERSION>  Build: <BUILDNUMBER>
  Copyright (c) <COPYRIGHTYEAR> Audiokinetic Inc.
*******************************************************************************/
//////////////////////////////////////////////////////////////////////
// 
// AkFilePackage.h
//
// This class represents a file package that was created with the 
// AkFilePackager utility app (located in ($WWISESDK)/samples/FilePackager/). 
// It holds a system file handle and a look-up table (CAkFilePackageLUT).
//
// CAkFilePackage objects can be chained together using the ListFilePackages
// typedef defined below.
//
//////////////////////////////////////////////////////////////////////

#ifndef _AK_FILE_PACKAGE_H_
#define _AK_FILE_PACKAGE_H_

#include "AkFilePackageLUT.h"
#include <AK/Tools/Common/AkObject.h>
#include <AK/Tools/Common/AkListBare.h>

//-----------------------------------------------------------------------------
// Name: Base class for items that can be chained in AkListBareLight lists.
//-----------------------------------------------------------------------------
template<class T>
class CAkListAware
{
public:
	CAkListAware()
		: pNextItem( NULL ) {}

	// Implement U_NEXTITEM AkListBare policy.
	static AkForceInline T *& Get( T * in_pItem ) 
	{
		return in_pItem->pNextItem;
	}
	
	T * pNextItem;
};

//-----------------------------------------------------------------------------
// Name: CAkFilePackage 
// Desc: Base class representing a file package (incomplete implementation). 
// It holds a look-up table (CAkFilePackageLUT) and manages memory for the LUT and
// for itself. 
//-----------------------------------------------------------------------------
class CAkFilePackage : public CAkListAware<CAkFilePackage>
{
public:
	// Package factory.
	// Creates a memory pool to contain the header of the file package and this object. 
	// Returns its address.
	template<class T_PACKAGE>
	static T_PACKAGE * Create( 
		const AkOSChar*		in_pszPackageName,	// Name of the file package (for memory monitoring and ID generation).
		AkUInt32 			in_uHeaderSize,		// File package header size, including the size of the header chunk AKPK_HEADER_CHUNK_DEF_SIZE.
		AkUInt32			in_uBlockAlign,		// Alignment of memory block.
		AkUInt32 &			out_uReservedHeaderSize, // Size reserved for header, taking mem align into account.
		AkUInt8 *&			out_pHeaderBuffer	// Returned address of memory for header.
		)
	{
		AKASSERT( in_uHeaderSize > 0 );

		out_pHeaderBuffer = NULL;
		
		// Create memory pool and copy header.
		// The pool must be big enough to hold both the buffer for the LUT's header
		// and a CAkFilePackage object.
		AkUInt8 * pToRelease = NULL;
		out_uReservedHeaderSize = ( ( in_uHeaderSize + in_uBlockAlign - 1 ) / in_uBlockAlign ) * in_uBlockAlign;
		AkUInt32 uMemSize = out_uReservedHeaderSize + sizeof( T_PACKAGE );

		pToRelease = (AkUInt8*)AkMalign(AkMemID_FilePackage, uMemSize, in_uBlockAlign);
		if ( !pToRelease )
			return NULL;

		// Generate an ID.
		AkUInt32 uPackageID = AK::SoundEngine::GetIDFromString( in_pszPackageName );
		
		// Construct a CAkFilePackage at the end of this memory region.
		T_PACKAGE * pFilePackage = AkPlacementNew( pToRelease + out_uReservedHeaderSize ) T_PACKAGE( uPackageID, in_uHeaderSize, pToRelease );
		AKASSERT( pFilePackage );	// Must succeed.

		out_pHeaderBuffer = pToRelease;

		return pFilePackage;
	}

	// Destroy file package and free memory / destroy pool.
	virtual void Destroy();

	// Getters.
	inline AkUInt32 ID() { return m_uPackageID; }
	inline AkUInt32 HeaderSize() { return m_uHeaderSize; }

	// Members.
	// ------------------------------
	CAkFilePackageLUT	lut;		// Package look-up table.

protected:
	AkUInt32			m_uPackageID;
	AkUInt32			m_uHeaderSize;
	// ------------------------------

protected:
	// Private constructors: users should use Create().
	CAkFilePackage();
	CAkFilePackage(CAkFilePackage&);
	CAkFilePackage(AkUInt32 in_uPackageID, AkUInt32 in_uHeaderSize, void * in_pToRelease)
		: m_uPackageID(in_uPackageID)
		, m_uHeaderSize(in_uHeaderSize)
		, m_pToRelease(in_pToRelease)
	{
	}
	virtual ~CAkFilePackage() {}
	
	// Helper.
	static void ClearMemory(
		void *		in_pMemToRelease	// Memory block to free before destroying pool.
		);

protected:
	void *				m_pToRelease;	// LUT storage (only keep this pointer to release memory).
};

//-----------------------------------------------------------------------------
// Name: ListFilePackages
// Desc: AkListBare of CAkFilePackage items.
//-----------------------------------------------------------------------------
typedef AkListBare<CAkFilePackage,CAkListAware,AkCountPolicyWithCount> ListFilePackages;

#endif //_AK_FILE_PACKAGE_H_
