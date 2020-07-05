//2012.01.06: TArray (TList) constructor added
//06.21.06: removed xerror.h
//12/06/04: added _iIndex and AddCurrent, which rolls through the array
//iIndex is the index of the last added entry
//12/06/04: changed Add and AddAt to return references. Still could be dangerous.
//11/03/04: change Add and AddAt to return pointers in TArray. Could be dangerous.
//11/02/04: added AddNoRepeat to TIArray, which only adds if the pointer isn't already in the array
//09/28/04: added FindIndex (similar to Find, but returns the index)
//01/09/03: changed NO_VALIDATEs to VALIDATEs
/* 11/11/02: fixed some ANSI incompatibilities 1/23/02: added some functionality back to TIArrayExt
9/4/01: added skeleton TIArrayExt, which allows for offset base indeces.
Much of the functionality has been lost, and some may be changed from that of TIArray */
#ifndef __TEMPLATE_ARRAY_H
#define __TEMPLATE_ARRAY_H

#include <iostream>

#include "template/xerror.h"
#include "template/list.h"

using namespace std;

template < class T > class TArray
{
private:
   unsigned _nSize;
   T * _tData;
   int _iIndex; //seems to track stepping??? Wow. What a mess!!!!

public:
   TArray( void )
   {
      _nSize = 0;
      _tData = 0;
      _iIndex = 1;
   }

   TArray( unsigned nSize );
   TArray( unsigned nSize, const T & t );
   TArray( const TArray & tArray );
   TArray( TList < T > &);

   TArray & operator = ( const TArray & tArray );

   virtual ~TArray( void )
   {
      if ( _tData )
         delete[] _tData;
   }

   T & operator[] ( unsigned i ) const
   {
#ifdef __VALIDATE__
      if ( i < 0 || i >= _nSize )
         throw XError( "Array out-of-bounds" );
#endif
      return _tData[i];
   }

   T & GetLast( void )
   { return _tData[_nSize - 1]; }

   unsigned GetSize( void )const
   { return _nSize; }

   //   int GetMax( void )const
   //   { return _nSize - 1; }

   T & Add( const T & );
   T & AddAt( const T &, unsigned );
   T & AddIndexed( const T & );

   T & Current( unsigned i = 0 ) const
   {
      int index = _iIndex + i;
      if ( index >= GetSize() ) index -= GetSize(); return ( * this ) [index];
   }

   unsigned IsFull( void ) { return _iIndex == ( GetSize() - 1 ); }

   unsigned Resize( unsigned );
protected:
   unsigned Expand( unsigned );
   unsigned Contract( unsigned );

public:
   unsigned Clear( void );

   unsigned Destroy( void )
   {
      if ( _tData )
      { delete[] _tData; }

      _tData = 0;
      _iIndex = 1;
      _nSize = 0;

      return 0;
   }

   T * Find( const T & ) const;
   unsigned FindIndex( const T & ) const;
   int operator == ( const TArray < T > & ) const;

//     ostream & operator << (ostream & , const TArray<T>&);
   //  friend istream& operator >> <>(istream&, TArray<T>&);
};

template < class T >
TArray < T >::TArray( unsigned nSize )
{
   _nSize = nSize;
   if ( _nSize > 0 )
      _tData = new T[_nSize];
   else
      _tData = 0;
   _iIndex = nSize;
}

template < class T >
TArray < T >::TArray( unsigned nSize, const T & t )
{
   _nSize = nSize;
   if ( _nSize > 0 )
      _tData = new T[_nSize];
   else
      _tData = 0;
   for ( unsigned i = 0; i < _nSize; i++ )
   { _tData[i] = t; }
   _iIndex = nSize;
}

template < class T >
TArray < T >::TArray( const TArray < T > & tArray )
{
   _nSize = tArray._nSize;
   if ( _nSize > 0 )
      _tData = new T[_nSize];
   else
      _tData = 0;
   for ( unsigned i = 0; i < _nSize; i++ )
   { _tData[i] = tArray._tData[i]; }
   _iIndex = tArray._iIndex;
}

