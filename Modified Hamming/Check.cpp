#include <fstream>
#include <iostream>
#include <math.h>
#include <stdlib.h>
using namespace std;
#define N 38
#define K 32
#define M 6

int find(int err[M])
{
	int flag = 0;
	for(int i = 0; i < M; ++i)
		if(err[i] != 0)
            flag = 1;
	if(flag == 0)
		return -1;
	ifstream ch("Checker.txt", ios::in);
    char check[N][M];
    for(int i = 0; i < N; ++i)
    	for(int j = 0; j < M; ++j)
    		ch>>check[i][j];
	for(int i = 0; i < N; ++i)
	{
		int flag = 0;
		for(int j = 0; j < M; ++j)
			if((check[i][j] - '0') != err[j])
				flag = 1;
		if(flag == 0)
			return i + 1;
	}
	return 10000;
}

void gener(int arr[K], long c)
{
    for(int i = 0; i < K; ++i)
	{
        if(c & (1<<i))
        	arr[i] = 1;
		else
			arr[i] = 0;
	}
}

int main()
{
	fstream f1("H.txt", ios::in);
	fstream f3("Errors.txt", ios::in);
	ofstream f2("Test.txt", ios::out);
	bool err[N][K];
	for(int i = 0; i < N; ++i)
		for(int j = 0; j < M; ++j)
			f3>>err[i][j];
	int excep[M] = {0, 1, 2, 3, 6, 12};
	int gen[M][N] = {0};
	for(int i = 0; i < M; i++)
    	for(int j = 0; j < N; ++j)
			f1>>gen[i][j];

	for(long abcd = 0; abcd < 38; ++abcd)
	{
		long abc = 130;
		int data[K] = {0};
		int coded[N] = {0};
		gener(data, abc);

		int dec[M] = {0};
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
				coded[i] = data[c1++];
		}

		int syn[M] = {0};

		// Decode phase
		for(int i = 0; i < M; ++i)
			for(int j = 0; j < N; ++j)
				syn[i] = syn[i] ^ (gen[i][j] * coded[j]);

		for(int i = 0; i < M; ++i)
			f2<<syn[i];
		f2<<"\n";
	}
}

/*cout<<"Enter 1 bit or 2 bit";
		int opt;
		cin>>opt;
		cout<<"Enter pos";
		int pos;
		cin>>pos;
		pos -= 1;
		switch(opt)
		{
			case 1:
				code[pos] = (code[pos] + 1) % 2;
			break;
			case 2:
				code[pos] = (code[pos] + 1) % 2;
				code[pos + 1] = (code[pos + 1] + 1) % 2;
			break;
			case 3:
				code[pos] = (code[pos] + 1) % 2;
				code[pos + 1] = (code[pos + 1] + 1) % 2;
				code[pos + 2] = (code[pos + 2] + 1) % 2;
			break;
		}*/
