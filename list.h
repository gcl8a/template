/*
 * 16.07.21: added Pop() function to make FIFO queue
 * 12.05.16: added TListIterator::DeleteCurrent
 * 12.04.09: added GetFirst and GetLast()
 * 12.03.05: Add Jump() and JumpToFirst() methods
 * 12.03.04: added Find(string) method to list
 * 11/05/31: Add TSList, a sorted list
 * 11/05/31: general maintenance and upkeep -- it's been a while
 */

//TODO: Clean up Find and FindElement (which are now based too much on strings)

//modified 02/28/01

/*\template\list.h contains basic direct and indirect lists
Direct lists hold objects themselves, while indirect lists hold object pointers
Each list has a double linked list of elements, which hold the objects or ptrs.
*/

#ifndef __TEMPLATE_LIST_H
#define __TEMPLATE_LIST_H

#ifndef __TEMPLATE_XERROR_H
#include <template/xerror.h>
#endif

#include <string>

template < class T > class TList;
template < class T > class TListIterator;

template < class T > class TListElement
{
public:
	T _tData;
	TListElement < T > * _ptNext;
	TListElement < T > * _ptPrev;

protected:
	TListElement( void ) //: _tData(0)
	{ _ptNext = 0; _ptPrev = 0; }

	TListElement( const T & t ) : _tData( t )
	{ _ptNext = 0; _ptPrev = 0; }

	int TestInvariant( void ) const;

	friend class TList < T >;
	friend class TListIterator < T >;
};

template < class T > int TListElement < T >::TestInvariant(void) const {
    if (_ptNext) {
        if (_ptNext->_ptPrev != this) throw XError("Invariant error in TListElement!");
    }

    if (_ptPrev) {
        if (_ptPrev->_ptNext != this) throw XError("Invariant error in TListElement!");
    }

    return 0;
}

template < class T > class TList
{
protected:
	unsigned int _unItems;
	TListElement < T > * _ptHead;
	TListElement < T > * _ptTail;
protected:
	virtual T* AddElement( TListElement < T > * );
	virtual unsigned int DeleteElement( TListElement < T > * pt );
	TListElement < T > * FindElement( const string & s ) const;
	//TListElement < T > * FindElement( const T & ) const;

public:
	TList( void ) { _ptHead = _ptTail = 0; _unItems = 0; }
	TList( const TList < T > & tList );

	TList < T > & operator = ( const TList < T > & tList );

	virtual ~TList( void ) { Flush(); }

	T* Add( const T & t )
	{ return AddElement( new TListElement < T > ( t ) ); }

	T* GetHead(void) {if(_ptHead) return &_ptHead->_tData; else return NULL;}
	T* GetTail(void) {if(_ptTail) return &_ptTail->_tData; else return NULL;}
/*
	unsigned int Delete( const T & t )
	{
		TListElement < T > * ptElement = FindElement( t );
		if ( ptElement ) return DeleteElement( ptElement );
		else return -1;
	}
*/
	T * Find( const T & t ) const
	{
		TListElement < T > * ptElement = FindElement( t );
		if ( ptElement ) { return & ptElement->_tData; }
		else
			return 0;
	}

	T * Find( const string s ) const
	{
		TListElement < T > * ptElement = FindElement( s );
		if ( ptElement ) { return & ptElement->_tData; }
		else
			return 0;
	}
    
    void Pop(void) //removes head from the list
    {
        if(_ptHead)
        {
          DeleteElement(_ptHead);
        }
    }

    int Flush( void );
	int Destroy(void) {return Flush();}

	unsigned int GetItemsInContainer( void ) const { return _unItems; }
	unsigned int GetSize(void) {return GetItemsInContainer();}

	void TestInvariant( void ) const;
	void TestFullInvariant( void ) const;

	friend class TListIterator < T >;
};

template < class T > T* TList < T >::AddElement( TListElement < T > * ptElement )
/*protected member function to add an element, with appropriate updating
of prev, next, etc.
*/
{
	TestInvariant();

	if ( !ptElement )
		throw XError( "Error adding TListElement!" );

	if ( _ptHead == 0 )
	{
		_ptHead = _ptTail = ptElement;
		ptElement->_ptNext = 0;
		ptElement->_ptPrev = 0;
		++_unItems;

		return &ptElement->_tData;
	}

	else
	{
		_ptTail->_ptNext = ptElement;
		ptElement->_ptPrev = _ptTail;
		ptElement->_ptNext = 0;
		_ptTail = ptElement;
		++_unItems;

		return &ptElement->_tData;
	}
}

