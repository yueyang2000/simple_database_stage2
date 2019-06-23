#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
using namespace std;

int main (int argc, const char * argv[])
{
    struct sockaddr_in server_addr;
    server_addr.sin_len = sizeof(struct sockaddr_in);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(11332);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_addr.sin_zero),8);

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket error");
        return 1;
    }
    char recv_msg[1024];
    char reply_msg[1024];

    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in))==0)     {
        long byte_num = recv(server_socket,recv_msg,1<<13,0);
        recv_msg[byte_num] = '\0';
        cout<<recv_msg;
        while (1) {
            cout<<"mysql>";
            bzero(recv_msg, 1024);
            bzero(reply_msg, 1024);
            string  reply;
            getline(cin,reply);
            send(server_socket, reply.c_str(), 1024, 0);

            long byte_num = recv(server_socket,recv_msg,1024,0);
            recv_msg[byte_num] = '\0';
            cout<<recv_msg;
            string receive(recv_msg);
            if(receive=="Bye"){break;}
        }
    }
    else{cout<<"link failed\n";}
    return 0;
}