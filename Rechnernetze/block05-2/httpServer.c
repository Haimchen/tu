/*
############################################################################
#                                                                          #
# In weiten Teilen übernommen von simplepost.c von Christian Grothoff      #
# github.com/ulion/libmicrohttpd/blob/master/doc/examples/simplepost.c     #
# (Original License: Public Domain)                                        #
#                                                                          #
############################################################################
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <microhttpd.h>

#define POSTBUFFERSIZE  512
#define MAXNAMESIZE     20
#define MAXANSWERSIZE   512

#define ERR		0
#define POST		1
#define PUT		2
#define GET		3
#define DEL		4

char* hashname = "hashtable";

// This struct holds all the information about the Client's connection we need to process a request:
struct connection_info_struct {
    int connectiontype;
    uint16_t key;
    uint16_t value;
    uint16_t valbef;
    int16_t err_code;
    uint16_t http_check;
    char *answerstring;
    struct MHD_PostProcessor *postprocessor;
};

int sockfd;
struct sockaddr_in their_addr; // information for connection with server

int packData(char *buffer, char *order, uint16_t key, uint16_t value) {
	snprintf(buffer, sizeof order, "%s", order);
	
	buffer[4] = key >> 8;
	buffer[5] = key;
	buffer[6] = value >> 8;
	buffer[7] = value;
	
	// do not function with the server.
	/*
	uint16_t tmp = htons(key);

	buffer[4] = tmp >> 8; // erste Hälfte von tmp
	buffer[5] = tmp;

	tmp = htons(value);
	buffer[6] = tmp >> 8;
	buffer[7] = tmp;
	*/
	return 0;
}

void unpack(char *buffer, uint16_t *key, uint16_t *value) {
	uint16_t tmp;
	
	tmp = (buffer[4] << 8) | buffer[5]; 
	*key = ntohs(tmp);

	tmp = (buffer[6] << 8) | buffer[7];
	*value = ntohs(tmp);
}

int getServer(uint16_t key, uint16_t *value) {
	char order[8];
	packData(order, "GET", htons(key), 0);

	sendto(sockfd, order, sizeof(order), 0, (struct sockaddr *) &their_addr, sizeof their_addr);

	char buffer[8];

	recv(sockfd, buffer, sizeof buffer, 0);

	if (buffer[0] == 'N' && buffer[1] == 'O' && buffer[2] == 'F') {
		printf("GET ERROR: key %u not found in table\n", key);
		return -1;
	} else {
		uint16_t key_in = 0;

		unpack(buffer, &key_in, value);

		printf("GET: key = %d | value = %d\n", key_in, *value);
		return 0;
	}
}

int setServer(uint16_t key, uint16_t value) {
	char order[8];
	packData(order, "SET", htons(key), htons(value));

	sendto(sockfd, order, sizeof(order), 0, (struct sockaddr *) &their_addr, sizeof their_addr);

	char buffer[8];

	recv(sockfd, buffer, sizeof buffer, 0);

	if (buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '!') {
		//printf("New in table: key = %d | value = %d\n", key, value);
		printf("SET: key %u | value %d\n", key, value);
		return 0;
	} else {
		printf("SET ERROR: key %u | value %d not added to table\n", key, value);
		return -1;
	}
}

int delServer(uint16_t key) {
	char order[8];
	packData(order, "DEL", htons(key), 0);

	sendto(sockfd, order, sizeof(order), 0, (struct sockaddr *) &their_addr, sizeof their_addr);

	char buffer[8];

	recv(sockfd, buffer, sizeof buffer, 0);

	if (buffer[0] == 'O' && buffer[1] == 'K' && buffer[2] == '!') {
		printf("DEL: key %d\n", key);
		return 0;
	} else {
		printf("DEL ERROR: key %u not removed from table\n", key);
		return -1;
	}
}