template<class T> unsigned int TList<T>::DeleteElement(TListElement<T> * ptElement)
{
	TestInvariant();

	if (!ptElement)	throw XError("Error in TList<T>::DeleteElement!");

	if (ptElement == _ptHead) {
		_ptHead = ptElement->_ptNext;
	}

	if (ptElement == _ptTail) {
		_ptTail = ptElement->_ptPrev;
	}

	if (ptElement->_ptPrev)
		ptElement->_ptPrev->_ptNext = ptElement->_ptNext;
	if (ptElement->_ptNext)
		ptElement->_ptNext->_ptPrev = ptElement->_ptPrev;

	delete ptElement;
	--_unItems;

	return 0;
}
/*
template < class T >
TListElement < T > * TList < T >::FindElement( const T & t ) const
//returns a pointer to the element if found
//returns NULL if not found
   {
   TListElement < T > * ptCurrent = _ptHead;
   while ( ptCurrent )
      {
      if ( ptCurrent->_tData == t ) return ptCurrent;
      ptCurrent = ptCurrent->_ptNext;
      }
   return 0;
   }
*/
template < class T >
TListElement < T > * TList < T >::FindElement( const string & s ) const
//returns a pointer to the element if found
//returns NULL if not found
   {
   TListElement < T > * ptCurrent = _ptHead;
   while ( ptCurrent )
      {
      if ( ptCurrent->_tData == s ) return ptCurrent;
      ptCurrent = ptCurrent->_ptNext;
      }
   return 0;
   }

template < class T >
TList < T >::TList( const TList < T > & tList )
   {
   _ptHead = 0;
   _ptTail = 0;
   _unItems = 0;

   TListElement < T > * ptCurrent = tList._ptHead;
   while ( ptCurrent )
      {
      Add( ptCurrent->_tData );
      ptCurrent = ptCurrent->_ptNext;
      }
   TestInvariant();
   }

template < class T >
TList < T > & TList < T >::operator = ( const TList < T > & tList )
   {
   _ptHead = 0;
   _ptTail = 0;
   _unItems = 0;

   TListElement < T > * ptCurrent = tList._ptHead;
   while ( ptCurrent )
      {
      Add( ptCurrent->_tData );
      ptCurrent = ptCurrent->_ptNext;
      }
   TestInvariant();
   return * this;
   }

template < class T >
int TList < T >::Flush( void )
{
	TListElement < T > * ptCurrent;
	while (( ptCurrent = _ptHead ))
	{ DeleteElement( ptCurrent ); }
	if ( _ptHead != 0 || _ptTail != 0 )
		throw XError( "Error flushing TList!" );

	TestInvariant();

	return _unItems;
}

template < class T >
void TList < T >::TestInvariant( void ) const
   {
   if ( _ptHead == 0 )
      {
      if ( _ptTail  || _unItems )
         throw XError( "Invariant error in TList!" );
      }
   else
      {
      if ( _ptHead == _ptTail && _unItems != 1 )
         throw XError( "Invariant error in TList!" );
      if ( _ptHead != _ptTail && _unItems <= 1 )
         throw XError( "Invariant error in TList!" );
      }
   }

template < class T >
void TList < T >::TestFullInvariant( void ) const
   {
   TestInvariant();
   if ( !_unItems )
      return;
   _ptHead->TestInvariant();
   _ptTail->TestInvariant();

   unsigned int unItems = 0;
   TListElement < T > * ptCurrent = _ptHead;
   if ( ptCurrent->_ptPrev ) throw XError( "Long invariant error in TList!" );
   while ( ptCurrent != _ptTail )
      {
      unItems++;
      ptCurrent->TestInvariant();
      ptCurrent = ptCurrent->_ptNext;
      }
   if ( ++unItems != _unItems ) throw XError( "Long invariant error in TList!" );
   if ( ptCurrent->_ptNext ) throw XError( "Long invariant error in TList!" );
   while ( --unItems )
      { ptCurrent = ptCurrent->_ptPrev; }
   if ( ptCurrent != _ptHead ) throw XError( "Long invariant error in TList!" );
}