template < class T > TArray < T >::TArray( TList < T > & tList)
{
    _nSize = tList.GetItemsInContainer();
    if (_nSize > 0)
        _tData = new T[_nSize];
    else
        _tData = 0;

    TListIterator<T> iterator(tList);
    for (unsigned i = 0; i < _nSize; i++)
    {
        _tData[i] = *iterator.Current();
        iterator++;
    }
    _iIndex = 0;
}

template < class T >
TArray < T > & TArray < T >::operator = ( const TArray < T > & tArray )
{
   _nSize = tArray._nSize;
   if ( _tData )
      delete[] _tData;
   if ( _nSize > 0 )
      _tData = new T[_nSize];
   else
      _tData = 0;
   for ( unsigned i = 0; i < _nSize; i++ )
   { _tData[i] = tArray._tData[i]; }
   _iIndex = tArray._iIndex;
   return * this;
}

template < class T >
T & TArray < T >::Add( const T & t )
{
   Expand( 1 );
   ( * this ) [GetSize() - 1] = t;
   return _tData[GetSize() - 1];
}

template < class T >
T & TArray < T >::AddAt( const T & t, unsigned i )
{
   if ( i > _nSize )
   { Expand( i - _nSize - 1 ); }
   ( * this ) [i] = t;
   return _tData[i];
}

template < class T >
T & TArray < T >::AddIndexed( const T & t )
{
   _iIndex++;
   if ( _iIndex >= GetSize() ) _iIndex = 0;
   return AddAt( t, _iIndex );
}

template < class T >
unsigned TArray < T >::Resize( unsigned nNewSize )
{
   if ( nNewSize > _nSize ) Expand( nNewSize - _nSize );
   return GetSize();
}

template < class T >
unsigned TArray < T >::Expand( unsigned nBy )
{
   if ( nBy > 0 )
   {
      TArray < T > tTemp( * this );
      unsigned nOldSize = _nSize;
      _nSize += nBy;
      if ( _tData ) delete[] _tData;
      _tData = new T[_nSize];
      for ( unsigned i = 0; i < nOldSize; i++ )
      { _tData[i] = tTemp[i]; }
   }
   return GetSize();
}

template<class T>
unsigned TArray<T>::Contract(unsigned nBy)
{
	if (nBy > 0)
	{
		TArray<T> tTemp(*this);

		if (nBy > _nSize)
			_nSize = 0;
		else
			_nSize -= nBy;

		if (_tData)
		{
			delete[] _tData;
			_tData = 0;
		}
		if (_nSize)
			_tData = new T[_nSize];
		for (unsigned i = 0; i < _nSize; i++)
		{
			_tData[i] = tTemp[i];
		}
	}
	return GetSize();
}

template < class T >
T * TArray < T >::Find( const T & t ) const
{
   for ( unsigned i = 0; i < _nSize; i++ )
   { if ( _tData[i] == t ) return & _tData[i]; }
   return 0;
}

template < class T >
unsigned TArray < T >::FindIndex( const T & t ) const
{
   for ( unsigned i = 0; i < _nSize; i++ )
   { if ( _tData[i] == t ) return i; }
   return -1; //return value is unsigned!
}

template < class T >
int TArray < T >::operator == ( const TArray < T > & tArray ) const
{
   for ( unsigned i = 0; i < _nSize; i++ )
   { if ( !( _tData[i] == tArray[i] ) ) return 0; }
   return 1;
}

template < class T >
istream & operator >> ( istream & stream, TArray < T > & tArray )
{
   int nSize;
   stream >> nSize;

   for ( unsigned i = 0; i < nSize; i++ )
   {
      T t;
      stream >> t;
      tArray.Add( t );
   }
   return stream;
}

template < class T >
ostream & operator << ( ostream & stream, const TArray < T > & tArray )
{
   stream << tArray.GetSize() << endl;
   for ( unsigned i = 0; i < tArray.GetSize(); i++ )
   {
      if ( i ) stream << '\t';
      stream << tArray[i];
   }

   return stream;
}

