/*
  Memcached library

  memcached_response() is used to determine the return result
  from an issued command.
*/

#include <memcached.h>

memcached_return memcached_response(memcached_st *ptr, 
                                    char *buffer, size_t buffer_length)
{
  size_t send_length;

  memset(buffer, 0, buffer_length);
  send_length= read(ptr->hosts[0].fd, buffer, buffer_length);

  if (send_length)
    switch(buffer[0])
    {
    case 'V': /* VALUE */
      return MEMCACHED_SUCCESS;
    case 'O': /* OK */
      return MEMCACHED_SUCCESS;
    case 'S': /* STORED STATS SERVER_ERROR */
      {
        if (buffer[1] == 'T') /* STORED STATS */
          return MEMCACHED_SUCCESS;
        else if (buffer[1] == 'E')
          return MEMCACHED_SERVER_ERROR;
        else
          return MEMCACHED_UNKNOWN_READ_FAILURE;
      }
    case 'D': /* DELETED */
      return MEMCACHED_SUCCESS;
    case 'N': /* NOT_FOUND */
      {
        if (buffer[4] == 'F')
          return MEMCACHED_NOTFOUND;
        else if (buffer[4] == 'S')
          return MEMCACHED_NOTSTORED;
        else
          return MEMCACHED_UNKNOWN_READ_FAILURE;
      }
    case 'E': /* PROTOCOL ERROR */
      return MEMCACHED_PROTOCOL_ERROR;
    case 'C': /* CLIENT ERROR */
      return MEMCACHED_CLIENT_ERROR;
    default:
      return MEMCACHED_UNKNOWN_READ_FAILURE;
    }

  return MEMCACHED_READ_FAILURE;
}