template < class T > class TListIterator {
private:
protected:
    TListElement < T > * _ptCurrent;
    TList < T > * _ptList;

public:

    TListIterator(TList < T > & tList) {
        _ptList = &tList;
        Restart();
    }

    operator int() {
        return _ptCurrent ? 1 : 0;
    }

    T * Restart(void) {
        _ptCurrent = _ptList->_ptHead;
        return Current();
    }

    T * Last(void) {
        _ptCurrent = _ptList->_ptTail;
        return Current();
    }

    T * Current(void) {
        if (_ptCurrent) return & _ptCurrent->_tData;
        else return NULL;
    }

    unsigned int DeleteCurrent(void)
    /*
     * deletes the current element and moves current to the next element
     */
	{
		if (_ptCurrent)
		{
			TListElement < T > * next = _ptCurrent->_ptNext;
			_ptList->DeleteElement(_ptCurrent);
			_ptCurrent = next;
                        
            return 0;
		}
		else
            return -1;
	}

    T * operator ++(int) {
        if (_ptCurrent) {
            TListElement < T > * current = _ptCurrent;
            _ptCurrent = _ptCurrent->_ptNext;
            return & current->_tData;
        }
        
        else return 0;
    }
    
    T* Jump(int jump_step) {
        if(jump_step >0)
            for(int i = 0; i < jump_step; i++)
                if(_ptCurrent) _ptCurrent = _ptCurrent->_ptNext;
                   
        return (operator ++());
    }

    T* JumpToFirst(const T& t) { //quickly finds the first occurrence 
        Restart();
        while (_ptCurrent) {
            if (t > *_ptCurrent->_tData)
                return _ptCurrent->_tData;
            _ptCurrent = _ptCurrent->_ptNext;
        }

        return NULL;
    }

};

template < class T > class TSList : public TList<T>
{
public:
        virtual T* AddElement( TListElement < T > * );
        virtual T* FindFirst(const T&);
};

template < class T > T* TSList < T >::AddElement(TListElement < T > * ptElement)
/*protected member function to add an element, with appropriate updating
of prev, next, etc. SORTED!
 */ {
    TList<T>::TestInvariant();

    if (!ptElement)
        throw XError("Error adding element in TSList::AddElement!");

    //start at the END and work backwards
    TListElement < T > * current = TList<T>::_ptTail;
    while (current) {
        if (ptElement->_tData > current->_tData) {
            if (current == TList<T>::_ptTail) TList<T>::_ptTail = ptElement;

            if (current -> _ptNext) current -> _ptNext -> _ptPrev = ptElement;
            ptElement -> _ptNext = current -> _ptNext;

            current -> _ptNext = ptElement;
            ptElement->_ptPrev = current;

            ++TList<T>::_unItems;

            return &ptElement->_tData;
        }

        current = current->_ptPrev;
    }

    //if we've made it here, then add before the head
    if (TList<T>::_ptHead == 0) {
        TList<T>::_ptHead = TList<T>::_ptTail = ptElement;

        ptElement->_ptNext = 0;
        ptElement->_ptPrev = 0;
        ++TList<T>::_unItems;

        return &ptElement->_tData;
    }
    else {
        ptElement->_ptNext = TList<T>::_ptHead;
        ptElement->_ptPrev = 0;

        TList<T>::_ptHead->_ptPrev = ptElement;
        TList<T>::_ptHead = ptElement;

        ++TList<T>::_unItems;

        return &ptElement->_tData;
    }
}

template < class T > T* TSList < T >::FindFirst(const T& t) {
    TListElement < T > * ptCurrent = TList<T>::_ptHead;
    while (ptCurrent) {
        if (t > ptCurrent->_tData)
            return &ptCurrent->_tData;
        ptCurrent = ptCurrent->_ptNext;
    }

    return NULL;
}

template < class T > class TIList;
template < class T > class TIListIterator;