/*template class TIArray holds an indirect array of object pointers to object
type T.  TIArray assumes ownership of all objects added to it.
Note that, as coded, copying a TIArray will call the copy constructor for every
pointer.  This will cause problems if the pointers are to a base class */


template < class T >
class TIArray
{
private:
   int _nSize; //size of the array
   T * * _tiData; //array of object pointers

protected:
   void TestIndex( int i ) const
   {
      if ( i < 0 || i >= _nSize )
         throw XError( "Array access out-of-bounds" );
   }

   int Expand( int );
   int Contract( int );

public:
   TIArray( void )
   {
      _nSize = 0;
      _tiData = 0;
   }
   TIArray( int nSize );
   TIArray( const TIArray & );

   TIArray & operator = ( const TIArray & );

   virtual ~TIArray( void );

   T * & operator[] ( int i ) const
   {
#ifdef __VALIDATE__
      TestIndex( i );
#endif
      return _tiData[i];
   }

   virtual int Add( T * );
   virtual int AddNoRepeat( T * );
   virtual int AddAt( T *, int );

   int Delete( int );
   int Destroy( int );

   int GetSize( void )const
   { return _nSize; }

   int GetItemsInContainer( void )const;

   int FindIndex( T * ) const;

   int Resize( int );

   int Empty( void );
   int Flush( void );
   int Clear( void );
   int Destroy( void );

   // friend ostream& operator << <>(ostream&, const TIArray<T>&);
   //		friend istream& operator >> <>(istream&, TIArray<T>&);

   int TestNullInvariant( void )const;
   int TestInvariant( void )const;
};



template < class T >
int TIArray < T >::TestInvariant( void )const
{
   if ( _tiData && !_nSize )
      throw XError( "Error in array!" );
   if ( !_tiData && _nSize )
      throw XError( "Error in array!" );
   if ( _nSize < 0 )
      throw XError( "Error in array!" );
   return 1;
}

template < class T >
int TIArray < T >::TestNullInvariant( void )const /* ensure that there are no NULL pointers */
{
   for ( int i = 0; i < _nSize; i++ )
   {
      if ( !_tiData[i] )
         throw XError( "NULL found in array" );
   }
   return 1;
}

template < class T >
TIArray < T >::TIArray( int nSize )
{
   _nSize = nSize;
   if ( _nSize > 0 )
      _tiData = new T * [_nSize];
   else
      _tiData = 0;
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = 0; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
}

template < class T >
TIArray < T >::TIArray( const TIArray < T > & tiArray )
{
   _nSize = tiArray._nSize;
   if ( _nSize > 0 )
      _tiData = new T * [_nSize];
   else
      _tiData = 0;
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = tiArray._tiData[i]; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
}

template < class T >
TIArray < T >::~TIArray( void )
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   if ( _tiData )
   { delete[] _tiData; }
}

template < class T >
TIArray < T > & TIArray < T >::operator = ( const TIArray < T > & tiArray )
{
   _nSize = tiArray._nSize;
   if ( _tiData )
   {
      delete[] _tiData;
      _tiData = 0;
   }
   if ( _nSize > 0 ) _tiData = new T * [_nSize];
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = tiArray._tiData[i]; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
   return * this;
}

template < class T >
int TIArray < T >::GetItemsInContainer( void )const
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   int nCount = 0;
   for ( int i = 0; i < GetSize(); i++ )
   { if ( operator[] ( i ) ) nCount++; }
   return nCount;
}

template < class T >
int TIArray < T >::FindIndex( T * pt ) const
{
   for ( int i = 0; i < _nSize; i++ )
   { if ( _tiData[i] == pt ) return i; }
   return -1;
}

template < class T >
int TIArray < T >::Add( T * pT )
{
   int i = 0;
   for ( ; i < _nSize; i++ )
   {
      if ( !_tiData[i] )
      {
         _tiData[i] = pT;
         return i;
      }
   }
   Expand( 1 );
   operator[] ( i ) = pT;
   return i;
}

