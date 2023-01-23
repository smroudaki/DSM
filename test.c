#include <stdio.h>
#include <stdlib.h> 
#include <arpa/inet.h>
#include "server.c"
#include "client.c"

#include <sys/socket.h> 

int main(int argc, char const *argv[]){
    int local_port_number, remote_port_number, num_pages, page_size, len, x;
	int remote_server_sock = 0, local_server_sock = 0, new_socket = 0, option = 1;
	struct sockaddr_in self_address, serv_addr;
	int addrlen = sizeof(self_address);
	char * mmapped_addr;
	long y=0;

	if (argc != 3) {
		fprintf(stderr, "invalid number of arguments");
		exit(EXIT_FAILURE);
	}

	local_port_number = strtoul(argv[1], NULL, 0);
	remote_port_number = strtoul(argv[2], NULL, 0);
    remote_server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (remote_server_sock < 0) {
		printf("\n Socket creation error \n");
	}

	memset(&serv_addr, '0', sizeof(serv_addr));
    	serv_addr.sin_family = AF_INET;
    	serv_addr.sin_port = htons(remote_port_number);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\nInvalid address/ Address not supported \n");
		exit(EXIT_FAILURE);
	}

	if (connect(remote_server_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
		printf("\nConnection Failed \n");

		if ((local_server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			printf("\n Socket creation error \n");
			exit(EXIT_FAILURE);
		}

		if (setsockopt(local_server_sock, SOL_SOCKET, 
			SO_REUSEADDR | SO_REUSEPORT,
			&option, sizeof(option))) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}

		self_address.sin_family = AF_INET;
		self_address.sin_addr.s_addr = INADDR_ANY;
		self_address.sin_port = htons(local_port_number);


        	if (bind(local_server_sock, (struct sockaddr *)&self_address, 
			sizeof(self_address)) < 0) {
                	perror("bind failed");
	                exit(EXIT_FAILURE);
        	}


        	if (listen(local_server_sock, 3) < 0) {
                	perror("listen");
	                exit(EXIT_FAILURE);
        	}

        	if ((new_socket = accept(local_server_sock, (struct sockaddr *)&self_address,(socklen_t*)&addrlen)) < 0) {
                	perror("accept");
	                exit(EXIT_FAILURE);
        	}
        int npg=req_page();
        send_page(npg,new_socket);
        
    }
    read_page(remote_server_sock);
    
}
