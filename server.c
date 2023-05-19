#include "Adresses.h"
#include "HashTable.h"

//============================================================================================================

uv_loop_t *loop;
uv_udp_t send_socket;
uv_udp_t recv_socket;

//============================================================================================================

size_t hashfunc(size_t htsize, const char* key) 
{
        size_t h = 0x73f8e5a39fe37a1b;
        while(*key) 
        {
                h ^= h << 13 | h >> 51;
                h += (*key)*0xa5a15f11a1f1af31;
                h = h << 49 | h>>15;
                key++;
        }
        return h % htsize;
}

//============================================================================================================

void alloc_buffer(uv_handle_t* h, size_t size, uv_buf_t* buf) 
{
	buf->base = malloc(size);
	buf->len = size;
}

//============================================================================================================

void on_send(uv_udp_send_t* req, int status) 
{
	printf("Send done %p with status %d %s\n", req->handle, status, uv_err_name(status));
	free(req);	
}

//============================================================================================================

void on_read(uv_udp_t* req, ssize_t nread, const uv_buf_t* buf, const struct sockaddr* addr, unsigned flags) 
{
    char *resp_buf;
    if (nread < 0) {

        fprintf(stderr, "Read error %s\n", uv_err_name(nread));
        uv_close((uv_handle_t*) req, NULL);
        free(buf->base);
        return;
    }
    elif ( nread == 0 ) {
		printf("Empty data got\n");
	}
    elif(nread > 0)
    {
        if(db_interpret(buf->base, &resp_buf))
            printf("ERROR\n");
        uv_udp_send_t* send_req = malloc(sizeof(uv_udp_send_t));
        int err = uv_udp_send(send_req, &recv_socket, &resp_buf, 1, (const struct sockaddr*)addr, on_send);
		if ( err ) 
			printf("Send %p returned: %d %s\n", &recv_socket, err, uv_err_name(err));

    }

    free(buf->base);
    uv_udp_recv_stop(req);
}

//============================================================================================================

int main()
{
    loop = uv_default_loop();
  
    uv_udp_init(loop, &recv_socket);
    struct sockaddr_in recv_addr;
    uv_ip4_addr(IP_ADDRESS, PORT, &recv_addr);
    uv_udp_bind(&recv_socket, (const struct sockaddr *)&recv_addr, UV_UDP_REUSEADDR);
    uv_udp_recv_start(&recv_socket, alloc_buffer, on_read);

	printf("Started read\n");
	uv_run(loop, UV_RUN_DEFAULT);
}

//============================================================================================================