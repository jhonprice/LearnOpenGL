#include "GlfwGuiApp.h"
using namespace std;
class Base {
public:
	virtual void f() = 0;
};
class BaseA : public Base {
public:
	virtual void f() override { std::cout << "321" << std::endl; };
};
class C {
public:
	std::shared_ptr<Base> p;
	void ff() {
		p->f();
	}
};

int main(int argc, char** argv)
{
	auto mgui = make_shared<ExampleGui>(ExampleGui());
	ExampleApp a(mgui);
	a.run();

	return 0;
}

