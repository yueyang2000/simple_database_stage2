#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
using std::string;
using std::cout;
extern string operation;
void Process_operation(std::string&);
int server()
{
    struct sockaddr_in server_addr;
    server_addr.sin_len = sizeof(struct sockaddr_in);
    server_addr.sin_family = AF_INET;//Address families AF_INET互联网地址簇
    server_addr.sin_port = htons(11332);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    bzero(&(server_addr.sin_zero),8);

    //创建socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);//SOCK_STREAM 有连接
    if (server_socket == -1) {
        perror("socket error");
        return 1;
    }
    int bind_result =bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_result == -1) {
        perror("bind error");
        return 1;
    }
    if (listen(server_socket, 5) == -1) {
        perror("listen error");
        return 1;
    }

    struct sockaddr_in client_address;
    socklen_t address_len;
    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &address_len);

    if (client_socket == -1) {
        perror("accept error");
        return -1;
    }
    
    char recv_msg[1024];
    char reply_msg[1024];
    send(client_socket,"hello\n",1024,0);
    while (1) {
        bzero(recv_msg, 1024);
        bzero(reply_msg, 1024);
        
        long byte_num = recv(client_socket,recv_msg,1024,0);
        recv_msg[byte_num] = '\0';
        std::string receive(recv_msg);
        if(receive=="quit"){
            send(client_socket, "Bye", 1024, 0);
            break;
        }
        cout<<"client said:"<<receive<<'\n';
        std::streambuf* backup;
        backup=cout.rdbuf();
        std::ofstream fout;
        fout.open("buffer");
        cout.rdbuf(fout.rdbuf());
        Process_operation(receive);
        cout.rdbuf(backup);
        fout.close();
    
        std::ifstream fin;
        fin.open("buffer");
        string reply;
        string temp;
        while(getline(fin,temp)){
            reply+=temp+"\n";
        }
        fin.close();
        send(client_socket, reply.c_str(), 1024, 0);
    }
    fclose(stdout);
    return 0;
}