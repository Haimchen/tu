
#ifndef PASSWD_DECRYPTER_H_
#define PASSWD_DECRYPTER_H_

typedef struct {
	ringbuffer* in;
	unsigned char **chat_lines;
	uint* line_length;
	uint num_lines;
	uint num_threads;
} decrypter_args;

void* decrypter_messages_thread(void* args);

#endif /* PASSWD_DECRYPTER_H_ */
