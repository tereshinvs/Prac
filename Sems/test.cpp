template<class T>
class Rational {
	private:
		T n;
		T d;

	public:
//		friend Rational<T> operator/(const Rational<T> &, const Rational<T> &);
		Rational<T> operator/(const Rational<T> &arg);
};
/*
template<class T>
Rational<T> operator/(const Rational<T> &arg1, const Rational<T> &arg2) {
	return Rational<T>(arg1.n * arg2.d, arg1.d * arg2.n);
}
*/

template<class T>
Rational<T> Rational<T>::operator/(const Rational<T> &arg) {
	return Rational<T>(n * arg.d, d * arg.n);
}
int main() {

}