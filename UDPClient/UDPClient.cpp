/*
	Simple udp client
*/
#include <fstream>
#include <iostream>
#include <cstdio>
#include<stdio.h>
#include<winsock2.h>
#include "Ws2tcpip.h"
#include <string>
//файл изображения должен быть создан C:\img\image.png, затем он считывается и передаётся

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 80	//The port on which to listen for incoming data

int leng;

unsigned  char* readFileBytes(const char* name)
{
	FILE* fp;
	fp = fopen("C:\\img\\image.png", "rb");
	fseek(fp, 0, SEEK_END);
	leng = ftell(fp);
	rewind(fp);
	unsigned char * file_data = (unsigned char*)malloc((leng + 1) * sizeof(char));
	fread(file_data, leng, 1, fp);
	return file_data;
}

std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	return r;
}

int main(void)
{
	Sleep(2000);
	struct sockaddr_in si_other;
	int slen = sizeof(si_other);
	SOCKET s;
	char buf[BUFLEN];
	char message[BUFLEN];
	unsigned char* img;
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	in_addr ips;
	

	std::string ds = SERVER;

	std::wstring stemp = s2ws(ds);
	LPCWSTR result = stemp.c_str();

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
	InetPton(AF_INET, result, &ips);
	si_other.sin_addr = ips;

	//FILE image = new FILE(_iobuf::)

	//start communication
	while (1)
	{
		printf("Preparing an image");
		
		//файл изображения должен быть уже создан, затем он считывается и передаётся
		img = readFileBytes("C:\\img\\image.png");

		std::string filename = "123456filename.png";

		char* mess = new char[leng + 18];

		char* ggg = new char[30];

		strcpy(ggg,filename.c_str());

		strcpy(mess, ggg);

		memcpy(mess+18,img,leng);

		//strcat(mess,img);

		if (sendto(s, mess, leng+18, 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		memset(buf, '\0', BUFLEN);

		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		puts(buf);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}