template < class T >
int TIArray < T >::AddNoRepeat( T * pT )
{
   int i = 0;
   //first check to see if this pointer exists
   for ( ; i < _nSize; i++ )
      if ( _tiData[i] == pT ) return i;

   //if not, look for an open spot
   for ( ; i < _nSize; i++ )
      if ( !_tiData[i] )
      {
         _tiData[i] = pT;
         return i;
      }

   //if no open spots, expand one
   Expand( 1 );
   operator[] ( i ) = pT;
   return i;
}

template < class T >
int TIArray < T >::Delete( int i )
{
   if ( operator[] ( i ) )
   {
      delete _tiData[i];
      operator[] ( i ) = 0;
   }
   return GetItemsInContainer();
}

template < class T >
int TIArray < T >::Destroy( int index ) /*deletes an item and removes the slot from the
array, moving everything
else back as necessary */
{
   //delete item
   Delete( index );

   //slide remaining items up one
   int i = index;
   for ( ; i < _nSize - 1; i++ )
   { _tiData[i] = _tiData[i + 1]; }

   //make last index null
   _tiData[i] = 0;

   //remove last index
   Contract( 1 );
   return i;
}

template < class T >
int TIArray < T >::Empty( void ) /* Empties the entire array without deleting the data pointers */
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = 0; }
#ifdef __VALIDATE__
   if ( GetItemsInContainer() )
      throw XError( "Error in item count!" );
#endif
   return 0;
}

template < class T >
int TIArray < T >::Flush( void ) /* Empties the array and deletes all of the data pointers */
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   for ( int i = 0; i < _nSize; i++ )
   {
      if ( _tiData[i] )
      {
         delete _tiData[i];
         _tiData[i] = 0;
      }
   }
#ifdef __VALIDATE__
   if ( GetItemsInContainer() )
      throw XError( "Error in item count!" );
#endif
   return 0;
}

template < class T >
int TIArray < T >::Clear( void ) /* destroys entire array without deleting data pointers */
{
   if ( _tiData )
   {
      Empty();
      delete[] _tiData;
      _tiData = 0;
      _nSize = 0;
   }
   return 0;
}

template < class T >
int TIArray < T >::Destroy( void ) /* destroys entire array , deleting the
pointers in the process */
{
   if ( _tiData )
   {
      Flush();
      delete[] _tiData;
      _tiData = 0;
      _nSize = 0;
   }

   return 0;
}

template < class T >
int TIArray < T >::Resize( int nNewSize )
{
   if ( nNewSize > _nSize ) Expand( nNewSize - _nSize );
   else if ( nNewSize < _nSize ) Contract( _nSize - nNewSize );
   return _nSize;
}

template < class T >
int TIArray < T >::Expand( int nBy )
{
   if ( nBy > 0 )
   {
      if ( !_tiData )
      {
         _nSize = nBy;
         _tiData = new T * [_nSize];
         for ( int i = 0; i < _nSize; i++ )
         { _tiData[i] = 0; }
         return _nSize;
      }
      else
      {
         TIArray < T > tiTemp( * this );
         _nSize += nBy;
         delete[] _tiData;
         _tiData = new T * [_nSize];
         int i = 0;
         for ( ; i < _nSize - nBy; i++ )
         { _tiData[i] = tiTemp[i]; }
         for ( ; i < _nSize; i++ )
         { _tiData[i] = 0; }
      }
   }
#ifdef __VALIDATE__
   TestInvariant();
#endif
   return _nSize;
}

