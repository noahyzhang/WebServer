#pragma once
#include<iostream>
#include<string>
#include<WinSock2.h>

class MySocket
{
public:
	enum TypeSocket
	{
		BlockingSocket,
		unBlockingSocket
	};
	MySocket();
	MySocket(SOCKET new_socket);
	virtual ~MySocket();
	std::string ReceiveLine();
	bool SendLine(std::string buffer);
	void Close();
	bool operator==(unsigned long code) {
	
		return socket_ == code;
	}

protected:
	SOCKET socket_;
private:
	static bool init_winsocket;
};

class SocketClient :public MySocket {
public:
	SocketClient(const std::string &host, int port);  //地址和端口
};

class SocketServer :public MySocket {
public:

	SocketServer(int port, //端口
		int connections,  //接收的连接数
		TypeSocket type = BlockingSocket);
	MySocket* Accept();
};
