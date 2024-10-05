#include <cmath>
#include <float.h>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <utility>
#include <vector>

#define STEP_INCREASE 1e-6

enum range { infinity_left, infinity_right };

enum sign { plus, minus };

class solution {
private:
  long double epsilon, a, b, c;

  long double f(long double x) { return pow(x, 3) + a * pow(x, 2) + b * x + c; }

  std::pair<long double, long double> find_extrem() {
    long double discriminant = pow(a, 2) - 3 * b;
    long double y0 = -discriminant / 3;

    if (y0 >= epsilon || discriminant < -epsilon) {
      return {LDBL_MAX, LDBL_MAX};
    }

    if (abs(discriminant) < epsilon) {
      discriminant = 0;
    }

    long double extreme1 = (-a - std::sqrt(discriminant)) / 3;
    long double extreme2 = (-a + std::sqrt(discriminant)) / 3;

    return {extreme1, extreme2};
  }

  long double find_border(long double border1, bool toRight) {

    long double value1 = f(border1);
    long double step = toRight ? STEP_INCREASE : -STEP_INCREASE;
    long double border2 = border1 + step;

    while (value1 * f(border2) > 0) {
      border2 += step;
    }

    return border2;
  }

  long double bisection(long double border1, long double border2) {
    long double f1 = f(border1);
    long double f2 = f(border2);

    if (fabs(f1) < epsilon)
      return border1;
    if (fabs(f2) < epsilon)
      return border2;

    while (true) {
      long double c = (border1 + border2) / 2;
      long double f_middle = f(c);

      if (fabs(f_middle) < epsilon)
        return c;
      if (f1 * f_middle < 0) {
        border2 = c;
        f2 = f_middle;
      } else {
        border1 = c;
        f1 = f_middle;
      }
    }
  }

public:
  void set_params(long double e, long double a, long double b, long double c) {
    this->epsilon = e;
    this->a = a;
    this->b = b;
    this->c = c;
  }

  void done() {

    auto extremes = find_extrem();
    long double alpha = extremes.first, beta = extremes.second;

    std::vector<std::pair<long double, int>> roots;
    if (alpha == LDBL_MAX) {
      long double border_0 = 0;
      long double f_0 = f(border_0);
      if (fabs(f_0) < epsilon) {
        // std::cout << "11" << std::endl;
        roots.push_back({border_0, 3});
      } else if (f_0 < -epsilon) {
        // std::cout << "22" << std::endl;

        long double border_1 = find_border(0, true);
        roots.push_back({bisection(border_0, border_1), 1});
      } else {
        // std::cout << "33" << std::endl;
        long double border_1 = find_border(0, false);
        roots.push_back({bisection(border_0, border_1), 1});
      }
    } else {
      long double f1 = f(alpha);
      long double f2 = f(beta);

      if (f1 >= epsilon && f2 >= epsilon) {
        // std::cout << "1";
        long double border_0 = find_border(alpha, false);
        roots.push_back({bisection(border_0, alpha), 1});
      } else if (f1 <= -epsilon && f2 <= -epsilon) {
        // std::cout << "2";
        long double border_1 = find_border(beta, true);
        roots.push_back({bisection(alpha, border_1), 1});
      } else if (fabs(f1) < epsilon && fabs(f2) < epsilon) {
        // std::cout << "3";
        roots.push_back({alpha, 3});
      } else if (fabs(f2) <= epsilon) {
        // std::cout << "4";
        long double border_0 = find_border(alpha, false);
        roots.push_back({bisection(border_0, alpha), 1});
        roots.push_back({beta, 2});
      } else if (fabs(f1) <= epsilon) {
        // std::cout << "5";
        long double border_1 = find_border(beta, true);
        roots.push_back({alpha, 2});
        roots.push_back({bisection(beta, border_1), 1});
      } else if (f1 >= epsilon && f2 <= -epsilon) {
        // std::cout << "6";
        long double border_0 = find_border(alpha, false);
        long double border_1 = find_border(beta, true);
        roots.push_back({bisection(border_0, alpha), 1});
        roots.push_back({bisection(alpha, beta), 1});
        roots.push_back({bisection(beta, border_1), 1});
      }
    }

    // if (!roots.empty()) {
    //   for (const auto &root : roots) {
    //     std::cout << "Root: " << root.first << std::endl;
    //   }
    // } else {
    //   std::cout << "No roots found." << std::endl;
    // }
    if (!roots.empty()) {
      std::cout << "-----------------------------------" << std::endl;
      for (const auto &root : roots) {
        std::cout << "Root: " << root.first << ", Multiplicity: " << root.second
                  << std::endl;
      }
      std::cout << "-----------------------------------" << std::endl;
    } else {
      std::cout << "No roots found." << std::endl;
    }
  }
};

std::ostream &operator<<(std::ostream &os, solution &sol) {
  return os << "123";
}

std::istream &operator>>(std::istream &in, solution &sol) {
  long double e, a, b, c;
  in >> e;
  in >> a;
  in >> b;
  in >> c;

  sol.set_params(e, a, b, c);

  return in;
}

int main() {
  solution sol;
  std::cout << "Input \"e a b c\"" << std::endl;
  std::cin >> sol;
  sol.done();
  // std::cout << sol;
  return 0;
}
