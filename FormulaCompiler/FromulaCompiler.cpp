#include <iostream>
#include <iomanip>
#include <string>
#include "SyntaxTree.h"

using namespace std;

int main()
	{
	string formula;
	while (getline(cin, formula))
		{
		SyntaxTree F;
		F.BuildSyntaxTree(formula);
		cout << fixed << setprecision(5);
		cout << F.Count() << endl;
		}
	system("pause");
	return 0;
	}