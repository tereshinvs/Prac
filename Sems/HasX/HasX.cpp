template<typename T>
class HasX {
	private:
		struct int2 {
			int _int[2];
		};

		template<typename U>
		int test(...);

		template<typename U>
		int2 test(U::x *);
	public:
		enum {
			value = sizeof(test< B<T> >(0)) == sizeof(int);
		};
};