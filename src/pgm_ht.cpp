/*!
 *  @mainpage Tabela de Dispersão
 *  @authors Thiago Cesar Morais Diniz de Lucena
 *  @authors Yuri Reinaldo da Silva
 *  @date May, 2016
 *  @version Complete
 *
 *	@file pgm_ht.cpp
 *
 *  File with the testing program
 */
#include <iostream>
#include <functional>
#include <tuple>
#include <cmath>

#include "hashtbl.h"

using namespace std;

struct Account
{
    std::string mClientName;// (key) Client's name.
    int mBankCode;          // (key) Bank code.
    int mBranchCode;        // (key) Branch code.
    int mNumber;            // (key) Account number.
    float mBalance;         // Account balance.
    using AcctKey = std::tuple< std::string, int, int, int >;

    Account( std::string _Name = "<empty>",
             int _BankCode = 1,  // Banco do Brasil.
             int _BranchCode = 1668, // Agencia UFRN.
             int _Number = 0,
             float _Balance = 0.f )
        :  mClientName( _Name ),
           mBankCode( _BankCode ),
           mBranchCode( _BranchCode ), 
           mNumber( _Number ), 
           mBalance( _Balance )
        { /* Empty */ }

    /*!
     * \brief Gera a chave (versão 1) que eh igual a conta corrente.
     */
    AcctKey getKey () const
    {
        return std::make_tuple( mClientName, mBankCode, mBranchCode, mNumber );
    }

    const Account& operator= ( const Account& rhs )
    {
        // Avoid assigning to itself.
        if( this != &rhs )
        {
            // Copy new values.
            this->mClientName = rhs.mClientName;
            this->mBankCode   = rhs.mBankCode;
            this->mBranchCode = rhs.mBranchCode;
            this->mNumber     = rhs.mNumber;
            this->mBalance    = rhs.mBalance;
        }
        return *this;
    }

    //TODO friend para imprimir
    inline friend std::ostream &operator<< ( std::ostream& _os, const Account& _acc )
    {
        _os << "[ Client: <"  << _acc.mClientName <<
               "> Bank: <"    << _acc.mBankCode <<
               "> Branch: <"  << _acc.mBranchCode <<
               "> Number: <"  << _acc.mNumber <<
               "> Balance: <" << _acc.mBalance << "> ]";
        return _os;
    }
};


struct KeyHash {
    std::size_t operator()(const Account::AcctKey& k) const
    {
        return  std::hash<std::string>()( std::get<0>(k) ) 
            xor std::hash<int>()( std::get<1>(k) * pow(23, 1) )
            xor std::hash<int>()( std::get<2>(k) * pow(23, 2) ) 
            xor std::hash<int>()( std::get<3>(k) * pow(23, 3) );
    }
};
 
struct KeyEqual {
    bool operator()(const Account::AcctKey& lhs, const Account::AcctKey& rhs) const
    {
        return (std::get<0>(lhs) == std::get<0>(rhs) and
                std::get<1>(lhs) == std::get<1>(rhs) and
                std::get<2>(lhs) == std::get<2>(rhs) and
                std::get<3>(lhs) == std::get<3>(rhs));
    }
};


int main( void )
{
    MyHashTable::HashTbl< Account::AcctKey, Account, KeyHash, KeyEqual > accounts(2); // Hash table shall have size 23.
    Account MyAccts[] =
    {
        { "Jose_Silva",    1, 1668, 20123, 1500.f },
        { "Carlos_Prado",  1, 1668, 35091, 1250.f },
        { "Aline_Bastos", 13,   33, 55723,  500.f },
        { "Pedro_Gomes",   1, 1801, 87661, 5800.f },
        { "Julio_Gouveia",   12, 1709, 33678, 1300.f },
        { "Raissa_Meireles",   52, 8076, 94230, 7800.f }
    }; // Names contain _ to help test the remove() method.

    Account::AcctKey searchKey; // An account key
    auto nAccts = sizeof( MyAccts ) / sizeof( Account );

    for ( auto i(0u) ; i < nAccts ; ++i )
    {
        accounts.insert( MyAccts[i].getKey() , MyAccts[i] );
    }

    accounts.showStructure();
    
#define _NOT_NOW
#ifdef _NOT_NOW
    // Checks for accounts and prints records if found
    cout << endl;
    Account acct;
    std::string name;
    int bank_code;
    int bank_branch;
    int acc_number;
    char option;
    
    while ( 1 )
    {
        std::cout << "Do you wish to delete an account? y n\n";
        cin >> option;
        
        if( option == 'n' || option == 'N') break;
        
        cout << "Enter account data to be deleted\n\n";
        
        cout << "Enter accountant's name: ";
        cin >> name;
        cout << "Enter account's bank code: ";
        cin >> bank_code;
        cout << "Enter account's bank branch code: ";
        cin >> bank_branch;
        cout << "Enter account number: ";
        cin >> acc_number;
        
        searchKey = std::make_tuple( name, bank_code, bank_branch, acc_number);
        
        if ( accounts.retrieve( searchKey, acct ) )
        {
            cout << acct.mNumber << " " << acct.mBalance << endl;
            cout << "Removing this account....\n";
            accounts.remove( acct.getKey() );
        }
        else
            cout << "Account not found." << endl;

        accounts.showStructure();
    }
    
#endif

    std::cout << "\n>>> Normal exiting...\n";

    return EXIT_SUCCESS;
}