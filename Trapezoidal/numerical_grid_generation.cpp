// Exp. in pg. 351
#include<iostream>
#include<stdlib.h>
#include<string>
#include<cmath>
#include<fstream>
#include<sstream>
#include<vector>
#include <iomanip> //for setprecision
using namespace std;
typedef double T;
T kesitox(T kesi, T aita);
T kesi_aita_toy(T kesi, T aita, T L, T H1, T H2);
T analytical_kesi_subx(T kesi, T aita, T L, T H1, T H2);

int main()
{
	unsigned int IM = 17, JM = 13;
	unsigned int L = 4, H1 = 2, H2 = 4;
	T dkesi = L/(IM - 1.);
	T daita = 1.0/(JM - 1);

	vector<vector<T>> grid;

	T kesi, aita;
	for(unsigned int j = 0; j < JM; j++)
		for(unsigned int i = 0; i < IM; i++)
		{
			kesi = i*dkesi;
			aita = j*daita;
			vector<T> gridlocal(2);
			gridlocal[0] = kesitox(kesi, aita);
			gridlocal[1] = kesi_aita_toy(kesi, aita, L, H1, H2);
			grid.push_back(gridlocal);
		}

	for(unsigned int j = 0; j < JM; j++)
		for(unsigned int i = 0; i < IM; i++)
	{	//error between analytical results and numerical results
		T kesi_subx_num, kesi_subx_ana, x_subkesi, x_subaita, y_subkesi, y_subaita;
		kesi = i*dkesi;
		aita = j*daita;
		//kesi_subx_ana = analytical_kesi_subx(kesi, aita, L, H1, H2);
		if(i == 0)
		{
			x_subkesi = (0. - 3.*grid[i+j*IM][0] + 4.*grid[i+1+j*IM][0] - grid[i+2+j*IM][0])/(2.*dkesi);
			y_subkesi = (0. - 3.*grid[i+j*IM][1] + 4.*grid[i+1+j*IM][1] - grid[i+2+j*IM][1])/(2.*dkesi);
		}else if(i == IM-1)
		{
			// here - symbol apperaed can be taken as a reference in https://en.wikipedia.org/wiki/Finite_difference
			x_subkesi = -(0. - 1.*grid[i-2+j*IM][0] + 4.*grid[i-1+j*IM][0] - 3.*grid[i+j*IM][0])/(2.*dkesi);
			y_subkesi = -(0. - 1.*grid[i-2+j*IM][1] + 4.*grid[i-1+j*IM][1] - 3.*grid[i+j*IM][1])/(2.*dkesi);
		}else
		{
			x_subkesi = (grid[i+1+j*IM][0] - grid[i-1+j*IM][0])/(2.*dkesi);
			y_subkesi = (grid[i+1+j*IM][1] - grid[i-1+j*IM][1])/(2.*dkesi);
		}

		if(j == 0)
		{
			x_subaita = (0. - 3.*grid[i+j*IM][0] + 4.*grid[i+(j+1)*IM][0] - grid[i+(j+2)*IM][0])/(2.*daita);
			y_subaita = (0. - 3.*grid[i+j*IM][1] + 4.*grid[i+(j+1)*IM][1] - grid[i+(j+2)*IM][1])/(2.*daita);
		}else if(j == JM-1)
		{
			x_subaita = -(0. - 1.*grid[i+(j-2)*IM][0] + 4.*grid[i+(j-1)*IM][0] - 3.*grid[i+j*IM][0])/(2.*daita);
			y_subaita = -(0. - 1.*grid[i+(j-2)*IM][1] + 4.*grid[i+(j-1)*IM][1] - 3.*grid[i+j*IM][1])/(2.*daita);
		}else{
			x_subaita = (grid[i+(j+1)*IM][0] - grid[i+(j-1)*IM][0])/(2.*daita);
			y_subaita = (grid[i+(j+1)*IM][1] - grid[i+(j-1)*IM][1])/(2.*daita);
		}

		T Jacobian = 1./(x_subkesi*y_subaita - y_subkesi*x_subaita);
		kesi_subx_num = Jacobian*y_subaita;
		grid[i+j*IM].push_back(kesi_subx_num);
		//T error = kesi_subx_num - kesi_subx_ana;
		//cout<<setprecision(20)<<kesi_subx_ana<<" "<<kesi_subx_num<<" "<<error<<endl;
	}

	ostringstream name;
	name<<"grid.dat";
	ofstream out(name.str().c_str());
	out<<"Title=\"Grid generated by using algebraic grid generation techniques\"\n"<<"VARIABLES=\"X\",\"Y\",\"kesi\",\"aita\",\"kesi_subx_num\"\n";
	out<<"ZONE T = \"BOX\", I = "<<IM<<", J = "<<JM<<", F = POINT"<<endl;
	for(unsigned int j = 0; j < JM; j++)
		for(unsigned int i = 0; i < IM; i++)
		{
			kesi = i*dkesi;
			aita = j*daita;
			// x axis is kesi, y axis is aita and z axis is kesi_subx then we can get the Fig. (9-6a) in pg. 356
			out<<grid[i+j*IM][0]<<" "<<grid[i+j*IM][1]<<" "<<kesi<<" "<<aita<<" "<<grid[i+j*IM][2]<<endl;
		}
}

T kesitox(T kesi, T aita)
{
	T x = kesi;
	return x;
}
T kesi_aita_toy(T kesi, T aita, T L, T H1, T H2)
{
	T y = (H1 + (H2 - H1)/L*kesi)*aita;
	return y;
}
T analytical_kesi_subx(T kesi, T aita, T L, T H1, T H2)
{
	//this formula is from pg. 354 of Eq. (9-29)
	T tmp = aita/(H1 + (H2 - H1)/L*kesi);
	T kesi_subx = -(H2 - H1)/L*tmp;
	return kesi_subx;
}