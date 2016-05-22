#include <iostream>

#include <cmath>
#include "hashtbl.h"




//! MyHashTable namespace encapsulates all class related to a simple hash function definition.
namespace MyHashTable {

    //----------------------------------------------------------------------------------------
    //! Default construtor.
    /*! Creates a hash table of the required capacity, which uses an external hash function
     *  that maps keys to unsigned long integers.
     *  If no external hash function is provided, an \r UndefinedHashFunctionException is generated.
     *  \param _initSize Required hash table capacity.
     *  \param _pfHF Pointer to an external hash function that does the first hashing and returns an unsigned long int.
     *  \throw UndefinedHashFunctionException if no external hash function is provided.
    */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    HashTbl< KeyType, DataType, KeyHash, KeyEqual >::HashTbl ( int _initSize )
        : mSize( nextPrime(_initSize) ), mCount( 0u )
    {
        mpDataTable = new std::list<Entry> [mSize];
    }

    //----------------------------------------------------------------------------------------
    //! Destrutor that just frees the table memory, clearing all collision lists.
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    HashTbl< KeyType, DataType, KeyHash, KeyEqual >::~HashTbl ()
    {
        for(auto i (0u); i < mSize; ++i)
        {
            mpDataTable[i].~list();
        }
        mSize = 0;
    }

    //----------------------------------------------------------------------------------------
    //! Inserts data into the hash table.
    /*! For an insertion to occur, the client code should provide a key and the data itself
     *  If the data is already stored in the table, the function updates the data with the
     *  new information provided.
     *  \param _newKey Key associated with the data, used to get to the stored information.
     *  \param _newDataItem Data to be stored or updated, in case the information is already stored in the hash table.
     *  \return false if the data is already stored in the table and it is updated; true, otherwise.
     *  \throw std::bad_alloc In case no memory is available for dynamic allocation required in the insertion procedure.
    */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    bool HashTbl< KeyType, DataType, KeyHash, KeyEqual >::insert ( const KeyType & _newKey, const DataType & _newDataItem ) throw ( std::bad_alloc )
    {
        // Flag used to indicate whether the data has been found or not.
        bool bDataFound( false );
        // Instatiation of Hash function functor
        KeyHash hashFunction;
        // Instantiation of equal function functor
        KeyEqual equalFunction;
        // Entry to be inserted
        Entry newEntry(_newKey, _newDataItem);
        // Table position
        auto end ( hashFunction(_newKey) % mSize);
        // Iterator that will help us run through the list
        auto it = mpDataTable[end].begin();
        // Runs through the list, searching for an element with the same key
        while( it != mpDataTable[end].end() )
        {
            if( true == equalFunction(it->mKey, _newKey) ) 
            {    
                it->mData = _newDataItem;
                return bDataFound;
            }
            it++;                    
        }
        
        // Checks whether rehash needs to be done;
        if(mCount + 1 == mSize )
        {
            rehash();
            end = hashFunction(_newKey) % mSize;
        }   
        
        // Inserts the new element at the beginning of the list
        mpDataTable[end].push_front(newEntry);
        bDataFound = true;
        mCount++;
        
        return bDataFound;
    }


    //----------------------------------------------------------------------------------------
    //! Removes data from the hash table.
    /*! Removes a data item from the table, based on the key associated with the data.
     *  If the data cannot be found, false is returned; otherwise, true is returned instead.
     *  \param _searchKey Data key to search for in the table.
     *  \return true if the data item is found; false, otherwise.
    */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    bool HashTbl< KeyType, DataType, KeyHash, KeyEqual >::remove ( const KeyType & _searchKey )
    {
        // Flag used to indicate if the element was removed or not
        auto bRemoved( false );
        // Instatiating of Hash function functor
        KeyHash hashFunction;
        // Instantiation of equal function functor
        KeyEqual equalFunction;
        // Hashing the key
        auto end ( hashFunction(_searchKey) % mSize);
        // Iterator that will help us run through the list
        auto it = mpDataTable[end].begin();
        // Runs through the list, searching for an element with the same key
        while(it != mpDataTable[end].end())
        {
            if(true == equalFunction(it->mKey, _searchKey))
            {
                mpDataTable[end].erase(it);
                mCount--;
                bRemoved = true;
                return bRemoved;
            }
            it++;
        }
        
        return bRemoved;
    }

