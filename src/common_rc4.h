typedef struct {
  const char* key;
  unsigned int i_iterator;
  unsigned int j_iterator;
  unsigned char characters[256];
} rc4_t;

void rc4_init(rc4_t* self, unsigned char* key, unsigned int i, unsigned int j);

void rc4_KSA(rc4_t* self);

void rc4_init_characters(rc4_t* self, unsigned char* characters);

void _rc4_swap(unsigned char* characters, unsigned int i, unsigned int j);

void rc4_PRGA(rc4_t* self, char* msg, unsigned char* result,
              unsigned char* characters);