template < class T >
int TIArray < T >::Contract( int nBy )
{
   if ( nBy > _nSize )
      throw XError( "Error Contracting TIArray" );
   if ( nBy > 0 )
   {
      TIArray < T > tiTemp( * this );
      int nOldSize = _nSize;
      _nSize -= nBy;

      if ( _tiData )
      {
         delete[] _tiData;
         _tiData = 0;
      }
      if ( _nSize ) _tiData = new T * [_nSize];

      //copy those data not lost in contraction
      int i = 0;
      for ( ; i < _nSize; i++ )
      { _tiData[i] = tiTemp[i]; }

      for ( ; i < nOldSize; i++ )
      {
         if ( tiTemp[i] )
            throw XError( "Losing data in contraction!" );
      }
   }
#ifdef __VALIDATE__
   TestInvariant();
#endif
   return _nSize;
}



template < class T >
class TISArray : public TIArray < T > /* automatically sorted array class */
{
private:
   protected : public : TISArray( void ) : TIArray < T > ()
   { }

   TISArray( int nSize ) : TIArray < T > ( nSize )
   { }

   TISArray( const TISArray < T > & tisArray ) : TIArray < T > ( tisArray )
   { }

   TISArray < T > & operator = ( const TISArray < T > & tisArray )
   {
      TIArray < T >::operator = ( tisArray );
      return * this;
   }

   ~TISArray( void )
   { }

   int Add( T * );
};



template < class T >
int TISArray < T >::Add( T * pT )
{
   //first find index of item immediately preceding placement
   int i;
   for ( i = this->GetSize()-1; i >= 0; i-- )
   {
      if ( !this->operator[] ( i ) )
      { continue; }
      else
      {
         if ( * pT < * this->operator[] ( i ) )
         { break; }
         else { }
      }
   }

   //now i is index of preceding member
   //if i==-1, pT>all items, so the item goes in the front

   AddAt( pT, ++i );
   return i;
}

template < class T >
int TIArray < T >::AddAt( T * pT, int index )
{
   if ( index >= _nSize )
   { Resize( index + 1 ); }

   if ( !operator[] ( index ) )
   { operator[] ( index ) = pT; }
   else
   {
      Resize( _nSize + 1 );
      for ( int j = _nSize - 1; j > index; j-- )
      { operator[] ( j ) = operator[] ( j - 1 ); }
      operator[] ( index ) = pT;
   }
   return _nSize;
}


/*
template < class T >
ostream & operator << ( ostream & stream, const TIArray < T > & tiArray )
{
   stream << tiArray.GetItemsInContainer() << endl;
   for ( int i = 0; i < tiArray.GetSize(); i++ )
   {
      if ( i ) stream << '\t';
      if ( tiArray[i] )
      { stream << * tiArray[i]; }
   }

   return stream;
}*/

/*template class TIArrayExt is an extension of TIArray. It allows you to set the start
of the array to be something other than [0] */



template < class T >
class TIArrayExt
{
private:
   int _nSize; //size of the array
   int _nLower; //start index
   T * * _tiData; //array of object pointers
   T * * _tiIndex; //offset pointer for accessing data

protected:
   void TestIndex( int i ) const
   {
      if ( i < _nLower || i >= _nSize + _nLower )
         throw XError( "Array access out-of-bounds" );
   }

   /* int Expand(int); */
   /* int Contract(int); */
public:
   TIArrayExt( void )
   {
      _nSize = 0;
      _nLower = 0;
      _tiData = 0;
      _tiIndex = 0;
   }
   TIArrayExt( int, int = 0 );
   TIArrayExt( const TIArrayExt & );

   TIArrayExt & operator = ( const TIArrayExt & );

   virtual ~TIArrayExt( void );

   T * & operator[] ( int i ) const
   {
#ifdef __VALIDATE__
      TestIndex( i );
#endif
      return _tiIndex[i];
   }

   /* virtual int Add(T*); */
   /* virtual int AddAt(T*, int); */

   int Delete( int );

   /* int Destroy(int); */

   int GetSize( void )const
   { return _nSize; }

   int GetItemsInContainer( void )const;

   /* int Resize(int, int=0); */

   int Empty( void );
   int Flush( void );
   /* int Clear(void); */
   /* int Destroy(void); */

   //  friend ostream& operator << <>(ostream&, const TIArrayExt<T>&);

