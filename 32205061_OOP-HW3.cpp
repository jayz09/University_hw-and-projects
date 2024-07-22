#include <iostream>
#include <ctime>
#define ROW 3
#define COL 3
using namespace std;

/*
				HW 3 (7pts, Due: 10/13 23:59PM)
Complete a class Matrix2 by inheriting the class Matrix that you made in HW2.
Your Matrix2 class should support the following functionalities:

1. zeros()
This function is newly added in Matrix2, which makes the given matrix a zero matrix.
That is, all elements of the given matrix should be zeros.

2. eyes()
Another new function eyes() makes the given matrix the identity matrix.

3. show()
The show() function of the class Matrix is redefined in class Matrix2.
Now, this function only displays the biggest element of the matrix.

4. You are free to modify the previous class Matrix if necessary, but you need to use inheritance to create class Matrix2.
Make sure all functions (including those of HW2) are working as intended.
Also, any errors in the previous HW2 code should be corrected. Only then can you get a full score.


Submit this file to the LMS.
*/


//Your code goes here.

class Matrix {
protected:
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

class Matrix2 :public Matrix {
	int save = 0;
public:
	void zeros(void);
	void eyes(void);
	void show(void);
};

void Matrix2::zeros(void) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			arr[i][j] = 0;
		}
	}
}

void Matrix2::eyes(void) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (i == j) { arr[i][j] = 1; }
			else { arr[i][j] = 0; }
		}
	}
}

void Matrix2::show(void) {
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (arr[i][j] > save) {
				save = arr[i][j];
			}
		}
	}
	cout << "The biggest element is : " << save << endl << endl;
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


	//added for HW3
	Matrix2 F;
	cout << "Zero Matrix" << endl;
	F.zeros();			// zero all elements.
	F.Matrix::show();	// call the show() of the base.
	F.show();			// diplay the biggest element.

	cout << "Identity Matrix" << endl;
	F.eyes();			// make F the identity matrix.
	F.Matrix::show();
	F.show();

	return 0;
}