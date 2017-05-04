#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>
#include <math.h>
#define N 38
#define M 6
#define K 32
using namespace std;
bool btoc(char arr[4], bool x[32])
{
    for(int i = 0; i < 4; ++i)
	{
		arr[i] = 0;
		for(int j = 0; j < 8; ++j)
		{
            arr[i] += (int)pow(2, j) * x[i * 8 + j];
		}
	}
}
int error(bool syn[M], bool err[N][M])
{
	bool flag = 0;
    for(int i = 0; i < 6; ++i)
    	if(syn[i])
    		flag = 1;
	if(flag == 0)
		return 0;
    for(int i = 0; i < N; ++i)
	{
		for(int j = 0; j < M; ++j)
		{
			if(syn[j] == err[i][j] && j == M - 1)
				return i;
            if(syn[j] == err[i][j])
            	continue;
			else
				break;
		}
	}
	return -1;
}
int main()
{
        char ip[16] = "127.0.0.1";
        int fd = socket(AF_INET, SOCK_STREAM, 0);
        if(fd < 0)
        {
                printf("Error creating socket\n");
                return -1;
        }
        struct sockaddr_in s_addr;
        s_addr.sin_family = AF_INET;
        s_addr.sin_port = htons(8271);
        if(inet_pton(AF_INET, ip, &s_addr.sin_addr)<=0)
        {
                printf("Error occured converting IP address\n");
                return -1;
        }
        if(connect(fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0)
        {
                printf("Error, connect failed\n");
                return -1;
        }

        int n;
        fstream f1("H.txt", ios::in);
        ofstream f2("Out.txt", ios::out);
		fstream f3("Errors.txt", ios::in);
		bool err[N][M];
		for(int i = 0; i < N; ++i)
			for(int j = 0; j < M; ++j)
				f3>>err[i][j];
		int excep[M] = {0, 1, 2, 3, 6, 12};
		int gen[M][N] = {0};
		for(int i = 0; i < M; i++)
			for(int j = 0; j < N; ++j)
				f1>>gen[i][j];
		int amount;
		int frames = 0;
		read(fd, &frames, sizeof(frames));
		cout<<frames<<" frames being delivered\n";
		for(int frame = 0; frame < frames; ++frame)
        {
        	bool message[38] = {0};
        	bool decoded[32] = {0};
        	read(fd, message, sizeof(message));

			bool syn[M] = {0};
			// Decode phase
			for(int i = 0; i < M; ++i)
				for(int j = 0; j < N; ++j)
					syn[i] = (syn[i] + (gen[i][j] * message[j])) % 2;
            int chk = error(syn, err);
            bool ack = 0;
            if(chk && chk != -1)
			{
				cout<<"Frame "<<frame<<": Error detected and corrected at position "<<chk<<"\n";
                message[chk] = (message[chk] + 1) % 2;
                ack = 1;
                write(fd, &ack, sizeof(ack));
			}
			else if(!chk)
			{
				cout<<"Frame "<<frame<<" received successfully\n";
				ack = 1;
                write(fd, &ack, sizeof(ack));
			}
			else
			{
				cout<<"Frame "<<frame<<" error detected, requesting retransmission\n";
				ack = 0;
                write(fd, &ack, sizeof(ack));
                --frame;
                continue;
			}
			int counter = 0;
			for(int i = 0; i < 38; ++i)
			{
				bool flag = 0;
                for(int j = 0; j < 6; ++j)
                	if(i == excep[j])
                        flag = 1;
				if(flag)
					continue;
                decoded[counter++] = message[i];
			}
			char msg[4];
            btoc(msg, decoded);
            f2.write((char *)msg, sizeof(msg));
        }
        f1.close();
        f2.close();
        f3.close();
        cin.get();
        cin.get();
}
