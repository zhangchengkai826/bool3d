#include <iostream>
#include <vector>
#include <cmath>
#include <exception>
#include <random>

namespace CG {
	struct Vec3 {
		float e[3];
		Vec3() {}
		Vec3(const float x[]);
		Vec3(const std::initializer_list<float> &x) :Vec3(x.begin()) {}
		float len();
		void norm();
		Vec3 operator/=(float x);
		Vec3 operator-();
	};

	float dot(Vec3 a, Vec3 b);
	Vec3 operator*(Vec3 a, float x);
	Vec3 operator/(Vec3 a, float x);

	Vec3::Vec3(const float x[]) {
		for (int i = 0; i < 3; i++) {
			e[i] = x[i];
		}
	}

	Vec3 Vec3::operator-() {
		return *this * (-1);
	}

	float Vec3::len() {
		float s = dot(*this, *this);
		return std::sqrt(s);
	}

	void Vec3::norm() {
		*this /= len();
	}

	Vec3 Vec3::operator/=(float x) {
		*this = *this / x;
		return *this;
	}

	Vec3 operator-(Vec3 a, Vec3 b) {
		Vec3 v;
		for (int i = 0; i < 3; i++) {
			v.e[i] = a.e[i] - b.e[i];
		}
		return v;
	}

	Vec3 norm(Vec3 x) {
		Vec3 r(x);
		r.norm();
		return r;
	}

	float dot(Vec3 a, Vec3 b) {
		float s = 0;
		for(int i = 0; i < 3; i++) {
			s += a.e[i] * b.e[i];
		}
		return s;
	}

	Vec3 operator*(Vec3 a, float x) {
		Vec3 v;
		for (int i = 0; i < 3; i++) {
			v.e[i] = a.e[i] * x;
		}
		return v;
	}

	Vec3 operator/(Vec3 a, float x) {
		return a * (1 / x);
	}


	std::ostream& operator<<(std::ostream& os, const Vec3& x)
	{
		os << "Vec3( " << x.e[0] << ", " << x.e[1] << ", " << x.e[2] << " )";
		return os;
	}

	struct AABB {
		Vec3 a;
		Vec3 b;
	};

	std::ostream& operator<<(std::ostream& os, const AABB& x)
	{
		os << "AABB( " << x.a << ", " << x.b << " )";
		return os;
	}

	struct Triangle {
		Vec3 p[3];
		float l[3]; /* edge length */
		Vec3 e[3]; /* edge normalized vector (0->1, 1->2, 2->0) */
		Vec3 n;
		AABB aabb;

		void calcLEN();
		void calcAABB();
		Triangle(const float x[]) :p{ Vec3(x), Vec3(x + 3), Vec3(x + 6) } {}
		Triangle(const std::initializer_list<float> &x) :Triangle(x.begin()) {}
	};

	std::ostream& operator<<(std::ostream& os, const Triangle& x) {
		os << "Triangle( " << "n: " << x.n << " )";
		return os;
	}

	void Triangle::calcAABB() {
		Vec3 &a = aabb.a, &b = aabb.b;
		a = b = p[0];
		for (int pid = 1; pid < 3; pid++) {
			for (int eid = 0; eid < 3; eid++) {
				float e = p[pid].e[eid];
				if (e < a.e[eid]) {
					a.e[eid] = e;
				}
				if (e > b.e[eid]) {
					b.e[eid] = e;
				}
			}
		}
	}

	Vec3 cross(Vec3 a, Vec3 b) {
		Vec3 r;
		for (int i = 0; i < 3; i++) {
			int p1 = (i + 1) % 3, p2 = (i + 2) % 3;
			r.e[i] = a.e[p1] * b.e[p2] - a.e[p2] * b.e[p1];
		}
		return r;
	}

	void Triangle::calcLEN() {
		for (int i = 0; i < 3; i++) {
			Vec3 un = p[(i + 1) % 3] - p[i]; 
			l[i] = un.len();
			e[i] = un / l[i];
		}
		n = norm(cross(e[0], -e[2]));
	}

	struct Mesh {
		std::vector<Triangle> t;

		Mesh(int pCnt);
	};

	float dist(Vec3 p, Triangle t) {
		Vec3 dp[3], dp_proj[3];
		float length[3];
		float h = dot(p - t.p[0], t.n);
		Vec3 hv = t.n * h;

		for (int i = 0; i < 3; i++) {
			dp[i] = p - t.p[i];
			dp_proj[i] = dp[i] - hv;
			length[i] = dot(dp_proj[i], t.e[i]);
		}

		for (int i = 0; i < 3; i++) {
			Vec3 dv = cross(dp_proj[i], t.e[i]);
			float val = dot(t.n, dv);
			if (length[i] >= 0 && length[i] <= t.l[i] && val > 0) {
				return dv.len();
			}
		}

		for (int i = 0; i < 3; i++) {
			int m = (i + 2) % 3;
			if (length[i] < 0 && length[m] > t.l[m]) {
				return dp[i].len();
			}
		}

		return std::abs(h);
	}

	Mesh::Mesh(int pCnt) {
		if (pCnt < 4) {
			std::runtime_error("only >= 4 points can form a water tight volume!");
		}
		
		std::random_device r;
		std::mt19937 mt(r());
		std::uniform_real_distribution<float> ud(-1, 1);

		for (int i = 0; i < 10; i++) {
			std::cout << ud(mt) << std::endl;
		}
	}
}

int main() {
	CG::Triangle t({ -2, 4, 3, 1, -5, 9, 2, 3, 8 });
	t.calcLEN();
	t.calcAABB();
	std::cout << t << std::endl;
	std::cout << dist(CG::Vec3({ -2, 7, 1 }), t);
	CG::Mesh m(4);
}