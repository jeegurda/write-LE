#include <iostream>
#include <iomanip>

enum endianness_t {
  BIG,         // 0x44332211  => 0x44 0x33 0x22 0x11
  LITTLE,      // 0x44332211  => 0x11 0x22 0x33 0x44
  UNKNOWN
};

const uint32_t test_value    = 0x44332211;
const bool is_little_endian  = (((char *)&test_value)[0] == 0x11) && (((char *)&test_value)[1] == 0x22);
const bool is_big_endian     = (((char *)&test_value)[0] == 0x44) && (((char *)&test_value)[1] == 0x33);

const endianness_t endianness =
   is_big_endian ? BIG:
  (is_little_endian ? LITTLE : UNKNOWN);


template <typename T>
T identity(T v){
  return v;
}

// 16 bits values

uint16_t swap_(uint16_t v) {
  return ((v & 0xFF) << 8) | ((v & 0xFF00) >> 8);
}

// 32 bits values

uint32_t swap_(uint32_t v) {
  return ((v & 0xFF) << 24) | ((v & 0xFF00) << 8) | ((v & 0xFF0000) >> 8) | ((v & 0xFF000000) >> 24);
}

template <typename T, endianness_t HOST, endianness_t REMOTE>
 struct en_swap {
  static T conv(T v) {
    return swap_(v);
  }
};

template <typename T>
struct en_swap<T, BIG, BIG> {
  static T conv(T v){
    return v;
  }
};

template <typename T>
struct en_swap<T, LITTLE, LITTLE> {
  static T conv(T v){
    return v;
  }
};

template <typename T>
T to_big(T v) {

  switch (endianness) {
  case LITTLE :
    return en_swap<T,LITTLE,BIG>::conv(v);
  case BIG :
    return en_swap<T,BIG,BIG>::conv(v);
  }
}

template <typename T>
T to_little(T v) {
   switch (endianness) {
   case LITTLE :
     return en_swap<T,LITTLE,LITTLE>::conv(v);
   case BIG :
     return en_swap<T,BIG,LITTLE>::conv(v);
  }
}


int main() {
  using namespace std;

  uint32_t x = 0x0ABCDEF0;
  uint32_t y = to_big(x);
  uint32_t z = to_little(x);

  cout << hex << setw(8) << setfill('0') << x << " " << y << " " << setw(8) << setfill('0') << z << endl;
}
