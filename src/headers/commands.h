#ifndef _SRC_HEADERS_COMMANDS_H
#define _SRC_HEADERS_COMMANDS_H

#include "include.h"

typedef enum __attribute__ (( packed )) {
  COMMAND_TYPE_ENABLE,
  COMMAND_TYPE_DISABLE,
  COMMAND_TYPE_MOVE,
  COMMAND_TYPE_HOME
} command_type_t;

typedef struct __attribute__ (( packed )) {
  unsigned reserved : 8;
} command_flags_t;

typedef struct __attribute__ (( packed )) {
  command_type_t type;
  command_flags_t flags;
} command_header_t;

typedef struct __attribute__ (( packed )) {
  uint8_t size;
  char payload[0];
} command_body_t;

typedef struct __attribute__ (( packed )) {
  command_header_t hdr;
  command_body_t body;
} command_t;

#endif
