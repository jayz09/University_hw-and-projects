#include <iostream>
using namespace std;

class vector {
	float arr[10];
public:
	void task1(void); // (a) Create a vector
	void task2(void); // (b) Modify the value of a given element
	void task3(void); // (c) Multiply by a scalar value
	void task4(void); // (d) Display the vector in the form (10, 20, 30,...)
};

int main() {

	vector a;
	a.task1();
	a.task2();
	a.task3();
	a.task4();

	return 0;
}

// (a) Create a vector
void vector::task1(void) {

	float stvl, diff;
	// stvl --> Starting value
	// diff --> Difference between value
	cout << "Enter the starting value : " << endl;
	cin >> stvl;
	cout << "Enter the difference between value : " << endl;
	cin >> diff;
	cout << "----------------------------------------" << endl;

	for (int i = 0; i < 10; i++) {
		arr[i] = stvl;
		stvl += diff;
		cout << arr[i] << " ";
	}
}

// (b) Modify the value of a given element
void vector::task2(void) {

	int a;
	float b;
	cout << "\n\nWhich element you want to modify from 0-9 : " << endl;
	cin >> a;
	cout << "Input the value you want to insert : " << endl;
	cin >> b;
	cout << "----------------------------------------" << endl;

	arr[a] = b;
	for (int i = 0; i < 10; i++) {
		cout << arr[i] << " ";
	}
}

// (c) Multiply by a scalar value
void vector::task3(void) {

	float a;
	cout << "\n\nInput a value to multiply : " << endl;
	cin >> a;
	cout << "----------------------------------------" << endl;

	for (int i = 0; i < 10; i++) {
		arr[i] *= a;
		cout << arr[i] << " ";
	}
}

// (d) Display the vector in the form (10, 20, 30,...)
void vector::task4(void) {

	cout << "\n\nThe vector below will be displayed in increment of 10" << endl;
	cout << "----------------------------------------" << endl;

	for (int i = 0; i < 10; i++) {
		arr[i] = (i + 1) * 10;
		cout << arr[i] << " ";
	}
	cout << endl;
}