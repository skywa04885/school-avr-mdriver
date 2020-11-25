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

void command_write_encoded_byte(uint8_t b)
{
  uint8_t t = b &0x0F;
  usart_write_byte(t);

  t = (b & 0xF0) >> 4;
  usart_write_byte(t);
}

void command_send(command_packet_t *pkt)
{
  uint8_t i;

  // Sends the start condition
  usart_write_byte(COMMANDS_USART_FLAG);

  // Starts writing the static part of the command
  for (i = 0; i < sizeof (command_packet_t); ++i)
    command_write_encoded_byte(((uint8_t *) pkt)[i]);

  // STarts writing the payload of the packet
  for (i = 0; i < pkt->body.size; ++i)
    command_write_encoded_byte(pkt->body.payload[i]);

  // Sends the stop condition
  usart_write_byte(COMMANDS_USART_FLAG);
}
