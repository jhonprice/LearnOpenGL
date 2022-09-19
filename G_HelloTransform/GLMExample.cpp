#include "TransformApp.h"
using namespace std;


int main(int argc, char** argv)
{
	auto appGui = make_shared<TransformGui>(TransformGui());
	TransformApp a(appGui);
	a.run();

	return 0;
}

