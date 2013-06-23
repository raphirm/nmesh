#include <stdio.h>
#include "conn_io.h" // make sure we implement the same API

FILE *read_line(int socket_fd) {
	FILE *stream;
	stream = fdopen(socket_fd, "r");
	if( stream == NULL )
	  return NULL;

	return stream;
}

int send_all(int socket_fd, void* bytes, size_t len) {
  size_t bytes_written,
	 total_bytes_written = 0;

  while( bytes_written != len )
  {
    bytes_written = write( socket_fd,
		           bytes + total_bytes_written,
			   len   - total_bytes_written);
    if( bytes_written == -1 ) {
      perror("failed to write to client");
      return -1;
    }
    total_bytes_written += bytes_written;
  }
}

