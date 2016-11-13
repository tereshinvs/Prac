class A {
	int m_Foo, m_Bar;

	A(int in_Foo = 0,
		int in_Bar = 0) {
		m_Foo = in_Foo;
		m_Bar = in_Bar;
	}

public:
	void print() {
		cout << m_Foo << endl;
		cout << m_Bar << endl;
	}
};

int main() {
	A x();
	A y(1);
	A z(2, 3);
	x.print();
	y.print();
	z.print();
}