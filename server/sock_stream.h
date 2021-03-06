#pragma once

#include "../utils/blob.h"
#include "../utils/string.h"
#include "client_socket.h"

class SockStream {
public:
	SockStream(ClientSocket &socket);

	void skip(int count);
	String peek_string(int length);
	String peek_to_crlf();
	String read_to_crlf();
	Blob read_bytes(int count);
	void read_bytes(unsigned char *destination, int count);

private:
	void ensure_chars(int count, int firstIndex = -1);

	ClientSocket &socket;
	Blob buffer;
	size_t position;
};