   int TestNullInvariant( void )const;
   int TestInvariant( void )const;
};



template < class T >
int TIArrayExt < T >::TestInvariant( void )const
{
   if ( _tiData && !_nSize )
      throw XError( "Error in array!" );
   if ( !_tiData && _nSize )
      throw XError( "Error in array!" );
   if ( _nSize < 0 )
      throw XError( "Error in array!" );
   return 1;
}

template < class T >
int TIArrayExt < T >::TestNullInvariant( void )const /* ensure that there are no NULL pointers */
{
   for ( int i = _nLower; i < _nSize + _nLower; i++ )
   {
      if ( !_tiIndex[i] )
         throw XError( "NULL found in array" );
   }
   return 1;
}

template < class T >
TIArrayExt < T >::TIArrayExt( int size, int lower ) : _nSize( size ), _nLower( lower )
{
   if ( _nSize > 0 )
   {
      _tiData = new T * [_nSize];
      _tiIndex = _tiData - _nLower;
   }
   else
      _tiIndex = _tiData = 0;

   for ( int i = _nLower; i < _nSize + _nLower; i++ )
   { _tiIndex[i] = 0; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
}

template < class T >
TIArrayExt < T >::TIArrayExt( const TIArrayExt < T > & tiArray ) : _nSize( tiArray._nSize ),
_nLower( tiArray._nLower )
{
   if ( _nSize > 0 )
   {
      _tiData = new T * [_nSize];
      _tiIndex = _tiData - _nLower;
   }
   else
      _tiIndex = _tiData = 0;

   for ( int i = _nLower; i < _nSize + _nLower; i++ )
   { _tiIndex[i] = tiArray._tiIndex[i]; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
}

template < class T >
TIArrayExt < T >::~TIArrayExt( void )
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   if ( _tiData )
   { delete[] _tiData; }
}

template < class T >
TIArrayExt < T > & TIArrayExt < T >::operator = ( const TIArrayExt < T > & tiArray )
{
   _nSize = tiArray._nSize;
   _nLower = tiArray._nLower;
   if ( _tiData )
   {
      delete[] _tiData;
      _tiIndex = _tiData = 0;
   }
   if ( _nSize > 0 )
   {
      _tiData = new T * [_nSize];
      _tiIndex = _tiData - _nLower;
   }

   for ( int i = _nLower; i < _nSize + _nLower; i++ )
   { _tiIndex[i] = tiArray._tiIndex[i]; }
#ifdef __VALIDATE__
   TestInvariant();
#endif
   return * this;
}

template < class T >
int TIArrayExt < T >::GetItemsInContainer( void )const
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   int nCount = 0;
   for ( int i = this->_lower; i < this->_lower + GetSize(); i++ )
   { if ( operator[] ( i ) ) nCount++; }
   return nCount;
}

/* template <class T> int TIArrayExt<T>::Add(T* pT) */
/* { */
/* int i=0; */
/* for(; i<_nSize; i++) */
/* { */
/* if(!_tiData[i]) */
/* { */
/* _tiData[i]=pT; */
/* return i; */
/* } */
/* } */
/* Expand(1); */
/* operator [](i)=pT; */
/* return i; */
/* } */

template < class T >
int TIArrayExt < T >::Delete( int i )
{
   if ( operator[] ( i ) )
   {
      delete _tiData[i];
      operator[] ( i ) = 0;
   }
   return GetItemsInContainer();
}

/* template <class T> int TIArrayExt<T>::Destroy(int index) */
/* deletes an item and removes the slot from the array, moving everything else */
/* back as necessary */
/*  */
/* { */
/* //delete item */
/* Delete(index); */

/* //slide remaining items up one */
/* int i=index; */
/* for(; i<_nSize-1; i++) */
/* { */
/* _tiData[i]=_tiData[i+1]; */
/* } */

/* //make last index null */
/* _tiData[i]=0; */
/* //	_nItems--; */

/* //remove last index */
/* Contract(1); */
/* return i; */
/* } */

