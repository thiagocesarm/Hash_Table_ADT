## Project Hash Table ADT

##### Version 1.0.0 - Monday May 23rd 2016

by Thiago César Morais Diniz de Lucena & Yuri Reinaldo da Silva

Description
--------------------------------------------------------------------------------------

This project implements a Hash Table abstract data type using a dynamic array of 
doubly linked lists. The context used to exemplify the hash table use is a 
system of bank accounts. Each bank account consists of the client's name, bank 
code, branch code, account's number and actual balance. 

The key used in the hash function to identify each account entry is a tuple 
consisting of the first four attributes mentioned above. 

The process of mapping each entry to a position in the table is made by double hashing.
When the load factor of the table reachs 1, then rehashing is used to increase 
table's size.


Instalation and running
--------------------------------------------------------------------------------------

To install and execute the program, follow these steps: 

1. The file must be extracted from the accompanying zip folder 
  "Hash-Table-ADT.zip", it should come with all the files necessary.
2. Open your terminal and navigate to the folder where the files where extracted
   to.
3. Simply input the __make__ command.
..* Instead of make, another way to do it is by inputting the following command 
	at the terminal:

g++ -std=c++11 -I include src/pgm\_ht.cpp -o bin/drive_hashtable


Then, to execute the program, you just run it by inputting the following command:
	
	bin/drive_hashtable

Using the hash table
--------------------------------------------------------------------------------------

In order to initiate a hash table you must declare it using the following 
structure:

MyHashTable::HashTbl< _keyType_, _dataType_, _hashFunction_, _equalFunction_ >
_tableName_ _(tableSize)_

Those names that are _highlighted_ should be substituted by their equivalents.
The first two (keyType and dataType) must always be informed, the last two are
optional if the keyType is a basic type (such as int, string, char...). 
(tableSize) is also optional, and must be an integer.

These are the following methods that can be used within the table:

##### bool insert ( const KeyType &, const DataType & );
Inserts the element passed by it's key and data at the appropriate location,
returns true if no element with the same key was already in the table, false
otherwise


##### bool remove ( const KeyType & );
Removes from the table the element with the corresponding key, returns true if 
the removal was successful, false if no element with the key could be found 

##### bool retrieve ( const KeyType &, DataType & ) const;
Searches if there is an element with the corresponding key, if it's found, sets 
the data variable passed as an argument with the data found and returns true. If 
nothing was found false is returned.

##### void clear ( void );
Wipes the whole table but keeps the same size.

##### bool isEmpty ( void ) const;
Checks whether there is any element wihthin the table, returns true if it is 
empty, returns false otherwise.

##### unsigned long int count ( void ) const;
Returns the amount of elements within the table.

##### void showStructure () const;
Prints the table contents on the terminal, showing the data inside each element
and their keys.