// This function sends some answer string back to the Client.
static int send_page(struct MHD_Connection *connection, unsigned int status_code, const char *page)
{
    int ret;
    struct MHD_Response *response;
    response = MHD_create_response_from_buffer(strlen(page), (void *) page, MHD_RESPMEM_MUST_COPY);
    MHD_add_response_header(response, MHD_HTTP_HEADER_CONTENT_ENCODING, "text/plain");
    if (!response) {
        return MHD_NO;
    }
    ret = MHD_queue_response(connection, status_code, response);
    MHD_destroy_response(response);
    return ret;
}

// This is a function that processes our POST requests. It will iterate over all provided keys.
static int
iterate_post(void *coninfo_cls, enum MHD_ValueKind kind, const char *key,
             const char *filename, const char *content_type, const char *transfer_encoding, const char *data, uint64_t off, size_t size)
{
    struct connection_info_struct *con_info = coninfo_cls;

    if (0 == strcmp(key, "value")) {
        if ((size > 0) && (size <= MAXNAMESIZE)) {
            char *answerstring;
            answerstring = malloc(MAXANSWERSIZE);
            if (!answerstring) {
                return MHD_NO;
            }
            
            con_info->value = atoi(data);
	    
	    con_info->err_code = setServer(con_info->key, con_info->value);
        } else {
            con_info->answerstring = NULL;
        }
        return MHD_NO;
    } else {
	    con_info->err_code = -1;
    }
    return MHD_YES;
}

// This is called after a completed request: clean up (free)
static void request_completed(void *cls, struct MHD_Connection *connection, void **con_cls, enum MHD_RequestTerminationCode toe)
{
    struct connection_info_struct *con_info = *con_cls;

    if (NULL == con_info) {
        return;
    }

    if (con_info->connectiontype == POST) {
        MHD_destroy_post_processor(con_info->postprocessor);
        if (con_info->answerstring) {
            free(con_info->answerstring);
        }
    }

    free(con_info);
    *con_cls = NULL;
}

// We define a callback to be called by libmicrohttpd when a client
// requests a resource. The following comment is straight from the microhttpd.h
// file:
/**
 * A client has requested the given url using the given method
 * (#MHD_HTTP_METHOD_GET, #MHD_HTTP_METHOD_PUT,
 * #MHD_HTTP_METHOD_DELETE, #MHD_HTTP_METHOD_POST, etc).  The callback
 * must call MHD callbacks to provide content to give back to the
 * client and return an HTTP status code (i.e. #MHD_HTTP_OK,
 * #MHD_HTTP_NOT_FOUND, etc.).
 *
 * @param cls argument given together with the function
 *        pointer when the handler was registered with MHD
 * @param url the requested url
 * @param method the HTTP method used (#MHD_HTTP_METHOD_GET,
 *        #MHD_HTTP_METHOD_PUT, etc.)
 * @param version the HTTP version string (i.e.
 *        #MHD_HTTP_VERSION_1_1)
 * @param upload_data the data being uploaded (excluding HEADERS,
 *        for a POST that fits into memory and that is encoded
 *        with a supported encoding, the POST data will NOT be
 *        given in upload_data and is instead available as
 *        part of #MHD_get_connection_values; very large POST
 *        data *will* be made available incrementally in
 *        @a upload_data)
 * @param upload_data_size set initially to the size of the
 *        @a upload_data provided; the method must update this
 *        value to the number of bytes NOT processed;
 * @param con_cls pointer that the callback can set to some
 *        address and that will be preserved by MHD for future
 *        calls for this request; since the access handler may
 *        be called many times (i.e., for a PUT/POST operation
 *        with plenty of upload data) this allows the application
 *        to easily associate some request-specific state.
 *        If necessary, this state can be cleaned up in the
 *        global #MHD_RequestCompletedCallback (which
 *        can be set with the #MHD_OPTION_NOTIFY_COMPLETED).
 *        Initially, `*con_cls` will be NULL.
 * @return #MHD_YES if the connection was handled successfully,
 *         #MHD_NO if the socket must be closed due to a serios
 *         error while handling the request
 */
