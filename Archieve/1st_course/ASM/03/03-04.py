get_radix_len = lambda n, k: 1 + (get_radix_len(n / k, k) if n >= k else 0)
open("03-04.prec", "w").write(str([get_radix_len(n, k) for n in range(0, 2011) for k in range(2, 11)]))
