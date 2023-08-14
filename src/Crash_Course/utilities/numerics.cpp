
#include <boost/math/constants/constants.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <boost/numeric/conversion/converter.hpp>
#include <catch2/catch_all.hpp>
#include <cmath>
#include <complex>
#include <limits>
#include <random>

/* Complex Numbers */
using namespace Catch;

TEST_CASE("std::complex has a real and imaginary component") {
    std::complex<double> a{0.5, 14.13};
    REQUIRE(std::real(a) == Approx(0.5));
    REQUIRE(std::imag(a) == Approx(14.13));
}

/* Complex Numbers */

/* Mathematical Constants */

TEST_CASE("boost::math offers constants") {
    using namespace boost::math::double_constants;

    auto sphereVolume = four_thirds_pi * std::pow(10, 3);
    REQUIRE(sphereVolume == Approx(4188.7902047));
}

/* Mathematical Constants */

/* Random Numbers */

TEST_CASE("mt19937_64 is psuedorandom") {
    std::mt19937_64 mtEngine{91586};
    REQUIRE(mtEngine() == 8346843996631475880);
    REQUIRE(mtEngine() == 2237671392849523263);
    REQUIRE(mtEngine() == 7333164488732543658);
}

TEST_CASE("std::random_device is invocable") {
    std::random_device rdEngine{};
    REQUIRE_NOTHROW(rdEngine());
}

// Random Number Distributions
TEST_CASE("std::uniform_int_distribution produces uniform ints") {
    std::mt19937_64 mtEngine{102787};
    std::uniform_int_distribution<int> intD{0, 10};
    const size_t n{1'000'000};

    int sum{};
    for (size_t i{}; i < n; i++) sum += intD(mtEngine);
    const auto sampleMean = sum / double{n};
    REQUIRE(sampleMean == Approx(5).epsilon(.1));
}

/* Random Numbers */

/*Boost Numeric Conversion*/
using doubleToInt = boost::numeric::converter<int, double>;

TEST_CASE("boost::converter offers the static method convert") { REQUIRE(doubleToInt::convert(3.14159) == 3); }

TEST_CASE("boost::numeric::converter implements operator()") {
    doubleToInt dti;
    REQUIRE(dti(3.14159) == 3);
    REQUIRE(doubleToInt{}(3.14159) == 3);
}

TEST_CASE("boost::numeric::converter checks for overflow") {
    auto yuge = std::numeric_limits<double>::max();
    doubleToInt dti;
    REQUIRE_THROWS_AS(dti(yuge), boost::numeric::positive_overflow);
}

TEST_CASE("boost::boost::numeric_cast checks overflow") {
    auto yuge = std::numeric_limits<double>::max();
    REQUIRE_THROWS_AS(boost::numeric_cast<int>(yuge), boost::numeric::positive_overflow);
}
/*Boost Numeric Conversion*/
