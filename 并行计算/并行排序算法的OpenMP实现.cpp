#ifdef LOCAL
#define _CRT_SECURE_NO_WARNINGS
#include<conio.h>
void pause();
#endif

#ifdef DEBUG
#endif

#include<cstdio>
#include<omp.h>
#include<ctime>
#include<algorithm>
#include<iostream>
#include<vector>
#include<climits>
#include<utility>
#include<functional>
using namespace std;

void parallel_sort(int *a, int *b, int n, int p)
{
	int tid, i, j, s;
	omp_set_num_threads(p);
	s = n / p;	//s - ���ֿ�Ĵ�С
#pragma omp parallel private(tid,i)
	{
		//���Ȼ��� �ֲ�����
		tid = omp_get_thread_num();
		if (tid == p - 1)
			sort(a + tid*s, a + n);
		else
			sort(a + tid*s, a + (tid + 1)*s);
		//�������
		int w = 0;
		for (i = tid*s; i < (tid + 1)*s; i += s / p)
		{
			b[tid*p + w++] = a[i];
		}
	}
	//��������
	sort(b, b + p*p);
	//ѡ����Ԫ
	vector<int> c(p - 1);
	for (i = p; i < p*p; i += p)
	{
		c[i / p - 1] = b[i];
	}
	//��Ԫ����
	vector<vector<int> > section(p), num(p);	//��¼���ε���㼰��С
	for (i = 0; i < p; i++)
	{
		section[i].resize(p);
		num[i].resize(p, 0);
	}
#pragma omp parallel private(tid,i)
	{
		tid = omp_get_thread_num();
		int k = 0;
		section[tid][0] = tid*s;
		for (i = tid*s; i < (tid + 1)*s; i++)
		{
			while (c[k] < a[i] && k < p - 1)
				section[tid][++k] = i;
			num[tid][k]++;
		}
		if (tid == p - 1)
		{
			for (i = (tid + 1)*s; i < n; i++)
			{
				while (c[k] < a[i] && k < p - 1)
					section[tid][++k] = i;
				num[tid][k]++;
			}
		}
	}
	//�鲢����
	vector<int> st;
	st.push_back(0);
	for (j = 0; j < p; j++)
	{
		int sum = 0;
		for (i = 0; i < p; i++)
			sum += num[i][j];
		st.push_back(sum + st[j]);
	}
#pragma omp parallel private(tid,i)
	{
		tid = omp_get_thread_num();
		int r = st[tid];
		vector<int> k(p);	//p���α�
		for (i = 0; i < p; i++)	//�α긳��ʼֵ
		{
			k[i] = section[i][tid];
		}
		for (;;)
		{
			int min = INT_MAX, minP;
			for (i = 0; i < p; i++)
			{
				if (k[i] - section[i][tid] < num[i][tid])
				{
					if (min > a[k[i]])
					{
						min = a[k[i]];
						minP = i;
					}
				}
			}
			if (min == INT_MAX)
			{
				break;
			}
			else
			{
				b[r++] = min;
				k[minP]++;
			}
		}
	}
}

int main()
{
	int n = 100000000, p = 2;
	clock_t pt, st;
	int* a = new int[n];
	int* b = new int[n];
	int* c = new int[n];
	freopen("out.txt", "w", stdout);
	while (p <= 64)
	{
		for (int r = 0; r < 5; r++)
		{
			pt = 0, st = 0;
			for (int i = 0; i < n; i++)
				a[i] = i;
			random_shuffle(a, a + n);

			/*��������ʱ��*/
			if (p==2)
			{
				for (int i = 0; i < n; i++)
					c[i] = a[i];
				st -= clock();
				sort(c, c + n);
				st += clock();
			}
			/*��������ʱ��*/
			pt -= clock();
			parallel_sort(a, b, n, p);
			pt += clock();
			for (int i = 0; i < n; i++)
				if (b[i] != i)
				{
					cout << "error!" << endl;
					break;
				}
			cout << p << endl;
			cout << "serial execution time is " << st / 1000.0 << " seconds" << endl;
			cout << "parallel execution time is " << pt / 1000.0 << " seconds" << endl;
			cout << "speedup ratio is " << st*1.0 / pt << endl;
			cout << endl;
		}
		cout << endl;
		p *= 2;
	}
	delete[] a;
	delete[] b;
	delete[] c;
#ifdef LOCAL
	pause();
#endif

	return 0;
}

#ifdef LOCAL
void pause()
{
	printf("Press any key to continue. . . \n");
	_getch();
}
#endif