#include <random>

class Uniform {
  private:
    static std::mt19937& m_generator();

  public:
    static int    generate_int(int min, int max);
    static double generate_real(double min, double max);
};