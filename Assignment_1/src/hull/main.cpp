////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;

double inline det(const Point &u, const Point &v) {
	return imag(conj(u) * v);
}

struct Compare {
	Point p0; // Leftmost point of the poly
	bool operator ()(const Point &p1, const Point &p2) {
		if (p2 == p0) return false;
		if (p1 == p0) return true;
		double d = det(p1 - p0, p2 - p0);
		return (d < 0 || (d == 0 && abs(p1 - p0) < abs(p2 - p0)));
	}
};

bool inline salientAngle(Point &a, Point &b, Point &c) {
	return (det(b - a, c - a) >= 0);
}

////////////////////////////////////////////////////////////////////////////////

Polygon convex_hull(std::vector<Point> &points) {
	Compare order;
	order.p0 = points[0];
	for (size_t i = 0; i < points.size(); ++i) {
		if (points[i].real() < order.p0.real()) {
			order.p0 = points[i];
		} else if (points[i].real() == order.p0.real() && points[i].imag() < order.p0.imag()) {
			order.p0 = points[i];
		}
	}
	std::sort(points.begin(), points.end(), order);
	Polygon hull;
	for (size_t i = 0; i < points.size(); ++i) {
		hull.push_back(points[i]);
		while (hull.size() > 3u && salientAngle(hull.end()[-3], hull.end()[-2], hull.end()[-1])) {
			hull.end()[-2] = hull.back();
			hull.pop_back(); // Pop inner vertices
		}
	}
	return hull;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> load_xyz(const std::string &filename) {
	std::vector<Point> points;
	std::ifstream in(filename);
	int n; in >> n;
	for (int i = 0; i < n; ++i) {
		double x, y, z;
		in >> x >> y  >> z;
		points.push_back(Point(x, y));
	}
	return points;
}

void save_off(const std::string &filename, Polygon &poly) {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << std::fixed;
	out << "OFF\n" << poly.size() << " " << poly.size() << " 0\n";
	for (const auto &v : poly) {
		out << v.real() << ' ' << v.imag() << " 0\n";
	}
	for (size_t i = 0; i < poly.size(); ++i) {
		out << "2 " << i << ' ' << (i+1)%poly.size() << "\n";
	}
	out << std::endl;
}

void save_obj(const std::string &filename, Polygon &poly) {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << std::fixed;
	for (const auto &v : poly) {
		out << "v " << v.real() << ' ' << v.imag() << " 0\n";
	}
	for (size_t i = 0; i < poly.size(); ++i) {
		out << "l " << i+1 << ' ' << 1+(i+1)%poly.size() << "\n";
	}
	out << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 2) {
		std::cerr << "Usage: " << argv[0] << " points.xyz output.obj" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon hull = convex_hull(points);
	save_obj(argv[2], hull);
	return 0;
}
