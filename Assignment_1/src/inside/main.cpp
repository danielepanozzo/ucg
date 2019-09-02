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

// Return true iff [a,b] intersects [c,d], and store the intersection in ans
bool intersect_segment(const Point &a, const Point &b, const Point &c, const Point &d, Point &ans) {
	const double eps = 1e-10; // small epsilon for numerical precision
	double x = det(c - a, d - c);
	double y = det(b - a, a - c);
	double z = det(b - a, d - c);
	if (std::abs(z) < eps || x*z < 0 || x*z > z*z || y*z < 0 || y*z > z*z) return false;
	ans = c + (d - c) * y / z;
	return true;
}

////////////////////////////////////////////////////////////////////////////////

Point outside(1000, 1000); // A point outside the workspace, must have "random" coordinates
bool is_inside(const Polygon &poly, const Point &query) {
	int n = poly.size();
	bool tmp, ans = false;
	for (size_t i = 0; i < poly.size(); ++i) {
		Point m; // Coordinates of intersection point
		tmp = intersect_segment(query, outside, poly[i], poly[(i + 1) % n], m);
		ans = (ans != tmp);
	}
	return ans;
}

////////////////////////////////////////////////////////////////////////////////

Polygon load_obj(const std::string &filename) {
	std::ifstream in(filename);
	std::vector<Point> points;
	Polygon poly;
	char key;
	while (in >> key) {
		if (key == 'v') {
			double x, y, z;
			in >> x >> y >> z;
			points.push_back(Point(x, y));
		} else if (key == 'f') {
			std::string line;
			std::getline(in, line);
			std::istringstream ss(line);
			int id;
			while (ss >> id) {
				poly.push_back(points[id-1]);
			}
		}
	}
	return poly;
}

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

void save_xyz(const std::string &filename, std::vector<Point> &points) {
	std::ofstream out(filename);
	if (!out.is_open()) {
		throw std::runtime_error("failed to open file " + filename);
	}
	out << std::fixed;
	out << points.size() << '\n';
	for (const auto &p : points) {
		out << p.real() << ' ' << p.imag() << " 0\n";
	}
	out << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {
	if (argc <= 3) {
		std::cerr << "Usage: " << argv[0] << " points.xyz poly.obj result.xyz" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon poly = load_obj(argv[2]);
	std::vector<Point> result;
	for (size_t i = 0; i < points.size(); ++i) {
		if (is_inside(poly, points[i])) {
			result.push_back(points[i]);
		}
	}
	save_xyz(argv[3], result);
	return 0;
}
