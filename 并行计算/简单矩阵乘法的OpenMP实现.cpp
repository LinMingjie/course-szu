#ifdef LOCAL
#define _CRT_SECURE_NO_WARNINGS
#include<conio.h>
void pause();
#endif

#ifdef DEBUG
#endif

//用静态数组会更快
#include<cstdio>
#include<cstring>
#include<iostream>
#include<omp.h>
#include<ctime>
using namespace std;
int **a, **b, **c, n;
long long sum;

void init()
{
	a = new int*[n]; b = new int*[n]; c = new int*[n];
	for (int i = 0; i < n; i++)
	{
		a[i] = new int[n];
		b[i] = new int[n];
		c[i] = new int[n];
		for (int j = 0; j < n; j++)
		{
			a[i][j] = i + j + 1;
			b[i][j] = 1; c[i][j] = 0;
		}
	}
	//memset(c, 0, sizeof(c));
}

int main()
{
	//freopen("out.txt", "w", stdout);
	int i, j, k; clock_t t = 0;
	int r = 1;
	while (r--)
	{
		//	for (int p = 1; p <= 4; p *= 2)
		{
			int p;
			cin >> n >> p;
			omp_set_num_threads(p);
			cout << p << " threads" << endl;
			init();
			sum = t = 0;
			t -= clock();
#pragma omp parallel for  private(i,j,k) shared(a,b,c,n) schedule(static)
			for (i = 0; i < n; i++)
			{
				for (j = 0; j < n; j++)
				{
					for (k = 0; k < n; k++)
					{
						c[i][j] += a[i][k] * b[k][j];
					}
				}
			}
			t += clock();
			for (i = 0; i < n; i++)
				for (j = 0; j < n; j++)
					sum += c[i][j];
			cout << sum << endl;
			cout << "用时：" << t / 1000.0 << "秒" << endl;
		}
		//	cout << endl;
	}
	//fclose(stdout);
	for (i = 0; i < n; i++)
	{
		delete[]a[i]; delete[]b[i]; delete[]c[i];
	}
	delete[]a; delete[]b; delete[]c;
#ifdef LOCAL
	pause();
#endif
	return 0;
}

/*
#include<cstdio>
#include<cstring>
#include<iostream>
#include<omp.h>
#include<ctime>
using namespace std;
#define N 1000
int a[N][N], b[N][N], c[N][N];
long long sum;

void init()
{
	for (int i = 0; i < N; i++)
	for (int j = 0; j < N; j++)
	{
		a[i][j] = i + j + 1;
		b[i][j] = 1;
	}
	memset(c, 0, sizeof(c));
}

int main()
{
	//freopen("out.txt", "w", stdout);
	int  i, j, k; clock_t t = 0;
	int r = 5;
	while (r--)
	{
		int p = 64;
		//for (int p = 1; p <= 64; p *= 2)
		{
			omp_set_num_threads(p);
			cout << p << " threads" << endl;

			init();
			sum = t = 0;
			t -= clock();
#pragma omp parallel for  private(i,j,k) shared(a,b,c) schedule(static) 
			for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
			for (k = 0; k < N; k++)
				c[i][j] += a[i][k] * b[k][j];
			t += clock();
			for (i = 0; i < N; i++)
			for (j = 0; j < N; j++)
				sum += c[i][j];
			cout << sum << endl;
			cout << "用时：" << t / 1000.0 << "秒" << endl;
		}
		cout << endl;
	}
	//fclose(stdout);
#ifdef LOCAL
	pause();
#endif
	return 0;
}
*/

#ifdef LOCAL
void pause()
{
	cout << "Press any key to continue. . . ";
	_getch();
}
#endif