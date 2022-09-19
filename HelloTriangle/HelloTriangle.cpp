#include "TriangleApp.h"
using namespace std;


int main(int argc, char** argv)
{
	auto appGui = make_shared<TriangleGui>(TriangleGui());
	TriangleApp a(appGui);
	a.run();

	return 0;
}

