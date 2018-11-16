#ifndef _UbiTypes_H_
#define _UbiTypes_H_

#include "Particle.h"

typedef struct Value {
  char  *variable_label;
  char  *dot_context;
  float dot_value;
  unsigned long dot_timestamp_seconds;
  unsigned int dot_timestamp_millis;
} Value;

typedef struct ContextUbi {
  char *key_label;
  char *key_value;
} ContextUbi;

typedef const char* UbiServer;

typedef enum {
  UBI_HTTP, UBI_TCP, UBI_UDP, UBI_PARTICLE
} IotProtocol;

typedef struct Ubi_flags {
  Ubi_flags(): particle_flag(PUBLIC) {}
  PublishFlags particle_flag;
} Ubi_flags;

#endif