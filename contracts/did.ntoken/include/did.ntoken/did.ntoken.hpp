#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>
#include <eosio/permission.hpp>

#include <string>

#include <did.ntoken/did.ntoken.db.hpp>

namespace flon {

using std::string;
using std::vector;

using namespace eosio;

/**
 * The `did.ntoken` sample system contract defines the structures and actions that allow users to create, issue, and manage tokens for FLON based blockchains. It demonstrates one way to implement a smart contract which allows for creation and management of tokens. It is possible for one to create a similar contract which suits different needs. However, it is recommended that if one only needs a token with the below listed actions, that one uses the `did.ntoken` contract instead of developing their own.
 *
 * The `did.ntoken` contract class also implements two useful public static methods: `get_supply` and `get_balance`. The first allows one to check the total supply of a specified token, created by an account and the second allows one to check the balance of a token for a specified account (the token creator account has to be specified as well).
 *
 * The `did.ntoken` contract manages the set of tokens, accounts and their corresponding balances, by using two internal multi-index structures: the `accounts` and `stats`. The `accounts` multi-index table holds, for each row, instances of `account` object and the `account` object holds information about the balance of one token. The `accounts` table is scoped to an eosio account, and it keeps the rows indexed based on the token's symbol.  This means that when one queries the `accounts` multi-index table for an account name the result is all the tokens that account holds at the moment.
 *
 * Similarly, the `stats` multi-index table, holds instances of `currency_stats` objects for each row, which contains information about current supply, maximum supply, and the creator account for a symbol token. The `stats` table is scoped to the token symbol.  Therefore, when one queries the `stats` table for a token symbol the result is one single entry/row corresponding to the queried symbol token if it was previously created, or nothing, otherwise.
 */
class [[eosio::contract("did.ntoken")]] didtoken : public contract {
   public:
      using contract::contract;

   didtoken(eosio::name receiver, eosio::name code, datastream<const char*> ds): contract(receiver, code, ds),
        _global(get_self(), get_self().value)
    {
        _gstate = _global.exists() ? _global.get() : global_t{};
    }

    ~didtoken() { _global.set( _gstate, get_self() ); }

   /**
    * @brief Allows `issuer` account to create a token in supply of `maximum_supply`. If validation is successful a new entry in statsta
    *
    * @param issuer  - the account that creates the token
    * @param maximum_supply - the maximum supply set for the token created
    * @return ACTION
    */
   ACTION create( const name& issuer, const int64_t& maximum_supply, const nsymbol& symbol, const string& token_uri, const name& ipowner );

   /**
    * @brief This action issues to `to` account a `quantity` of tokens.
    *
    * @param to - the account to issue tokens to, it must be the same as the issuer,
    * @param quntity - the amount of tokens to be issued,
    * @memo - the memo string that accompanies the token issue transaction.
    */
   ACTION issue( const name& to, const nasset& quantity, const string& memo );

   ACTION retire( const nasset& quantity, const string& memo );

   ACTION burn( const name& owner,const nasset& quantity, const string& memo );
   
   /*
    * For those who are disqualified, their DID must be claimed back
    */
   ACTION reclaim( const name& target, const nsymbol& did, const string& memo );

	/**
	 * @brief Transfers one or more assets.
	 *
    * This action transfers one or more assets by changing scope.
    * Sender's RAM will be charged to transfer asset.
    * Transfer will fail if asset is offered for claim or is delegated.
    *
    * @param from is account who sends the asset.
    * @param to is account of receiver.
    * @param assetids is array of assetid's to transfer.
    * @param memo is transfers comment.
    * @return no return value.
    */
   ACTION transfer( const name& from, const name& to, const vector<nasset>& assets, const string& memo );
   using transfer_action = action_wrapper< "transfer"_n, &didtoken::transfer >;

   /**
    * @brief fragment a NFT into multiple common or unique NFT pieces
    *
    * @return ACTION
    */
   // ACTION fragment();

   ACTION settokenuri(const uint64_t& symbid, const string& url);

   ACTION setnotary(const name& notary, const bool& to_add);
   /**
    * @brief notary to notarize a NFT asset by its token ID
    *
    * @param notary
    * @param token_id
    * @return ACTION
    */
   ACTION notarize(const name& notary, const uint32_t& token_id);


   ACTION setacctperms(const name& issuer, const name& to, const nsymbol& symbol,  const bool& allowsend, const bool& allowrecv);


   private:
      void add_balance( const name& owner, const nasset& value, const name& ram_payer );
      void sub_balance( const name& owner, const nasset& value );

      inline void require_issuer(const name& issuer, const nsymbol& sym) {
         nstats_t::idx_t tokenstats( get_self(), sym.raw() );
         auto existing = tokenstats.find( sym.raw() );
         check( existing != tokenstats.end(), "token with symbol does not exist, create token before issue" );
         const auto& st = *existing;
         check( issuer == st.issuer, "can only be executed by issuer account" );
      }
   private:
      global_singleton    _global;
      global_t            _gstate;
};
} //namespace flon
