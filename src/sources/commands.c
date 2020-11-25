#include "../headers/commands.h"

void command_usart_read(uint8_t *buffer)
{
  uint8_t i = 0;

  // Waits for the start condition to be transmitted
  while (usart_read_byte() != COMMANDS_USART_FLAG);

  // Reads the rest of the packet, until the stop condition is reached
  //  or when i hits 240
  uint8_t t;
  while (i < 240) {
    t = command_read_byte();
    if (t == COMMANDS_USART_FLAG) break;
    else buffer[i++] = t;
  }
}

void command_send_byte(uint8_t byte)
{
  usart_write_byte(byte & 0x0F);
  usart_write_byte((byte & 0xF0) >> 4);
}

uint8_t command_read_byte(void)
{
  uint8_t t = usart_read_byte();
  if (t == COMMANDS_USART_FLAG) return t;
  return t | (usart_read_byte() << 4);
}