#ifndef _SRC_HEADERS_COMMANDS_H
#define _SRC_HEADERS_COMMANDS_H

#include "include.h"
#include "usart.h"

/******************************
 * Definitions
 ******************************/

#define COMMANDS_USART_FLAG       0xB3

/******************************
 * Datatypes
 ******************************/

typedef enum __attribute__ (( packed )) {
  COMMAND_MOTOR_MX = 0,
  COMMAND_MOTOR_MR
} command_motor_t;

typedef enum __attribute__ (( packed )) {
  COMMAND_TYPE_ENABLE = 0,    /* Disables specified motor */
  COMMAND_TYPE_DISABLE,       /* Enables specified motor */
  COMMAND_TYPE_MOVE,          /* Move to POS */
  COMMAND_TYPE_EAHOME         /* Enter Auto-Homing Mode */
} command_type_t;

typedef struct __attribute__ (( packed )) {
  unsigned reserved : 8;
} command_packet_flags_t;

typedef struct __attribute__ (( packed )) {
  command_type_t type;
  command_packet_flags_t flags;
} command_packet_header_t;

typedef struct __attribute__ (( packed )) {
  uint8_t size;
  char payload[0];
} command_packet_body_t;

typedef struct __attribute__ (( packed )) {
  command_packet_header_t hdr;
  command_packet_body_t body;
} command_packet_t;

/******************************
 * Prototypes
 ******************************/

void command_usart_read(uint8_t *buffer);

void command_send_byte(uint8_t byte);
uint8_t command_read_byte(void);

#endif
