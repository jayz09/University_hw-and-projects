#include <iostream>
#include <ctime>
#define ROW 3
#define COL 3
using namespace std;

/*
				HW 2 (5pts, Due: 10/4 23:59PM)
Provide a class Matrix to complete this code.
This class should support the following functionalities:

1. Matrix Creation
A 3 by 3 matrix is created. All values are intialized as non-zero integers.
You may want to use rand for this.

2. Matrix Addtion and Multiplication
You may want to use operator overloading.

3. Display
The values of a matrix are "well" displayed.

Submit this file to the LMS.
*/

//Your code goes here.

class Matrix {
	int arr[ROW][COL];
	static bool init;
public:
	Matrix(void);
	friend Matrix operator+(Matrix a, Matrix b);
	friend Matrix operator*(Matrix c, Matrix d);
	void show(void);
};

bool Matrix::init = false;

Matrix::Matrix(void) {
	if (!init) {
		init = true;
		srand(time(NULL));
	}

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			arr[i][j] = rand() % 10;
		}
	}
}

Matrix operator+(Matrix a, Matrix b) {
	Matrix tmp;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			tmp.arr[i][j] = a.arr[i][j] + b.arr[i][j];
		}
	}
	return tmp;
}

Matrix operator*(Matrix c, Matrix d) {
	Matrix tmp;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			tmp.arr[i][j] = 0;
			for (int k = 0; k < COL; k++) {
				tmp.arr[i][j] += c.arr[i][k] * d.arr[k][j];
			}
		}
	}
	return tmp;
}


void Matrix::show(void) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}



//DO NOT TOUCH THE MAIN FUNCTION.
int main() {

	Matrix A, B, C;		// 3 by 3 matrix created. All values are intialized as non-zero integers.

	Matrix D = A + B;	// Matrix Addition
	Matrix E = A * C;	// Matrix Multiplication

	cout << "A" << endl;
	A.show();		// display the values
	cout << "B" << endl;
	B.show();
	cout << "C" << endl;
	C.show();


	cout << "A + B" << endl;
	D.show();

	cout << "A * C" << endl;
	E.show();

	return 0;
}