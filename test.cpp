
#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/types.h>
#include <eosiolib/crypto.h>
#include <eosiolib/transaction.h>
#include <sstream>
// #include <fc/crypto/hex.hpp>
#include "./common.hpp"
#include <string>

using std::string;
using namespace eosio;
//所有智能合约继承contract类
class truelove : public eosio::contract {
  public:
      // 初始化lovertable, scope和contract都是自己
      truelove(account_name s):eosio::contract(s), lovetable(s, s)
      {}


      // @abi action
      void transfer(account_name sender, account_name receiver, asset quanity, string memo) {
        
        lovetable.emplace( _self, [&]( auto& s ) {
          //获取下一个可用的主键值，这样可以实现id的自动增长
          s.id = lovetable.available_primary_key();
          s.sender = sender;
        });
        
      }

  private:
      // @abi table 
      struct lover{
        uint64_t        id; //auto increment id
        account_name    sender;
        
        account_name primary_key()const { return id; }
        
        EOSLIB_SERIALIZE( lover, (id)(amount)(sender)(txHash)(letter) )
      };
      
      typedef eosio::multi_index<N(lover), lover> records;
      records lovetable;
};
