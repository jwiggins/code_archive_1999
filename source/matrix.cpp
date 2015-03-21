#include <iostream.h>
void multiply(int matrix_one[2][2],int matrix_two[2][2]);
void main ()
{
	cout << "This program multiplies 2 2x2 matrices." << endl;
	int matrix_one[2][2],matrix_two[2][2]; //define the matrices
	int a,b,c,d; //first matrix variables
	int e,f,g,h; //second matrix variables
	
	cout << "Input four numbers: ";
	cin >> a >> b >> c >> d;
	cout << "This is your first matrix." << endl;
	matrix_one[0][0]=a;
	cout << matrix_one[0][0] << " ";
	matrix_one[0][1]=b;
	cout << matrix_one[0][1] << endl;
	matrix_one[1][0]=c;
	cout << matrix_one[1][0] << " ";
	matrix_one[1][1]=d;
	cout << matrix_one[1][1] << endl;
	
	cout << "Input four more numbers: ";
	cin >> e >> f >> g >> h;
	cout << "This is your second matrix." << endl;
	matrix_two[0][0]=e;
	cout << matrix_two[0][0] << " ";
	matrix_two[0][1]=f;
	cout << matrix_two[0][1] << endl;
	matrix_two[1][0]=g;
	cout << matrix_two[1][0] << " ";
	matrix_two[1][1]=h;
	cout << matrix_two[1][1] << endl;
	
	cout << "Now we multiply" << endl;
	multiply(matrix_one,matrix_two);
	return;
}
void multiply(int matrix_one[2][2],int matrix_two[2][2])
{
	int matrix_end[2][2];
	int i,j,k,l;
	
	matrix_end[0][0]= (matrix_one[0][0]*matrix_two[0][0]) + (matrix_one[0][1]*matrix_two[1][0]);
	cout << matrix_end[0][0] << " ";
	matrix_end[0][1]= (matrix_one[0][0]*matrix_two[0][1]) + (matrix_one[0][1]*matrix_two[1][1]);
	cout << matrix_end[0][1] << endl;
	matrix_end[1][0]= (matrix_one[1][0]*matrix_two[0][0]) + (matrix_one[1][1]*matrix_two[1][0]);
	cout << matrix_end[1][0] << " ";
	matrix_end[1][1]= (matrix_one[1][0]*matrix_two[0][1]) + (matrix_one[1][1]*matrix_two[1][1]);
	cout << matrix_end[1][1] << endl;
	return;
}