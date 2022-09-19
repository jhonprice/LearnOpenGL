#include "RectangleApp.h"
using namespace std;


int main(int argc, char** argv)
{
	auto appGui = make_shared<RectangleGui>(RectangleGui());
	RectangleApp a(appGui);
	a.run();

	return 0;
}

