#include <memory>
#include <numbers>
#include <string>
#include <type_traits>

using std::numbers::pi, std::string, std::unique_ptr;

const static string blue{"C0"};
const static string black{"k"};
const static string red = "C3";
const static string green = "C2";
const static string purple = "C4";
const static string orange = "C2";
const static string gray = "gray";

template <typename Base, typename T>
inline bool instanceof (const T* ptr) {
    auto res = dynamic_cast<const Base*>(ptr) != nullptr;
    return res;
}

template <typename Base, typename T>
inline bool instanceof (const unique_ptr<T>& ptr) {
    auto res = dynamic_cast<const Base*>(ptr.get()) != nullptr;
    return res;
}

template <typename T>
concept Numeric = requires { std::is_integral_v<T> || std::is_floating_point_v<T>; };

constexpr inline double toDeg(double rad) { return rad * 180 / pi; }

constexpr inline double toRad(double deg) { return deg * pi / 180; }