static int
answer_to_connection(void *cls, struct MHD_Connection *connection,
                     const char *url, const char *method, const char *version, const char *upload_data, size_t * upload_data_size, void **con_cls)
{
	char *answerstring;
	answerstring = malloc(MAXANSWERSIZE);
	
	if (*con_cls == NULL) {
		struct connection_info_struct *con_info;
		char tmp_url[sizeof(url)];
		strcpy(tmp_url,url);
		char *tmp_tok;

		con_info = malloc(sizeof(struct connection_info_struct));
		if (con_info == NULL) {
			return MHD_NO;
		}
		con_info->answerstring = NULL;
		
		tmp_tok = strtok(tmp_url, "/");
		
		if (tmp_tok == NULL){
			return send_page(connection, MHD_HTTP_NOT_FOUND, "No ressource selected\n");
		}
		
		if (strcmp(tmp_tok,hashname) != 0) {
			return send_page(connection, MHD_HTTP_NOT_FOUND, "Ressource not found\n");
		}
		
		tmp_tok = strtok(NULL, "/");
		
		if (strcmp(method, "POST") == 0) {
			con_info->key = atoi(tmp_tok);
			con_info->postprocessor = MHD_create_post_processor(connection, POSTBUFFERSIZE, iterate_post, (void *) con_info);
			con_info->err_code = getServer(con_info->key, &con_info->valbef);
			
			if (con_info->err_code >= 0) {
				snprintf(answerstring, MAXANSWERSIZE, "Error: Key %u with value %d in table\n", con_info->key, con_info->valbef);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_FORBIDDEN;
				return send_page(connection, con_info->http_check, con_info->answerstring);
			} else {
				con_info->postprocessor = MHD_create_post_processor(connection, POSTBUFFERSIZE, iterate_post, (void *) con_info);

				if (con_info->postprocessor == NULL) {
					free(con_info);
					return MHD_NO;
				}
				con_info->connectiontype = POST;
			}
		} else if (strcmp(method, "PUT") == 0) {
			con_info->connectiontype = PUT;
			con_info->key = atoi(tmp_tok);
			con_info->err_code = getServer(con_info->key, &con_info->valbef);
			
			if (con_info->err_code < 0) {
				snprintf(answerstring, MAXANSWERSIZE, "Error: Key %u not found\n", con_info->key);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_NOT_FOUND;
				return send_page(connection, con_info->http_check, con_info->answerstring);
			} else {
				con_info->postprocessor = MHD_create_post_processor(connection, POSTBUFFERSIZE, iterate_post, (void *) con_info);

				if (con_info->postprocessor == NULL) {
					free(con_info);
					return MHD_NO;
				}
				con_info->connectiontype = PUT;
			}
		} else if (strcmp(method, "GET") == 0) {
			con_info->connectiontype = GET;
			con_info->key = atoi(tmp_tok);
			con_info->err_code = getServer(con_info->key, &con_info->value);
			if (con_info->err_code < 0) {
				snprintf(answerstring, MAXANSWERSIZE, "Error: Key %u not found\n", con_info->key);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_NOT_FOUND;
			} else {
				snprintf(answerstring, MAXANSWERSIZE, "Key %u with value %d found\n", con_info->key, con_info->value);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_OK;
			}
		}  else if (strcmp(method, "DELETE") == 0) {
			con_info->connectiontype = DEL;
			con_info->key = atoi(tmp_tok);
			con_info->err_code = delServer(con_info->key);
			if (con_info->err_code < 0) {
				snprintf(answerstring, MAXANSWERSIZE, "Error: Key %u not found / deleted\n", con_info->key);
				con_info->answerstring = answerstring;
				return send_page(connection, MHD_HTTP_NOT_FOUND, con_info->answerstring);
			} else {
				snprintf(answerstring, MAXANSWERSIZE, "Key %u deleted\n", con_info->key);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_OK;
			}
		}

		*con_cls = (void *) con_info;
		return MHD_YES;
	}

	if (0 == strcmp(method, "POST")) {
		struct connection_info_struct *con_info = *con_cls;

		if (*upload_data_size != 0) {
			MHD_post_process(con_info->postprocessor, upload_data, *upload_data_size);
			*upload_data_size = 0;

			if (con_info->err_code < 0) {
				snprintf(answerstring, MAXANSWERSIZE, "Error: Key %u with value %d not added\n", con_info->key, con_info->value);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_BAD_REQUEST;
			} else {
				snprintf(answerstring, MAXANSWERSIZE, "Key %u with value %d added\n", con_info->key, con_info->value);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_CREATED;
			}
			
			return MHD_YES;
		} else if (NULL != con_info->answerstring) {
			return send_page(connection, con_info->http_check, con_info->answerstring);
		}
	}
	if (0 == strcmp(method, "PUT")) {
		struct connection_info_struct *con_info = *con_cls;

		if (*upload_data_size != 0) {
			MHD_post_process(con_info->postprocessor, upload_data, *upload_data_size);
			*upload_data_size = 0;

			if (con_info->err_code < 0) {
				snprintf(answerstring, MAXANSWERSIZE, "Error: Key %u with value %d not updated\n", con_info->key, con_info->value);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_BAD_REQUEST;
			} else {
				snprintf(answerstring, MAXANSWERSIZE, "Key %u with value %d updated (before: %d)\n", con_info->key, con_info->value, con_info->valbef);
				con_info->answerstring = answerstring;
				con_info->http_check = MHD_HTTP_CREATED;
			}
			
			return MHD_YES;
		} else if (NULL != con_info->answerstring) {
			return send_page(connection, con_info->http_check, con_info->answerstring);
		}
	}
	if (0 == strcmp(method, "GET")) {
		struct connection_info_struct *con_info = *con_cls;
		if (NULL != con_info->answerstring) {
			return send_page(connection, con_info->http_check, con_info->answerstring);
		}
	}
	if (0 == strcmp(method, "DELETE")) {
		struct connection_info_struct *con_info = *con_cls;
		if (NULL != con_info->answerstring) {
			return send_page(connection, con_info->http_check, con_info->answerstring);
		}
	}
	return send_page(connection, MHD_HTTP_NOT_FOUND, "404 - Not found\n");
}

