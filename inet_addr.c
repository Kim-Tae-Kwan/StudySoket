#include <stdio.h>
#include <arpa/inet.h>

int main()
{

	char *addr1;
	char *addr2;

	addr1=gets();
	addr2=gets();
	



	

	unsigned long conv=inet_addr(addr1);
	if(conv==INADDR_NONE)
		printf("Error occured!\n");
	else
		printf("Network ordered integer addr : %#lx \n",conv);

	conv=inet_addr(addr2);
	if(conv==INADDR_NONE)
		printf("Error occureded \n");
	else
		printf("Network ordered integer addr : %#lx \n",conv);

	return 0;


}
