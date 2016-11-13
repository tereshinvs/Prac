#include <iostream>

using namespace std;

class A {
	public:
		A() {}
		~A() {}
		
		static void G() {
			cout << "G" << endl;
		}
		
		void H() const {
			cout << "H" << endl;
		}
};

int main() {
	const A a;
	A::G();
	a.H();
}