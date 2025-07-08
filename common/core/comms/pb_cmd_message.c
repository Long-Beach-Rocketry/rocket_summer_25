#include "pb_cmd.h"

bool get_buf_pb(cmd_message* self, uint8_t* buffer, size_t size)
{
    pb_cmd* cmd = self->priv;
    bool status;
    pb_ostream_t stream = pb_ostream_from_buffer(buffer, sizeof(buffer));
    status = pb_encode(&stream, ((pb_cmd*)self->priv)->schema_field,
                       ((pb_cmd*)self->priv)->schema_object);
    cmd->buffer_size = stream.bytes_written;
    return status;
}

size_t get_size_pb(cmd_message* self)
{
    pb_cmd* cmd = self->priv;
    return cmd->buffer_size;
}

bool parse_pb(cmd_message* self, uint8_t* buffer, size_t size)
{
    bool status;
    pb_cmd* cmd = self->priv;
    pb_istream_t stream = pb_istream_from_buffer(buffer, size);
    status = pb_decode(&stream, ((pb_cmd*)self->priv)->schema_field,
                       ((pb_cmd*)self->priv)->schema_object);
    return status;
}

bool pb_cmd_message_init(cmd_message* cmd_message, pb_cmd* cmd)
{
    cmd_message->get_buf = get_buf_pb;
    cmd_message->get_size = get_size_pb;
    cmd_message->parse = parse_pb;
    cmd_message->priv = (void*)cmd;
    return true;
}