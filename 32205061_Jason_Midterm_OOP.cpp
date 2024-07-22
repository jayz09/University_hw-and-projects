#include <iostream>
#include <cstdlib>
#include <ctime>
#define PI 3.14
#define EXIT -1
using namespace std;


/*					OOP MID-TERM EXAM (30pts, 21.10.21.)
Your goal is
1) to supplement the class Shape, and
2) to create Circle and Rectangle classes by inheriting from the Shape class,
to make the given main function work properly.

Read the main function carefully, and find out what you need to implement.
Submit this file to the LMS.

Notes.
1. You are not allowed to modify the main function.
2. The class Shape is currently incomplete; something must be added or modified.
   You are FREE to modify the class Shape.


Several requirements are listed below:
1. Circle and Rectangle classes
	1-1. Both Circle and Rectangle are characterized by one integer variable _d which has a different meaning depending on the shape of the object as follows:
		 For Circle, _d represents the radius of the circle.
		 For Rectangle, _d represents the length of one side. Assume that all sides of a rectangle object have the same length, that is, a square.
	1-2. Both Circle and Rectangle require a parameterized constructor, which initializes the value of _d.

2. displayTypeAndArea()
	2-1. diplayTypeAndArea() displays the type (either circle or rectangle) and the area (as a float value) of the corresponding object.
	2-2. There is no fixed output format.
	2-3. When displayTypeAndArea() is called, it should display the values of the corresponding object, not that of the base (i.e., Shape).
		 For example, if displayTypeAndArea() is called by a circle, then its ouput should be like "Type: Circle, Area: 4".

3. getCount()
	This is a static function that displays the number of objects currently created.

4. Operator > Overloading
	Implement operator > overloading to make it possible to compare the areas of two objects. The return value is determined as follows:
	For Object1 > Object2,
	If Object1 and Object2 are not of the same shape,						it returns -2.
	If Object1 and Object2 are of the same shape, and
			 if the area of Object1 is greater than that of Object2,		it returns 1.
		else if the area of Object1 is less than that of Object2,			it returns -1.
		else if the area of Object1 is equal to that of Object2,			it returns 0.
*/


// Complete the Shape class. You are FREE to modify this.
class Shape {
protected:
	int _d;
	float AOC; // Area of circle
	float AOR; // Area of rectangle
	string shape;
public:
	static int cnt;
	virtual void displayTypeAndArea() { cout << "You call the wrong function. Something in your code is wrong." << endl; }
	static int getCnt(void) {
		return cnt;
	}
	friend int operator>(Shape x, Shape y) {
		if (x.shape != y.shape) {
			return -2;
		}
		else if (x.shape == y.shape) {
			if (x.shape == "Circle") {
				if (x.AOC > y.AOC) {
					return 1;
				}
				else if (x.AOC < y.AOC) {
					return -1;
				}
				else if (x.AOC == y.AOC) {
					return 0;
				}
			}
			else if (x.shape == "Rectangle") {
				if (x.AOR > y.AOR){
					return 1;
				}
				else if (x.AOR < y.AOR) {
					return -1;
				}
				else if (x.AOR == y.AOR) {
					return 0;
				}
			}
		}
		return 0;
	}
};

int Shape::cnt = 0;

class Circle : public Shape {
public:
	Circle(int x) {
		_d = x;
		cnt++;
	}
	void displayTypeAndArea() {
		AOC = float(PI * (_d * _d));
		shape = "Circle";
		cout << "Type: " << shape << ", Area : " << AOC << endl;
	}
};

class Rectangle : public Shape {
public:
	Rectangle(int x) {
		_d = x;
		cnt++;
	}
	void displayTypeAndArea() {
		AOR = float(_d * _d);
		shape = "Rectangle";
		cout << "Type: " << shape << ", Area : " << AOR << endl;
	}
};

// DO NOT TOUCH THE MAIN FUNCTION.
int main() {
	srand(time(NULL));
	cout << "Press 'c' for circle, 'r' for rectangle, and 'q' to quit." << endl;
	cout << "You can create up to 5 objects." << endl << endl;
	Shape* arr[5] = {};									// maximum 5 objects can be created.

	char s;
	int i = -1;
	while (++i < 5) {
		cin >> s;
		switch (s) {
		case 'c':
			arr[i] = new Circle(rand() % 10 + 1);		// A circle is created.
			break;
		case 'r':
			arr[i] = new Rectangle(rand() % 10 + 1);	// A rectangle is created
			break;
		case 'q':
		default:
			i = EXIT;									// Otherwise, the loop is terminated.
			break;
		}
		if (i == EXIT) break;
		if (arr[i] != NULL)
			arr[i]->displayTypeAndArea();				// The type and the area of arr[i] are displayed, e.g., "Type: Rectangle, Area: 4"
	}



	cout << endl << "Total number of objs created: " << Shape::getCnt() << endl;				// Total # of objects created is displayed.

	if (arr[1] != NULL) {
		cout << "arr[0] > arr[1]?: " << (*(arr[0]) > *(arr[1])) << endl;						// Compares the areas of two given objects.
		cout << "arr[1] > arr[0]?: " << (*(arr[1]) > *(arr[0])) << endl;
	}
	if (arr[2] != NULL)
		cout << "arr[0] > arr[2]?: " << (*(arr[0]) > *(arr[2])) << endl;

	return 0;
}