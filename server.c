#include "Adresses.h"
#include "HashTable.h"
#include "Main.h"

//============================================================================================================

uv_loop_t *loop;
uv_udp_t send_socket;
uv_udp_t recv_socket;

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
	free(req->data);
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
	else if ( nread == 0 ) {
		printf("Empty data got\n");
	}
	else if(nread > 0)
    {
		printf("Interpreting buf %s\n", buf->base);
		uv_buf_t rbuf;
        if((rbuf.len = db_interpret(buf->base, &resp_buf)) <= 0)
            printf("ERROR\n");
		rbuf.base = resp_buf;
        uv_udp_send_t* send_req = malloc(sizeof(uv_udp_send_t));
		send_req->data = resp_buf;
        int err = uv_udp_send(send_req, &recv_socket, &rbuf, 1, (const struct sockaddr*)addr, on_send);
		if ( err ) 
			printf("Send %p returned: %d %s\n", &recv_socket, err, uv_err_name(err));

    }

    free(buf->base);
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
	db_init();
	printf("Started read\n");
	uv_run(loop, UV_RUN_DEFAULT);
}

//============================================================================================================
