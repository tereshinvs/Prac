from math import *

print([factorial(n) // (factorial(k) * factorial(n - k)) if n >= k else 0 for n in range(0, 32) for k in range(0, 32)])

