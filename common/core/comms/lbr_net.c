#include "lbr_net.h"

void lbr_net_node_init(Bus* sender, uint8_t address)
{
    sender->address = address;
    sender->state = IDLE;
    sender->pack = pack;
    sender->read_byte = read_byte;
    sender->get_package_size = get_package_size;
    sender->flush_data = flush_data;
    sender->receive_index = 0;
    sender->package_size = 0;
}

bool pack(Bus* self, uint8_t* buffer, uint16_t buffer_size, uint8_t target,
          uint8_t* data, uint8_t data_size)
{
    uint8_t index = 0;
    uint32_t sum = 0;
    uint8_t* output = buffer;
    memset(output, 0, buffer_size);
    if (data_size + PACKET_HEADER_SIZE > buffer_size)
    {
        return false;
    }
    output[index++] = START_TRANSMISSION;
    sum += START_TRANSMISSION;
    output[index++] = target;
    sum += target;
    output[index++] = data_size;
    sum += data_size;
    for (uint8_t i = 0; i < data_size; i++)
    {
        output[index++] = data[i];
        sum += data[i];
    }
    output[index++] = sum % CHECKSUM_BITS;
    return true;
}

void read_byte(Bus* self, uint8_t data)
{
    {
        switch (self->state)
        {
            case IDLE:
                if (data == NACK)
                {
                    self->state = ERROR;
                }
                else if (data == START_TRANSMISSION)
                {
                    self->state = READ_ADDRESS;
                    memset(self->receive_buffer, 0,
                           sizeof(self->receive_buffer));
                    self->sum = 0;
                    self->receive_index = 0;
                    self->package_size = 0;
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;

                    self->receive_index += 1;
                }
                if (data == ACK)
                {
                    self->state = ACKNOWLEDGED;
                }
                break;
            case READ_ADDRESS:
                if (data == self->address)
                {
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;
                    self->receive_index += 1;
                    self->state = READ_LEN;
                }
                else
                {
                    self->state = IDLE;
                }
                break;
            case READ_LEN:
                self->package_size = data;
                self->receive_buffer[self->receive_index] = data;
                self->sum += data;
                self->receive_index += 1;
                self->state = READ_DATA;
                break;
            case READ_DATA:
                if (self->receive_index == self->package_size + 2)
                {
                    self->state = VALIDATE;
                }
                self->receive_buffer[self->receive_index] = data;
                self->sum += data;
                self->receive_index += 1;

                break;
            case VALIDATE:
                self->sum = self->sum % 256;
                if (self->sum != data)
                {
                    self->state = ERROR;
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;
                    self->receive_index += 1;
                }
                else
                {
                    self->state = FINISHED;
                    self->receive_buffer[self->receive_index] = data;
                    self->sum += data;
                    self->receive_index += 1;
                }

                break;
            case ERROR:
                break;
            default:
                break;
        }
    }
    return;
}

void flush_data(Bus* self, uint8_t* buffer)
{
    for (uint8_t i = 0; i < self->package_size; i++)
    {
        buffer[i] = self->receive_buffer[i + 3];
    }
    self->package_size = 0;
    memset(self->receive_buffer, 0, sizeof(self->receive_buffer));
    self->sum = 0;
    self->state = IDLE;
    return;
}

uint8_t get_package_size(Bus* self)
{
    return self->package_size;
}
