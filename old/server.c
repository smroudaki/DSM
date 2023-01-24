#include <stdio.h>
#include <sys/socket.h> 
#include <errno.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/mman.h>

void create_socket_server(struct sockaddr_in self_address,int local_port_number,int local_server_sock){
	int new_socket=0;
	self_address.sin_family = AF_INET;
		self_address.sin_addr.s_addr = INADDR_ANY;
		self_address.sin_port = htons(local_port_number);
		int addrlen = sizeof(self_address);
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
}

void send_page(int num_pages,int new_socket){
	int len;
	long y=0;
	char * mmapped_addr;
	int page_size = sysconf(_SC_PAGE_SIZE);
	len = num_pages * page_size;
		mmapped_addr = mmap(NULL, len, PROT_READ,
		    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
		if (mmapped_addr == MAP_FAILED)
			printf("memory allocation unsuccessful\n");
		else 
			printf("memory allocation successful, mmapped address = %p, mmapped size = %d\n", 
					mmapped_addr, len);

		y = (unsigned long) mmapped_addr;
		
		send (new_socket, &y, sizeof(y), 0);
		//printf("Mapped address pointer sent to client\n");

		send (new_socket, &len, sizeof(len), 0);
		//printf("Mapped size sent to client\n");
}

// lock_memory(char   *addr,
//             size_t  size)
// {
//   unsigned long    page_offset, page_size;

//   page_size = sysconf(_SC_PAGE_SIZE);
//   page_offset = (unsigned long) addr % page_size;

//   addr -= page_offset;  /* Adjust addr to page boundary */
//   size += page_offset;  /* Adjust size with page_offset */

//   return ( mlock(addr, size) );  /* Lock the memory */
// }

// unlock_memory(char   *addr,
//               size_t  size)
// {
//   unsigned long    page_offset, page_size;

//   page_size = sysconf(_SC_PAGE_SIZE);
//   page_offset = (unsigned long) addr % page_size;

//   addr -= page_offset;  /* Adjust addr to page boundary */
//   size += page_offset;  /* Adjust size with page_offset */

//   return ( munlock(addr, size) );  /* Unlock the memory */
// }