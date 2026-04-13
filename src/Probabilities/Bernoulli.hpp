#pragma once
#include <random>

class Bernoulli {
  private:
    double                                 probability_p;
    std::mt19937                           generator;
    std::bernoulli_distribution            std_bernoulli;
    std::uniform_real_distribution<double> uniform_dist;

  public:
    // Constructor
    Bernoulli(double p);

    // Methods
    bool generate_std();
    bool generate_scratch();
    void compare(int n);
};