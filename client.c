#include <stdio.h>
#include <sys/socket.h> 
#include <errno.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/mman.h>
#define SA struct sockaddr

int req_page(){
	int num_pages;
	printf("How many pages would you like to allocate (greater than 0)?\n");
		if(scanf("%d", &num_pages)){
			printf("num_pages read is %d\n", num_pages);
		}
	return num_pages;
}

void read_page(int remote_server_sock){
	long y=0;
	int len;
	char * mmapped_addr;
	int x = read (remote_server_sock, &y, sizeof(y));
	printf("number of bytes read %d\n", x);
	if (x > 0){
		mmapped_addr = (char *) y;
	}

	if (read(remote_server_sock, &len, sizeof(len)) < 0){
                printf("len read failed\n");
        } 
     
	mmapped_addr = mmap((char *)y, len, PROT_READ | PROT_WRITE,
                    MAP_SHARED | MAP_ANONYMOUS, -1, 0);
                if (mmapped_addr == MAP_FAILED)
                        printf("memory allocation unsuccessful\n");
                else
                        printf("memory allocation successful, mmapped address = %p, mmapped size = %d\n", mmapped_addr, len);
}