#include "BaseShaderApp.h"
using namespace std;


int main(int argc, char** argv)
{
	auto appGui = make_shared<BaseShaderGui>(BaseShaderGui());
	BaseShaderApp a(appGui);
	a.run();

	return 0;
}

