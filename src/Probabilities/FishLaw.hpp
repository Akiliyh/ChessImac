#include <random>

class FishLaw {
  private:
    double                                 m_lambda;
    std::mt19937                           m_generator;
    std::uniform_real_distribution<double> m_dist;

  public:
    FishLaw(double lambda, unsigned int seed = std::random_device{}());

    int generate();

    void   set_lambda(double lambda);
    double get_lambda() const;
};