template < class T > class TIListElement
/*TIListElement implements a list element for storing indirect lists
Note that the element holds a pointer to T but does not delete it
automatically upon destruction.  Also, copying is done of the pointer only,
not the underlying object
*/
   {
private:
   T * _ptData;
   TIListElement < T > * _ptNext;
   TIListElement < T > * _ptPrev;

protected:
   TIListElement( void )
      { _ptNext = 0; _ptPrev = 0; _ptData = 0; }

   TIListElement( T * pt )
   : _ptData( pt )
      { _ptNext = 0; _ptPrev = 0; }

   TIListElement( const TIListElement & tElement )
   : _ptData( tElement._ptData ) //copies address, not contents
      {
      _ptPrev = 0;
      _ptNext = 0;
      }

   TIListElement & operator = ( const TIListElement & tElement )
      {
      _ptData = tElement._ptData; //copies address, not contents
      _ptPrev = 0;
      _ptNext = 0;
      return * this;
      }

   ~TIListElement( void ) //NOTE! NO AUTOMATIC DELETE!
      { }

   void TestInvariant( void ) const;

   friend class TIList < T >;
   friend class TIListIterator < T >;
   };


template < class T >
void TIListElement < T >::TestInvariant( void ) const
   {
   if ( _ptNext )
      { if ( _ptNext->_ptPrev != this ) throw XError( "Invariant error in TIListElement!" ); }
   if ( _ptPrev )
      { if ( _ptPrev->_ptNext != this ) throw XError( "Invariant error in TIListElement!" ); }
   if ( !_ptData ) throw XError( "Invariant error in TIListElement!" );
   }


template < class T >
class TIList
/*Implements an indirect list
*/
   {
private:
   unsigned int _unItems;
   TIListElement < T > * _ptHead;
   TIListElement < T > * _ptTail;
protected:
   unsigned int AddElement( TIListElement < T > * );
   unsigned int DetachElement( TIListElement < T > * );
   unsigned int DeleteElement( TIListElement < T > * );

   TIListElement < T > * FindElement( const T * pt ) const;				//compares addresses
   TIListElement < T > * FindElement( const T & t ) const;            //compares contents
   TIListElement < T > * FindElement( const string & s ) const;            //compares string
   TIListElement < T > * FindOppositeElement( const T & t ) const;   //requires ^ operator

   friend class TIListIterator < T >;

public:
   TIList( void ) { _ptHead = _ptTail = 0; _unItems = 0; }
   TIList( const TIList < T > & tList );

   TIList < T > & operator = ( const TIList < T > & tList );

   ~TIList( void ) { Flush(); }

   unsigned int Add( const TIList < T > & );

   unsigned int Add(T * pt)
	{
#ifdef __VALIDATE__
		TestInvariant();
#endif
		return AddElement(new TIListElement<T> (pt));
	}

   unsigned int Detach( const T * pt )
      {
      TIListElement < T > * ptiElement = FindElement( pt );
      if ( ptiElement ) return DetachElement( ptiElement );
      else
         return 0;
      }

   unsigned int Detach( const T & t )
      {
      TIListElement < T > * ptiElement = FindElement( t );
      if ( ptiElement ) return DetachElement( ptiElement );
      else
         return 0;
      }

   unsigned int DetachOpposite( const T & t )
      {
      TIListElement < T > * ptiElement = FindOppositeElement( t );
      if ( ptiElement ) return DetachElement( ptiElement );
      else
         return 0;
      }

   unsigned int Delete( const T * pt )
      {
      TIListElement < T > * ptiElement = FindElement( pt );
      if ( ptiElement ) return DeleteElement( ptiElement );
      else
         return 0;
      }

   unsigned int Delete( const T & t )
      {
      TIListElement < T > * ptiElement = FindElement( t );
      if ( ptiElement ) return DeleteElement( ptiElement );
      else
         return 0;
      }

   T * Find( const T * pt )
      {
      TIListElement < T > * ptElement = FindElement( pt );
      if ( ptElement ) { return ptElement->_ptData; }
      else
         return 0;
      }

   T * Find( const T & t )
      {
      TIListElement < T > * ptElement = FindElement( t );
      if ( ptElement ) { return ptElement->_ptData; }
      else
         return 0;
      }

   T * Find( const string & s )
      {
      TIListElement < T > * ptElement = FindElement( s );
      if ( ptElement ) { return ptElement->_ptData; }
      else
         return 0;
      }

   T * FindOpposite( const T & t )
      {
      TIListElement < T > * ptElement = FindOppositeElement( t );
      if ( ptElement ) { return ptElement->_ptData; }
      else
         return 0;
      }

   T * GetFirst( void ) { return _ptHead->_ptData; }

   T * GetRandomItem( void );

   int Flush( void );
   int Purge( void );

   int Destroy( void ) { return Purge(); }

   unsigned int GetItemsInContainer( void ) const { return _unItems; }

   void TestInvariant( void ) const;
   void TestFullInvariant( void ) const;
   };


