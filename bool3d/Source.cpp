#include <iostream>
#include <vector>
#include <cmath>

namespace CG {
	struct Vec3 {
		float e[3];
		Vec3() {}
		Vec3(const float x[]);
		float len();
	};

	Vec3::Vec3(const float x[]) {
		for (int i = 0; i < 3; i++) {
			e[i] = x[i];
		}
	}

	float Vec3::len() {
		float s = 0;
		for (int i = 0; i < 3; i++) {
			s += e[i] * e[i];
		}
		return std::sqrt(s);
	}

	Vec3 operator-(Vec3 a, Vec3 b) {
		Vec3 v;
		for (int i = 0; i < 3; i++) {
			v.e[i] = a.e[i] - b.e[i];
		}
		return v;
	}

	Vec3 operator/(Vec3 a, float x) {
		Vec3 v;
		for (int i = 0; i < 3; i++) {
			v.e[i] = a.e[i] / x;
		}
		return v;
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
		Vec3 s, t; 
		AABB aabb;

		void calcST();
		void calcAABB();
		Triangle(const float x[]) :p{ Vec3(x), Vec3(x + 3), Vec3(x + 6) } {}
		Triangle(const std::initializer_list<float> &x) :Triangle(x.begin()) {}
	};

	std::ostream& operator<<(std::ostream& os, const Triangle& x) {
		os << "Triangle( " << "s: " << x.s << ", " << "t: " << x.t << " )";
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

	void Triangle::calcST() {
		Vec3 s_un = p[1] - p[0], t_un = p[2] - p[0];
		s = s_un / s_un.len(); 
		t = t_un / t_un.len();
	}

	struct Mesh {
		std::vector<Triangle> t;
	};
}

int main() {
	CG::Triangle t({ -2, 4, 3, 1, -5, 9, 2, 3, 8 });
	t.calcST();
	t.calcAABB();
	std::cout << t;
}