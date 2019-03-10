#include"MySocket.h"
#include <iostream>
#include<fstream>
#include<process.h>


unsigned int __stdcall Connection(void* p)
{
	MySocket* accept_socket = (MySocket*)p;
	std::ifstream file("E://1.txt");
	std::string buf;
	while (getline(file,buf))
	{
		accept_socket->SendLine(buf);
	}
	//accept_socket->SendLine("<html><body><h1>我是张意< / h1><p>阿里巴巴员工< / p>< / body>< / html>");
	while (1)
	{
		std::string buffer = accept_socket->ReceiveLine();
		if (buffer.empty())
			break;
		std::cout << buffer << std::endl;
	}

	delete accept_socket;
	return 0;
}

int main()
{
	//try
	//{
	//	SocketClient web_read("www.baidu.com", 80);
	//	web_read.SendLine("GET / HTTP/1.0");
	//	web_read.SendLine("Host: www.baidu.com");
	//	web_read.SendLine("");

	//	while (true)
	//	{
	//		//首页的信息
	//		std::string buf = web_read.ReceiveLine();
	//		if (buf.empty())
	//			break;
	//		std::cout << buf;
	//	}
	//}
	//catch (const char* error_msg)
	//{
	//	std::cout << error_msg << std::endl;
	//}

	try
	{
		SocketServer web_server(80, 10);
		while (true)
		{
			MySocket* accept_socket = web_server.Accept();
			//CloseHandle((HANDLE)_beginthreadex(nullptr, 0, Connection, accept_socket, 0, nullptr));
			accept_socket->SendLine("<html><body><h1>我是张意< / h1><p>阿里巴巴员工< / p>< / body>< / html>");
		}

	}
	catch (const char* e)
	{
		std::cout << e << std::endl;
	}
	catch (...)
	{
		std::cout << "Unkown Error" << std::endl;
	}

	return 0;
}