template < class T >
TIListElement < T > * TIList < T >::FindElement( const T * pt ) const
//returns a pointer to the element if found using pointer comparison
//returns NULL if not found
   {
   TIListElement < T > * ptCurrent = _ptHead;
   while ( ptCurrent )
      {
      if ( ptCurrent->_ptData == pt ) return ptCurrent;
      ptCurrent = ptCurrent->_ptNext;
      }
   return 0;
   }

template < class T >
TIListElement < T > * TIList < T >::FindElement( const T & t ) const
//returns a pointer to the element if found using value comparison
//returns NULL if not found
   {
   TIListElement < T > * ptCurrent = _ptHead;
   while ( ptCurrent )
      {
      if ( * (ptCurrent->_ptData) == t ) return ptCurrent;
      ptCurrent = ptCurrent->_ptNext;
      }
   return 0;
   }

template < class T >
TIListElement < T > * TIList < T >::FindElement( const string & s ) const
//returns a pointer to the element if found using value comparison
//returns NULL if not found
   {
   TIListElement < T > * ptCurrent = _ptHead;
   while ( ptCurrent )
      {
      if ( * (ptCurrent->_ptData) == s ) return ptCurrent;
      ptCurrent = ptCurrent->_ptNext;
      }
   return 0;
   }

template < class T >
TIListElement < T > * TIList < T >::FindOppositeElement( const T & t ) const
//returns a pointer to the element if found using ^ operator
//returns NULL if not found
   {
   TIListElement < T > * ptCurrent = _ptHead;
   while ( ptCurrent )
      {
      if ( * ptCurrent->_ptData ^ t ) return ptCurrent;
      ptCurrent = ptCurrent->_ptNext;
      }
   return 0;
   }

template < class T >
unsigned int TIList < T >::AddElement( TIListElement < T > * ptElement )
   {
   TestInvariant();
   if ( !ptElement )
      throw XError( "Error in TIList<T>::AddElement!" );
   if ( _ptHead == 0 )
      {
      _ptHead = _ptTail = ptElement;
      ptElement->_ptNext = 0;
      ptElement->_ptPrev = 0;
      return ++_unItems;
      }
   else
      {
      _ptTail->_ptNext = ptElement;
      ptElement->_ptPrev = _ptTail;
      ptElement->_ptNext = 0;
      _ptTail = ptElement;
      return ++_unItems;
      }
   }

template < class T >
unsigned int TIList < T >::DetachElement( TIListElement < T > * ptElement )
//remove data without deleting pointer
   {
   TestInvariant();
   if ( ptElement == _ptHead ) { _ptHead = ptElement->_ptNext; }
   if ( ptElement == _ptTail ) { _ptTail = ptElement->_ptPrev; }
   if ( ptElement )
      {
      if ( ptElement->_ptPrev )
         ptElement->_ptPrev->_ptNext = ptElement->_ptNext;
      if ( ptElement->_ptNext )
         ptElement->_ptNext->_ptPrev = ptElement->_ptPrev;
      delete ptElement;
      --_unItems;
      return 1;
      }
   else
      throw XError( "Error in TIList<T>::DetachElement!" );
   }

template < class T >
unsigned int TIList < T >::DeleteElement( TIListElement < T > * ptElement )
//removes data, deleting pointer and freeing memory
   {
   TestInvariant();
   if ( ptElement == _ptHead ) { _ptHead = ptElement->_ptNext; }
   if ( ptElement == _ptTail ) { _ptTail = ptElement->_ptPrev; }
   if ( ptElement )
      {
      if ( ptElement->_ptPrev )
         ptElement->_ptPrev->_ptNext = ptElement->_ptNext;
      if ( ptElement->_ptNext )
         ptElement->_ptNext->_ptPrev = ptElement->_ptPrev;
      delete ptElement->_ptData;
      delete ptElement;
      --_unItems;
      return 1;
      }
   else
      return 0;
   }

template < class T >
TIList < T >::TIList( const TIList < T > & tiList )
   {
   _ptHead = 0;
   _ptTail = 0;
   _unItems = 0;

   TIListElement < T > * ptCurrent = tiList._ptHead;
   while ( ptCurrent )
      {
      Add( ptCurrent->_ptData );
      ptCurrent = ptCurrent->_ptNext;
      }
   TestInvariant();
   }

