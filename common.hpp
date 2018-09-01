#include <eosiolib/eosio.hpp>
#include <eosiolib/types.h>
#include <string>
/**
* class to represent an in-wasm-memory array
* it is a hint to the transcriber that the next parameter will
* be a size (data bytes length) and that the pair are validated together
* This triggers the template specialization of intrinsic_invoker_impl
* @tparam T
*/
namespace comm {
  template<typename T>
  struct array_ptr {
    explicit array_ptr (T * value) : value(value) {}

    typename std::add_lvalue_reference<T>::type operator*() const {
        return *value;
    }

    T *operator->() const noexcept {
        return value;
    }

    template<typename U>
    operator U *() const {
        return static_cast<U *>(value);
    }

    T *value;
  }; 

  std::string to_hex( const char* d, uint32_t s ) 
  {
      std::string r;
      const char* to_hex="0123456789abcdef";
      uint8_t* c = (uint8_t*)d;
      for( uint32_t i = 0; i < s; ++i )
          (r += to_hex[(c[i]>>4)]) += to_hex[(c[i] &0x0f)];
      return r;
  }
}