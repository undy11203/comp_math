#include <float.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <math.h>

#define STEP_INCREASE 1

enum range {
    infinity_left,
    infinity_right,
    finity
};

enum sign {
    plus,
    minus
};

class solution {
private:
    int multiplicity;
    long double x_result, x2_result = LDBL_MAX, x3_result = LDBL_MAX;
    long double left_bound, right_bound;
    long double e, a, b, c;

    long double f(long double x) {
        return pow(x, 3) + a*pow(x, 2) + b*x + c;
    }
    long double f_shtrih(long double x) {
        return 3*pow(x,2) + 2*a*x + b;
    }
    std::pair<long double, long double> f_shtrix_extrem() {

    }
    sign find_sign(long double value) {
        return value < 0 ? minus : plus;
    }
    long double discriminant_f_shtrix() {
        return 4*pow(a,2) - 12*b;
    }

    long double localization(range cur_range, long double x) {
        std::cout << "branch(localization)" << std::endl;
        long double step = 0;
        while(sign(f(x)) == sign(f(x+step))) {
            if(cur_range == infinity_right) {
                x = step;
                step += STEP_INCREASE;
            } else if(cur_range == infinity_left) {
                x = step;
                step -= STEP_INCREASE;
            }
        }
        if(cur_range == infinity_right) {
            left_bound = x;
            right_bound = x+step;
        } else if(cur_range == infinity_left) {
            right_bound = x;
            left_bound = x+step;
        }

        return bisection(left_bound, right_bound);
    }
    long double bisection(long double left, long double right) {
        std::cout << "branch(bisection)" << std::endl;

        long double m = (left + right) / 2;
        if(abs(f(m)) <= e) {
            return m;
        } else if(f(m) > e) {
            return bisection(left, m);
        } else if(f(m) < -e) {
            return bisection(m, right);
        }
    }
public:
    void set_params(long double e, long double a, long double b, long double c) {
        this->e = e; this->a = a; this->b = b; this->c = c;
    }

    void done(){
        std::cout << "main branch(discriminant)" << std::endl;
        if(discriminant_f_shtrix() < -e) {
            if(abs(f(0)) <= e) {
                x_result = 0; multiplicity = 1;
            }else if(f(0) > e) {
                x_result = localization(infinity_left, 0);
            }else if(f(0) < -e) {
                x_result = localization(infinity_right, 0);
            }
        }else if(discriminant_f_shtrix() > e) {
            if(abs(f(-a/3)) <= e) {
                x_result = -a/3; multiplicity = 1;
            }else if(f(-a/3) > e) {
                x_result = localization(infinity_left, -a/3);
            }else if(f(-a/3) < -e) {
                x_result = localization(infinity_right, -a/3);
            }
        }else if(abs(discriminant_f_shtrix()) <= e) {
            long double alpha = (-a-sqrt(discriminant_f_shtrix()))/3, beta = (-a+sqrt(discriminant_f_shtrix()))/3;
            if(abs(f(alpha)) < e && abs(f(beta)) < e) {
                x_result = (alpha+beta)/2;
                multiplicity = 3;
            }else if(f(alpha) > e && f(beta) < -e) {
                x_result = localization(infinity_left, alpha);
                x2_result = bisection(alpha, beta);
                x3_result = localization(infinity_right, beta);
                multiplicity = 1;
            } else if(abs(f(beta)) < e) { //?
                x_result = localization(infinity_left, alpha);
                multiplicity = 2;
            } else if(abs(f(alpha)) < e) { //?
                x_result = localization(infinity_right, beta);
                multiplicity = 2;
            } else if(f(alpha) < -e) {
                x_result = localization(infinity_right, beta);
                multiplicity = 1;
            } else if(f(beta) > e) {
                x_result = localization(infinity_left, alpha);
                multiplicity = 1;
            }
        }
    }

    std::string get_result() {
        if(x2_result == LDBL_MAX && x3_result == LDBL_MAX) {
            return  "kratnostb and x_result and f_result" +
            std::to_string(multiplicity) + " " +
                std::to_string(x_result) + " " +
                    std::to_string(f(x_result));
        } else if(x2_result != LDBL_MAX && x3_result == LDBL_MAX) {
            return  "kratnostb and x_result and f_result" +
            std::to_string(multiplicity) + " " +
                std::to_string(x_result) + " " +
                    std::to_string(f(x_result)) + "\n" +
                        std::to_string(x2_result) + " " +
                            std::to_string(f(x2_result));
        } else if(x2_result != LDBL_MAX && x3_result != LDBL_MAX) {
            return  "kratnostb and x_result and f_result" +
            std::to_string(multiplicity) + " " +
                std::to_string(x_result) + " " +
                    std::to_string(f(x_result)) + "\n" +
                        std::to_string(x2_result) + " " +
                            std::to_string(f(x2_result)) + "\n" +
                                std::to_string(x3_result) + " " +
                                    std::to_string(f(x3_result));
        }
    }

};

std::ostream& operator<< (std::ostream &os, solution &sol) {
    return os << sol.get_result();
}

std::istream& operator>> (std::istream &in, solution &sol) {
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
    std::cout << sol;
    return 0;
}