int main(int argc, char *const *argv)
{
	struct MHD_Daemon *d;
	int serverPort;
	struct hostent *he;

	if (argc != 4) {
		printf("PORT  serveraddress serverport \n");
		return 1;
	}
	
	serverPort = atoi(argv[3]);

	//Resolv hostname to IP Address
	if ((he=gethostbyname(argv[2])) == NULL) {  // get the host info
		herror("gethostbyname");
		exit(1);
	}
	/* ******************************************************************
	Creachar* address; intte Socket
	******************************************************************* */

	sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    
	//setup transport address
	their_addr.sin_family = AF_INET;
	their_addr.sin_port = htons(serverPort);
	their_addr.sin_addr = *((struct in_addr *)he->h_addr);
	memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);
	
	// straight from the docs:
	/**
	* Start a webserver on the given port.  Variadic version of
	* #MHD_start_daemon_va.
	*
	* @param flags combination of `enum MHD_FLAG` values
	* @param port port to bind to
	* @param apc callback to call to check which clients
	*        will be allowed to connect; you can pass NULL
	*        in which case connections from any IP will be
	*        accepted
	* @param apc_cls extra argument to apc
	* @param dh handler called for all requests (repeatedly)
	* @param dh_cls extra argument to @a dh
	* @return NULL on error, handle to daemon on success
	* @ingroup event
	*/
	d = MHD_start_daemon(MHD_USE_SELECT_INTERNALLY | MHD_USE_DEBUG, atoi(argv[1]), NULL, NULL,
                         &answer_to_connection, NULL, MHD_OPTION_NOTIFY_COMPLETED, request_completed, NULL, MHD_OPTION_END);

	if (d == NULL) {
		return 1;
	}
	// wait for input and exit
	getc(stdin);

	MHD_stop_daemon(d);
	close(sockfd);
	return 0;
}
