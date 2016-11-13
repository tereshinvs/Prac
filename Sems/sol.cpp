#include <iostream>

using namespace std;

class A {
	public:
		virtual void f() {
			cout << "1 ";
		}

		void g() {
			cout << "2 ";
			A::h();
		}

		void h() {
			cout << "3 ";
			f();
		}
};

class B: public A {
	public:
		virtual void f() {
			cout << "4 ";
		}

		virtual void g() {
			cout << "5 ";
			B::f();
		}

		void h() {
			cout << "6 ";
		}
};

class C: public B {
	public:
		void f() {
			cout << "7 ";
			B::g();
		}

		void g() {
			cout << "8 ";
			A::g();
		}

		virtual void h() {
			cout << "9 ";
			C::g();
		}
};

int main() {
	C c;
	c.h();
}