template < class T >
int TIArrayExt < T >::Empty( void ) /* Empties the entire array without deleting
the data pointers */
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   for ( int i = 0; i < _nSize; i++ )
   { _tiData[i] = 0; }
#ifdef __VALIDATE__
   if ( GetItemsInContainer() )
      throw XError( "Error in item count!" );
#endif
   return 0;
}

template < class T >
int TIArrayExt < T >::Flush( void ) /* Empties the array and deletes all of the data pointers */
{
#ifdef __VALIDATE__
   TestInvariant();
#endif
   for ( int i = 0; i < _nSize; i++ )
   {
      if ( _tiData[i] )
      {
         delete _tiData[i];
         _tiData[i] = 0;
      }
   }
#ifdef __VALIDATE__
   if ( GetItemsInContainer() )
      throw XError( "Error in item count!" );
#endif
   return 0;
}

/* template <class T> int TIArrayExt<T>::Clear(void) */
/* destroys entire array without deleting data pointers */
/*  */
/* { */
/* if(_tiData) */
/* { */
/* Empty(); */
/* delete[] _tiData; */
/* _tiData=0; */
/* _nSize=0; */
/* } */
/* return 0; */
/* } */

/* template <class T> int TIArrayExt<T>::Destroy(void) */
/* destroys entire array , deleting the pointers in the process */
/*  */
/* { */
/* if(_tiData) */
/* { */
/* Flush(); */
/* delete[] _tiData; */
/* _tiData=0; */
/* _nSize=0; */
/* } */

/* return 0; */
/* } */

/* template <class T> int TIArrayExt<T>::Expand(int nBy) */
/* { */
/* if(nBy>0) */
/* { */
/* if(!_tiData) */
/* { */
/* _nSize=nBy; */
/* _tiData=new T*[_nSize]; */
/* for(int i=0; i<_nSize; i++) */
/* { */
/* _tiData[i]=0; */
/* } */
/* return _nSize; */
/* } */
/* else */
/* { */
/* TIArrayExt<T> tiTemp(*this); */
/* _nSize+=nBy; */
/* delete[] _tiData; */
/* _tiData=new T*[_nSize]; */
/* int i=0; */
/* for(; i<_nSize-nBy; i++) */
/* { */
/* _tiData[i]=tiTemp[i]; */
/* } */
/* for(; i<_nSize; i++) */
/* { */
/* _tiData[i]=0; */
/* } */
/* } */
/* } */
/* #ifdef __VALIDATE__ */
/* TestInvariant(); */
/* #endif */
/* return _nSize; */
/* } */

/* template <class T> int TIArrayExt<T>::Contract(int nBy) */
/* { */
/* if(nBy>_nSize) */
/* throw XError("Error Contracting TIArrayExt"); */
/* if(nBy>0) */
/* { */
/* TIArrayExt<T> tiTemp(*this); */
/* int nOldSize=_nSize; */
/* _nSize-=nBy; */

/* if(_tiData) */
/* { */
/* delete[] _tiData; */
/* _tiData=0; */
/* } */
/* if(_nSize) _tiData=new T*[_nSize]; */

/* //copy those data not lost in contraction */
/* int i=0; */
/* for(; i<_nSize; i++) */
/* { */
/* _tiData[i]=tiTemp[i]; */
/* } */

/* for(; i<nOldSize; i++) */
/* { */
/* if(tiTemp[i]) */
/* throw XError("Losing data in contraction!"); */
/* } */
/* } */
/* #ifdef __VALIDATE__ */
/* TestInvariant(); */
/* #endif */
/* return _nSize; */
/* } */
/*
template < class T >
ostream & operator << ( ostream & stream, const TIArrayExt < T > & tiArray )
{
   stream << tiArray.GetSize() << endl;
   for ( int i = _nLower; i < tiArray.GetSize() + _nLower; i++ )
   {
      if ( i ) stream << '\t';
      if ( tiArray[i] )
      { stream << * tiArray[i]; }
   }

   return stream;
}*/
#endif
