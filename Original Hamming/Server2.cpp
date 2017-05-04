#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <math.h>
using namespace std;
#define N 38
#define K 32
#define M 6
bool ctob(char arr[4], bool *x)
{
    for(int i = 0; i < 4; ++i)
		for(int j = 0; j < 8; ++j)
            x[i * 8 + j] = ((arr[i] & (int)pow(2, j)) ? 1 : 0);
}
void errorify(int con, bool copy[N], int frame, int frames)
{
	cout<<"Sent frame "<<frame<<"\n";
	bool code[N];
	for(int i = 0; i < N; ++i)
		code[i] = copy[i];
	int selection = rand() % 2 + 1;
	int pos = 0;//(rand() % (N - selection));
    if(frame < frames * 0.2 || frame > frames * 0.8)
    	selection = 4;
	switch(selection)
	{
		case 1:
			cout<<"One bit error introduced at "<<pos<<"\n";
			code[pos] = (code[pos] + 1) % 2;
		break;
		case 2:
			cout<<"Two bit burst error introduced at position "<<pos<<"\n";
			code[pos] = (code[pos] + 1) % 2;
			code[pos + 1] = (code[pos + 1] + 1) % 2;
		break;
		case 3:
			cout<<"Three bit burst error introduced at position "<<pos<<"\n";
			code[pos] = (code[pos] + 1) % 2;
			code[pos + 1] = (code[pos + 1] + 1) % 2;
			code[pos + 2] = (code[pos + 2] + 1) % 2;
		break;
		case 4:
			cout<<"No error introduced\n";
		break;
	}
	write(con, code, sizeof(code));
	bool ack = 0;
    read(con, &ack, sizeof(ack));
    if(ack == 0)
    	errorify(con, copy, frame, frames);
}
int main()
{

	// Create a file descriptor
	int fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in s_addr;
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	s_addr.sin_port = htons(8271);

	if(bind(fd, (struct sockaddr*)&s_addr, sizeof(s_addr)) < 0)
	{
			printf("Error on bind!\n");
			return -1;
	}
	else
			printf("Successfully started server on port %d\n", s_addr.sin_port);
	listen(fd, 10);
	fstream f1("H2.txt", ios::in);
	fstream f3("Errors2.txt", ios::in);
	bool err[N][K];
	for(int i = 0; i < N; ++i)
		for(int j = 0; j < M; ++j)
			f3>>err[i][j];
	int excep[M] = {0, 1, 4, 7, 15, 31};
	int gen[M][N] = {0};
	for(int i = 0; i < M; i++)
    	for(int j = 0; j < N; ++j)
			f1>>gen[i][j];

	int con = accept(fd, (struct sockaddr*)NULL, NULL);
	{
		char filename[1000];
        cout<<"Enter file to send\n";
		cin>>filename;
        ifstream mfile(filename, ios::in);
        mfile.seekg(0, ios::end);
        int r = mfile.tellg() / 4;
        mfile.seekg(0, ios::beg);
		write(con, &r, sizeof(r));
        for(int frame = 0; frame < r; ++frame)
		{
			char re[4];
			bool data[K] = {0};
			bool coded[N] = {0};
			int dec[M] = {0};
			mfile.read((char *)re, sizeof(re));
			ctob(re, data);
			for(int i = 0; i < M; ++i)
			{
				int c1 = 0;
				for(int j = 0; j < N; ++j)
				{
					bool flag = 0;
					for(int ab = 0; ab < M; ++ab)
						if(j == excep[ab])
							flag = 1;
					if(flag)
						continue;
					dec[i] = (dec[i] + (data[c1++] * gen[i][j])) % 2;
				}
			}

			int c1 = 0;
			int c2 = 0;
			for(int i = 0; i < N; ++i)
			{
				bool flag = 0;
				for(int ab = 0; ab < M; ++ab)
					if(i == excep[ab])
						flag = 1;
				if(flag)
					coded[i] = dec[c2++] % 2;
				else
					coded[i] = data[c1++] % 2;
			}

			// Introduce error
			errorify(con, coded, frame, r);

		}
	}
	//write(con, result2, sizeof(result2));
	//write(con, result, sizeof(result));

	close(con);
	cin.get();
	cin.get();
	return 0;
}
