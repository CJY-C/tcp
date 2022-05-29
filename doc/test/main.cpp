#include <thread>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>

void server();
void client();

int main(int argc, char* argv[])
{
    std::thread server_thread(server);

    char input = '0';
    while (true)
    {
        std::cout << "input number:" << std::endl;
        std::cin >> input;
        if (input == 'q')
            break;
        std::thread client_thread(client);
        client_thread.join();
    }
    server_thread.join();
    return 0;
}

void client()
{
	//第1步：创建客户端的socket。
	int sockfd;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return;
	}



	//第2步：向服务器发起连接请求。
	struct hostent* h;
	/* if((h = gethostbyname(argv[1])) == 0) */
	if((h = gethostbyname("127.0.0.1")) == 0)
	{
		printf("gethostbyname failed.\n");
		close(sockfd);
		return;
	}
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5005);
	/* servaddr.sin_port = htons(atoi(argv[2])); */
	memcpy(&servaddr.sin_addr, h->h_addr, h->h_length);

	if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
	{
		perror("connect");
		close(sockfd);
		return;
	}
	
	char buffer[1024];
	memset(buffer, 0, sizeof(buffer));
	

	//第3步：与服务端通信，发送一个报文后等待回复，然后再发下一个报文。
	for(int ii=0; ii<3; ii++)
	{
		int iret;
		sprintf(buffer, "这是第%d个超级女生，编号%03d。", ii+1, ii+1);
		
		if((iret = send(sockfd, buffer, strlen(buffer), 0)) <= 0)
		{
			perror("send");
			break;
		}
		printf("发送：%s\n", buffer);

		memset(buffer, 0, sizeof(buffer));
		
		if((iret = recv(sockfd, buffer, sizeof(buffer), 0)) <= 0)
		{
			printf("iret = %d\n", iret);
			break;
		}
		printf("接收：%s\n", buffer);
	}
	
	//第4步：关闭socket，释放资源。
	close(sockfd);	
}

void server()
{
	// 第1步：创建服务端的socket。
	int listenfd;
    // AF_INET 表示 localhost, SOCK_STREAM 是有连接的 tcp 协议
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return;
	}



	//第2步：把服务端用于通信的地址和端口绑定到socket上。
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//servaddr.sin_addr.s_addr = inet_addr("192.168.190.134);
	servaddr.sin_port = htons(5005);
	/* servaddr.sin_port = htons(atoi(argv[1])); */
	
	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0)
	{
		perror("bind");
		close(listenfd);
		return;
	}


	//第3步，把socket设置为监听模式。
	if(listen(listenfd, 5) != 0)
	{
		perror("listen");
		close(listenfd);
		return;
	}


    int clientfd;
    int socklen = sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr;

    int i = 3;
    while (i--)
    {
        //第4步：接受客户端的连接。
        clientfd = accept(listenfd, (struct sockaddr*)&clientaddr, (socklen_t*)&socklen);
        printf("客户端（%s）已连接。\n", inet_ntoa(clientaddr.sin_addr));

        //第5步：与客户端通信，接受客户端发过来的报文后，回复ok。
        char buffer[1024];
        while(1)
        {
            int iret;
            memset(buffer, 0, sizeof(buffer));
            if((iret = recv(clientfd, buffer, sizeof(buffer), 0)) <= 0 )
            {
                printf("iret = %d\n", iret);
                break;
            }
            printf("接收：%s\n", buffer);

            strcpy(buffer, "ok");
            if((iret = send(clientfd, buffer, strlen(buffer), 0)) <= 0)
            {
                perror("send");
                break;
            }
            printf("发送：%s\n",buffer);
        }
            
    }

	//第6步：关闭socket，释放资源。
	close(listenfd);
	close(clientfd);

}
