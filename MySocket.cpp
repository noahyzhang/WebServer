#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#include "MySocket.h"

#pragma comment(lib,"ws2_32.lib")

bool MySocket::init_winsocket = false;
MySocket::MySocket()
{
	if (!init_winsocket)
	{
		WSADATA info;
		if (WSAStartup(MAKEWORD(2, 2), &info))
			throw "Coule not start WSA";
		init_winsocket = true;
	}
	socket_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == socket_)
	{
		throw "Invalid socket";
	}
}

MySocket::MySocket(SOCKET new_socket)
{
	socket_ = new_socket;
}

MySocket::~MySocket()
{
}

bool MySocket::SendLine(std::string buffer)  //发送数据
{
	buffer += '\n';
	return SOCKET_ERROR != send(socket_, buffer.c_str(), buffer.length(), 0);
}

std::string  MySocket::ReceiveLine()    
{
	std::string ret;
	while (true)
	{
		char buf;
		switch (recv(socket_, &buf, 1, 0))
		{
		case 0:
			return ret;
		case -1:
			return "";
		}
		ret += buf;
		if (buf == '\n')
		{
			break;
		}
	}
	return ret;
}

SocketClient::SocketClient(const std::string &host, int port)   //socket客户端
{
	bool isok = false;
	do {

		hostent *he = gethostbyname(host.c_str());
		if (!he) {
			break;
		}
		sockaddr_in addr = { 0 };
		addr.sin_family = AF_INET;
		addr.sin_port = htons(port);
		addr.sin_addr = *((in_addr*)he->h_addr);
		if (SOCKET_ERROR == connect(socket_, (sockaddr*)&addr, sizeof(sockaddr)))
		{
			break;
		}
		isok = true;
	} while (FALSE);

	if (!isok)
	{
		char error_msg[MAXBYTE] = { 0 };
		strerror_s(error_msg, WSAGetLastError());
		throw error_msg;
	}

}

SocketServer::SocketServer(int port, int connections,  TypeSocket type) //端口，接收的连接数
{
	sockaddr_in addr = { 0 };
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	if (type == unBlockingSocket)
	{
		//设置非阻塞
		unsigned long arg = 1;
		ioctlsocket(socket_, FIONBIO,&arg);
	}
	if (SOCKET_ERROR == bind(socket_,(sockaddr*)&addr, sizeof(sockaddr)))
	{
		//在抛出异常前，保存现场
		Close();
		throw "Bind Error";
	}
	listen(socket_, connections);
}

void MySocket::Close()
{
	closesocket(socket_);
}

MySocket* SocketServer::Accept()  //accept服务器阻塞等待客户端连接
{
	MySocket *accept_socket = new MySocket(accept(socket_, nullptr, nullptr));
	if (*accept_socket == INVALID_SOCKET)
	{
		throw "Invalid socket";
	}
	return accept_socket;
}