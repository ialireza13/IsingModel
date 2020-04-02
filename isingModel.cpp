#include <iostream>
#include <math.h>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <string>
using namespace std;

float T, dt;
int* arr;
int n, spf;

float energy_change(int i, int j){
	int left, right, top, bottom;
	if (i == 0) left = arr[(n-1)*n + j]; else left = arr[(i-1)*n + j];
	if (i == n-1) right = arr[0 + j]; else right = arr[(i+1)*n + j];
	if (j == 0) top = arr[i*n + (n-1)]; else top = arr[i*n + (j-1)];
	if (j == n-1) bottom = arr[i*n + 0]; else bottom = arr[i*n + (j+1)];
	return 2.0 * arr[i*n + j] * (left + right + top + bottom);
}

void simulate(){
	string filename;

	time_t time1 = time(NULL);
	int num=0, i, j, dE, m, x, y, up=0, down=0;
	float r;
	do{
		filename = "plot" + to_string(num) + ".xpm";
		num++;
		ofstream output(filename.c_str());
		output<<"/* XPM */\n"<<
				"static char * dla0025.ppm[] = {\n"<<
				"\""<<n<<" "<<n<<" 2 1\",\n"<<
				"\"0	c #55fd4f\",\n"<<
				"\"1	c #1100fd\",\n";
		for(x = 0; x < n; x++){
			output<<"\"";
			for(y = 0; y < n; y++){
				if (arr[x*n + y] == -1)
					output<<0;
				else
					output<<1;
			}
			output<<"\",\n";
		}
		output.close();
		for(m = 0; m <= spf; m++){
			r = rand()/(RAND_MAX + 1.0) * n;
			i = r;
			r = rand()/(RAND_MAX + 1.0) * n;
			j = r;
			dE = energy_change(i, j);
			if ((dE <= 0) || rand()/(RAND_MAX+1.0) < exp(-dE/T)){
				arr[i*n + j] *= -1;
			}
		}
	} while((time(NULL) - time1) < dt);
	for(x = 0; x < n; x++){
		for(y = 0; y < n; y++){
			if (arr[x*n + y] == 1)
				up++;
			else
				down++;
		}
	}
	cout<<"\nUp = "<<up<<'\n';
	cout<<"Down = "<<down<<"\n\n";
	float M = (up-down)/(n*n);
	cout<<"Magnetization = "<<M<<'\n';
}


int main(){
	srand(time(NULL));
	cout<<"Enter lattice size: "; cin>>n;
	cout<<"Enter run time length: "; cin>>dt;
	cout<<"Enter step per frame: "; cin>>spf;
	cout<<"Enter temperature: "; cin>>T;
	arr = new int[n*n];

	//Filling the array with +1 , -1
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			arr[i*n + j]=int(rand()/(RAND_MAX+1.0)*2)*2-1;
		}
	}
	simulate();
	delete [] arr;
	return 0;
}
