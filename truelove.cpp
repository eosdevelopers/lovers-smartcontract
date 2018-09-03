
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/types.h>
#include <eosiolib/crypto.h>
#include <eosiolib/transaction.h>
// #include <fc/crypto/hex.hpp>
#include "./common.hpp"
#include <string>

using std::string;
using namespace eosio;
//所有智能合约继承contract类
class truelove : public eosio::contract {
  public:
      //love类构造函数
      using contract::contract;
      //@abi action 成员函数声明
      // void say(account_name lover, string letter) {
      //   //验证权限
      //   require_auth( _self );
      //   // 定义loves类型
      //   print("_self:", name{_self}, ";", _self);
      //   // records lovetable( _self, _self);
      //   // //查询lover是否存在， 如果存在则assert
      //   // auto exit = lovetable.find(lover);
      //   // eosio_assert(exit == lovetable.end(), "You love has been said!");
      //   // //构造loves
      //   // lovetable.emplace( _self, [&]( auto& s ) {
      //   //     s.lover = lover;
      //   //     s.letter = letter;
      //   // });
      // }

      // @abi action
      void transfer(account_name sender, account_name receiver, asset quanity, string memo) {
        // print("transfer:", name{sender}, "=>", name{receiver}, ": ", quanity, ";   memo:", memo);
        auto sym = quanity.symbol.value;
        // print("sym:", sym, "==", S(4, EOS));
        if (sym != S(4, EOS)) {
          return;
        }
        size_t tx_size = transaction_size();
        char buff[tx_size];
        size_t read = read_transaction(buff, tx_size);
        checksum256 h;
        sha256(buff, read, &h);
        string txHash = comm::to_hex((const char*)(&h), sizeof(h));
        // 
        
        records  lovetable( _self, _self);
        //构造loves
        lovetable.emplace( _self, [&]( auto& s ) {
          s.id = lovetable.available_primary_key();
          s.sender = sender;
          s.txHash = txHash;
          s.letter = memo;
          s.amount = quanity.amount;//(uint64_t)(quanity.amount * 10000);
        });
        // print("done");
      }

  private:
      // @abi table 私有数据成员声明
      struct lover {
        uint64_t        id; //auto increment id
        int64_t        amount;
        account_name    sender;
        string          txHash;    
        string          letter;
        
        //私有函数成员
        account_name primary_key()const { return id; }
        EOSLIB_SERIALIZE( lover, (id)(amount)(sender)(txHash)(letter) )
      };
      //数据库类型定义
      typedef eosio::multi_index<N(lover), lover> records;
};


/** 
 * Convenient macro to create contract apply handler
 * To be able to use this macro, the contract needs to be derived from eosio::contract
 * 
 * @brief Convenient macro to create contract apply handler 
 * @param TYPE - The class name of the contract
 * @param MEMBERS - The sequence of available actions supported by this contract
 * 
 * Example:
 * @code
 * EOSIO_ABI( eosio::bios, (setpriv)(setalimits)(setglimits)(setprods)(reqauth) )
 * @endcode
 */
#define EOSIO_ABI_EX( TYPE, MEMBERS ) \
extern "C" { \
   void apply( uint64_t receiver, uint64_t code, uint64_t action ) { \
      auto self = receiver; \
      if( action == N(onerror)) { \
         /* onerror is only valid if it is for the "eosio" code account and authorized by "eosio"'s "active permission */ \
         eosio_assert(code == N(eosio), "onerror action's are only valid from the \"eosio\" system account"); \
      } \
      if( code == self || code == N(eosio.token) || action == N(onerror) ) { \
         TYPE thiscontract( self ); \
         switch( action ) { \
            EOSIO_API( TYPE, MEMBERS ) \
         } \
         /* does not allow destructor of thiscontract to run: eosio_exit(0); */ \
      } \
   } \
} \
 /// @}  dispatcher

EOSIO_ABI_EX(truelove, (transfer))
