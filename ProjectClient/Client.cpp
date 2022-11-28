#pragma comment(lib, "ws2_32")
#include <iostream>
#include <winsock2.h>
#include <thread>
using namespace std;
#define PACKET_SIZE 1024

void recv_data(SOCKET& s) {
	char buf[PACKET_SIZE];

	while (1) {
		ZeroMemory(buf, PACKET_SIZE);
		recv(s, buf, PACKET_SIZE, 0);

		if (WSAGetLastError()) break; //�������� ����

		cout << "\n[Server] >> " << buf << "\n���� �����͸� �Է� >> ";
	}

	return;
}

int main() {
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa)) {
		cout << "WSA error";
		WSACleanup();
		return 0;
	}

	SOCKET skt;
	skt = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (skt == INVALID_SOCKET) {
		cout << "socket error";
		closesocket(skt);
		WSACleanup();
		return 0;
	}

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4444);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	while (connect(skt, (SOCKADDR*)&addr, sizeof(addr)));

	char buf[PACKET_SIZE];

	recv(skt, buf, PACKET_SIZE, 0); //�ڽ��� ������ Ŭ���̾�Ʈ��ȣ ����
	int mynum = atoi(buf); //char������ int������ ����ȯ
	sprintf(buf, "[%d] %s::%d", mynum, inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
	// �ܼ��������� >> [���� ������ Ŭ���̾�Ʈ��ȣ] [����������]::[������Ʈ]
	SetConsoleTitle(buf);

	thread(recv_data, ref(skt)).detach();

	while (!WSAGetLastError()) {
		cout << "���� �����͸� �Է� >> ";
		cin >> buf;

		send(skt, buf, strlen(buf), 0); // ����������
	}

	closesocket(skt);
	WSACleanup();
}