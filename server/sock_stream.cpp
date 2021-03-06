#include "sock_stream.h"
#include <sys/socket.h>
#include <string.h>

SockStream::SockStream(ClientSocket &socket) : socket(socket), buffer("") {
	position = 0;
}

void SockStream::skip(int count) {
	ensure_chars(count);
	position += count;
}

String SockStream::peek_string(int length) {
	ensure_chars(length);
	return String((const char *) buffer.ptr(), position, length);
}

String SockStream::read_to_crlf() {
	String result = peek_to_crlf();
	position += result.size() + 2;
	return result;
}

String SockStream::peek_to_crlf() {
	String result = "";
	size_t start = position;

	while (true) {
		ensure_chars(2, start);

		int line_end = buffer.index("\r\n", start);
		if (line_end >= 0) {
			result += String((const char *) buffer.substr(start, line_end - start).ptr(), 0, line_end - start);
			return result;
		}

		result += (const char *) buffer.substr(start).ptr();
		start = buffer.size();
	}
}

Blob SockStream::read_bytes(int count) {
	Blob result;
	result.ensure_capacity(count);
	ensure_chars(count);

	result = buffer.substr(position, count);
	position += count;

	return result;
}

void SockStream::read_bytes(unsigned char *destination, int count) {
	ensure_chars(count);
	memcpy(destination, buffer.ptr() + position, count);
	position += count;
}

void SockStream::ensure_chars(int count, int firstIndex) {
	if (firstIndex == -1)
		firstIndex = position;

	while ((size_t) (firstIndex + count) > buffer.size()) {
		Blob temp = socket.receive();
		buffer += temp;
	}
}
