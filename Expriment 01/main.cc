#include <iostream>
#include "my_print.h"

using namespace std;

int main()
{
	char my_string[1000];
	cout << "Please input a string: ";
	cin >> my_string;
	my_print(my_string);
	my_print2(my_string);
}
