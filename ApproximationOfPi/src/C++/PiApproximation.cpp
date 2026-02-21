#include <iostream>
#include <cstdlib>
#include <random>
#include <cmath>

template <typename T>
T sq(T value) {
    return std::pow(value, 2.0);
};

int main() {
   int numSamples; std::cin >> numSamples;

   std::random_device rd;
   std::mt19937 gen(rd());
    
   int insideCircle = 0;

    for (int i = 0; i <= numSamples; i++) {

        std::uniform_real_distribution<double> dist(-1.0, 1.0);

        double x = dist(gen);
        double y = dist(gen);

        if (sq(x) + sq(y) <= 1) {
            insideCircle += 1;
        };

    }

    double ratio = insideCircle / numSamples;
    double estimation = ratio * 4;

    std::cout << estimation;
}