    //----------------------------------------------------------------------------------------
    //! Retrieves data from the table.
    /*! Retrieves a data item from the table, based on the key associated with the data.
     *  If the data cannot be found, false is returned; otherwise, true is returned instead.
     *  \param _searchKey Data key to search for in the table.
     *  \param _dataItem Data record to be filled in when data item is found.
     *  \return true if the data item is found; false, otherwise.
    */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    bool HashTbl< KeyType, DataType, KeyHash, KeyEqual >::retrieve ( const KeyType & _searchKey, DataType & _dataItem ) const
    {
        // Flag that points whether the received key was found
        auto bFound( false );
        // Instatiating of Hash function functor
        KeyHash hashFunction;
        // Instantiation of equal function functor
        KeyEqual equalFunction;
        // Hashing the key
        auto end ( hashFunction(_searchKey) % mSize);
        // Iterator that will help us run through the list
        auto it = mpDataTable[end].begin();
        // Runs through the list, searching for an element with the same key
        while(it != mpDataTable[end].end())
        {
            if(true == equalFunction(it->mKey, _searchKey))
            {
                _dataItem = it->mData;
                bFound = true;
                break;
            }
            it++;
        }
        
        return bFound;
    }

    //! Clears the data table.
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    void HashTbl< KeyType, DataType, KeyHash, KeyEqual >::clear ()
    {
        for(auto _list : mpDataTable)
        {
            _list.erase(_list.begin(), _list.end());
        }
        mCount = 0;
    }

    //! Tests whether the table is empty.
    /*!
     * \return true is table is empty, false otherwise.
     */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    bool HashTbl< KeyType, DataType, KeyHash, KeyEqual >::isEmpty () const
    {
        return ( mCount == 0 );
    }

    //! Counts the number of elements currently stored in the table.
    /*!
     * \return The current number of elements in the table.
     */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    unsigned long int HashTbl< KeyType, DataType, KeyHash, KeyEqual >::count () const
    {
        return mCount;
    }

    //! Prints out the hash table content.
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    void HashTbl< KeyType, DataType, KeyHash, KeyEqual >::showStructure () const
    {
        KeyHash hashFn;

        // Traverse the list associated with the based address (idx), calculated before.
        for( auto i(0) ; i < mSize; ++i )
        {
            std::cout << i << " :{ key=";
            for( auto & e : mpDataTable[ i ] )
            {
                std::cout << hashFn( e.mKey ) << " ; " << e.mData << " " ;
            }
            std::cout << "}\n";
        }
    }
    
    /*! @brief Doubles the size of the table
     *
     * 
     */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    void HashTbl< KeyType, DataType, KeyHash, KeyEqual >::rehash ()
    {
        // Size after rehashing
        unsigned int newSize = nextPrime(mSize * 2);
        // New table dinamically allocated
        std::list< Entry > *newTable = new std::list< Entry > [newSize];
        // Instatiating of Hash function functor
        KeyHash hashFunction;
        
        for( auto i(0u); i < mSize; i++)
        {
            if(mpDataTable[i].empty())
            {
                continue;
            }
            
            auto it = mpDataTable[i].begin();
            
            // 
            while(it != mpDataTable[i].end())
            {
                
                Entry temp(it->mKey, it->mData);
                auto end (hashFunction(temp.mKey) % newSize);
                newTable[end].push_front(temp);
                it++;
            }
            
        }
        
        
        this->~HashTbl();
        mpDataTable = newTable;
        mSize = newSize;
        
    }
    
    /*! @brief Returns next prime number of a given number.
     *  @param _size Number given.
     *  @return Next prime number, or the number itself (if it is a prime number).
     */
    template < typename KeyType, typename DataType, typename KeyHash, typename KeyEqual >
    unsigned int HashTbl< KeyType, DataType, KeyHash, KeyEqual >::nextPrime (unsigned int _size)
    {
        int divisors = 0;

        while(1)
        {
            for( auto i(2u); i <= sqrt(_size); i++)
            {
                if ( _size % i == 0 )
                    divisors++;
                
                if( divisors != 0)
                    break; 
            }
            
            if( divisors == 0)
                return _size;
            
            _size++;   
            divisors = 0;
        }
    }

} // namespace MyHashTable
