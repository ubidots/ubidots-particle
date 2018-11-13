#ifndef _UbiTypes_H_
#define _UbiTypes_H_

typedef struct Value {
  char  *variable_label;
  char  *dot_context;
  float dot_value;
  unsigned long dot_timestamp_seconds;
  unsigned int dot_timestamp_millis;
} Value;

typedef enum {
  UBI_HTTP, UBI_TCP, UBI_UDP, UBI_PARTICLE
} IotProtocol;

#endif