template < class T >
TIList < T > & TIList < T >::operator = ( const TIList < T > & tiList )
   {
   _ptHead = 0;
   _ptTail = 0;
   _unItems = 0;

   TIListElement < T > * ptCurrent = tiList._ptHead;
   while ( ptCurrent )
      {
      Add( ptCurrent->_ptData );
      ptCurrent = ptCurrent->_ptNext;
      }
   TestInvariant();
   return * this;
   }

template < class T >
unsigned int TIList < T >::Add( const TIList < T > & tiList )
/*adds all of the pointers from one list to another
*/
   {
   TIListElement < T > * ptCurrent = tiList._ptHead;
   while ( ptCurrent )
      {
      Add( ptCurrent->_ptData );
      ptCurrent = ptCurrent->_ptNext;
      }
   TestInvariant();
   return _unItems;
   }

template < class T >
int TIList < T >::Flush( void )
//flushes without deleting pointers
   {
   TIListElement < T > * ptCurrent = _ptHead;
   while ( ptCurrent )
      {
      DetachElement( ptCurrent );
      ptCurrent = _ptHead;
      }
   if ( _ptHead != 0 || _ptTail != 0 )
      throw XError( "Error flushing TIList!" );
   TestInvariant();
   return 1;
   }

template < class T >
int TIList < T >::Purge( void )
//flushes and deletes pointers
   {
   TIListElement < T > * ptCurrent = _ptHead;
   while ( ptCurrent )
      {
      DeleteElement( ptCurrent );
      ptCurrent = _ptHead;
      }
   if ( _ptHead != 0 || _ptTail != 0 )
      throw XError( "Error flushing TIList!" );
   TestInvariant();
   return 1;
   }

template < class T >
T * TIList < T >::GetRandomItem( void )
   {
   if ( !GetItemsInContainer() )
      return 0;
   int nRandom = random( GetItemsInContainer() );
   TIListElement < T > * ptCurrent = _ptHead;
   while ( nRandom-- )
      { ptCurrent = ptCurrent->_ptNext; }
   return ptCurrent->_ptData;
   }

template < class T >
void TIList < T >::TestInvariant( void ) const
   {
   if ( _ptHead == 0 )
      {
      if ( _ptTail  || _unItems )
         throw XError( "Invariant error in TIList!" );
      }
   else
      {
      if ( _ptHead == _ptTail && _unItems != 1 )
         throw XError( "Invariant error in TIList!" );
      if ( _ptHead != _ptTail && _unItems <= 1 )
         throw XError( "Invariant error in TIList!" );
      }
   }

template < class T >
void TIList < T >::TestFullInvariant( void ) const
   {
   TestInvariant();
   if ( !_unItems )
      return;
   _ptHead->TestInvariant();
   _ptTail->TestInvariant();

   unsigned int unItems = 0;
   TIListElement < T > * ptCurrent = _ptHead;
   if ( ptCurrent->_ptPrev ) throw XError( "Long invariant error in TIList!" );
   while ( ptCurrent != _ptTail )
      {
      unItems++;
      ptCurrent->TestInvariant();
      ptCurrent = ptCurrent->_ptNext;
      }
   if ( ++unItems != _unItems ) throw XError( "Long invariant error in TIList!" );
   if ( ptCurrent->_ptNext ) throw XError( "Long invariant error in TIList!" );
   while ( --unItems )
      { ptCurrent = ptCurrent->_ptPrev; }
   if ( ptCurrent != _ptHead ) throw XError( "Long invariant error in TIList!" );
   }


template < class T >
class TIListIterator
   {
private:
   protected :
   TIListElement < T > * _ptiCurrent;
   TIList < T > * _ptiList;

public:
   TIListIterator( TIList < T > & tiList )
      {
      _ptiList = & tiList;
      Restart();
      }

   T * Restart( void )
      {
      _ptiCurrent = _ptiList->_ptHead;
      return _ptiCurrent ? _ptiCurrent->_ptData : 0;
      }

   T * Current( void )
      { return _ptiCurrent ? _ptiCurrent->_ptData : 0; }

   T * operator ++ ( int )
      {
      if ( _ptiCurrent )
         {
	 T* curr = _ptiCurrent->_ptData;
         _ptiCurrent = _ptiCurrent->_ptNext;
         return curr;
         }
      else
         return 0;
      }
   };

#endif
