import random
from pygost.gost34112012 import GOST34112012

gost_params = [
    0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFDC4,
    0xE8C2505DEDFC86DDC1BD0B2B6667F1DA34B82574761CB0E879BD081CFD0B6265EE3CB090F30D27614CB4574010DA90DD862EF9D4EBEE4761503190785A71C760,
    2 ** 512 - 569,
    3,
    0x7503CFE87A836AE3A61B8816E25450E6CE5E1C93ACF1ABC1778064FDCBEFA921DF1626BE4FD036E93D75E6A50E3A41E98028FE5FC235F5B889A589CB5215F2A4,
    0xffffffff00000000ffffffffffffffffbce6faada7179e84f3b9cac2fc632551
]


def gost3411(M):
    f = GOST34112012()
    f.update(str(M).encode('utf-8'))
    return int(f.hexdigest(), base=16)


def extended_euclid(a, b):
    if a == 0:
        return b, 0, 1
    else:
        gcd, x, y = extended_euclid(b % a, a)
        return gcd, y - (b // a) * x, x


def modular_multiplication(a, b, mod):
    return (a % mod * b % mod + mod) % mod


def modular_division(a, b, mod):
    g, x, y = extended_euclid(b, mod)
    if g == 1:
        x = (x % mod + mod) % mod
    return modular_multiplication(a, x, mod)


class EllipticCurvePoint:
    def __init__(self, curve, x=None, y=None, is_zero=False):
        self.x = x
        self.y = y
        self.a = curve.a
        self.b = curve.b
        self.p = curve.p
        self.curve = curve
        self.is_zero = is_zero

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y and self.curve == other.curve

    def is_inverted(self, other):
        x1, y1 = self.x, self.y
        x2, y2 = other.x, other.y
        return y1 % self.p == -y2 % self.p and x1 == x2

    def __add__(self, other):
        x1, y1 = self.x, self.y
        x2, y2 = other.x, other.y
        if self.is_inverted(other):
            return EllipticCurvePoint(self.curve, is_zero=True)
        if self.is_zero:
            return other
        if other.is_zero:
            return self
        dy, dx = (y2 - y1) % self.p, (x2 - x1) % self.p
        div = modular_division(dy, dx, self.p) if self != other else modular_division(3 * (x1 * x1) + self.a, 2 * y1,
                                                                                      self.p)
        x3 = ((div ** 2) - x1 - x2) % self.p
        y3 = (div * (x1 - x3) - y1) % self.p
        return EllipticCurvePoint(self.curve, x3, y3)

    def __sub__(self, other):
        new = EllipticCurvePoint(self.curve, other[0], -other[1])
        return self + new

    def double(self, P):
        return P + P

    def multiplication(self, P, n):
        if n == 1:
            return P
        if n % 2 == 0:
            return self.double(self.multiplication(P, n / 2))
        else:
            return self.multiplication(P, n - 1) + P

    def __mul__(self, n):
        return self.multiplication(self, n)

    def __rmul__(self, n):
        return self * n


class EllipticCurve:
    def __init__(self, a, b, p, x, y, q):
        assert (4 * a ** 3 + 27 * b ** 2) % p != 0
        self.a = a
        self.b = b
        self.p = p
        self.P = EllipticCurvePoint(self, x, y)
        self.q = q


class GOST3410(object):
    def __init__(self, curve):
        self.curve = curve
        self.d = random.getrandbits(256)
        self.Q = self.d * self.curve.P

    def validate(self, M, r, s):
        if r <= 0 or r >= self.curve.q:
            return False
        if s <= 0 or s >= self.curve.q:
            return False
        mhash = self._compute_hash(M)
        e = self._compute_e(mhash, self.curve.q)

        v = modular_division(1, e, self.curve.q)
        z1 = modular_multiplication(s, v, self.curve.q)
        z2 = modular_multiplication(-r, v, self.curve.q)
        C = z1 * self.curve.P + z2 * self.Q
        R = C.x % self.curve.q
        return R != r

    def generate(self, M):
        mhash = self._compute_hash(M)
        e = self._compute_e(mhash, self.curve.q)

        while True:
            k = self._generate_k(self.curve.q)
            C, r = self._compute_curve_c_r(k, self.curve.P, self.curve.q)
            if r == 0:
                continue

            s = self._compute_s(r, self.d, k, e, self.curve.q)
            if s == 0:
                continue
            return r, s

    @staticmethod
    def _compute_hash(M):
        return gost3411(M)

    @staticmethod
    def _compute_e(mhash, q):
        e = mhash % q
        if e == 0:
            e = 1

        return e

    @staticmethod
    def _generate_k(q):
        return random.randrange(0, q)

    @staticmethod
    def _compute_curve_c_r(k, P, q):
        C = k * P
        r = C.x % q
        return C, r

    @staticmethod
    def _compute_s(r, d, k, e, q):
        return (r * d + k * e) % q


if __name__ == '__main__':
    params = gost_params
    curve = EllipticCurve(
        params[0],
        params[1],
        params[2],
        params[3],
        params[4],
        params[5]
    )

    gost3410 = GOST3410(curve)

    with open("gost_data.txt", 'r') as file:
        text = file.read().split(',')
    for string in text:
        r, s = gost3410.generate(string)
        print(f"string: {string}")
        print(f"r: {r}\ns: {s}")
        print(f"Validation: {gost3410.validate(string, r, s)}")

    input("Press any key...")
