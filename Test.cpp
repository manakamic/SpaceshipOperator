#include "Test.h"

#ifndef DEFAULT_OPERATOR
namespace {
    // 指数表記(1.0 x 10^-6) == (0.000001f) をイプシロンとして使用
    constexpr auto ABSOLUTE_EPSILON = 1.0e-6f; // 絶対値
    constexpr auto RELATIVE_EPSILON = 1.0e-6f; // 相対値
}

std::strong_ordering Test::operator <=>(const Test& other) const {
    // C++17 の初期化子付きif文 (if statement with initializer)
    // 本来は
    // cmppare != std::strong_ordering::equivalent
    // だが std::weak_ordering や std::partial_ordering でも
    // equivalent は 0 なので慣例として 0 で比較する
    if (auto compare = num <=> other.num; compare != 0) {
        return compare;
    }

    if (auto cmppare = IsEqualFloat(value, other.value); cmppare != 0) {
        return cmppare;
    }

    return str <=> other.str;
}

bool Test::operator ==(const Test& other) const {
    // float の比較は == を使用してはいけない
    // 冗長な if ネストだが、明示的に比較コストが低い順に比較しているを明示
    if (num == other.num) {
        if (auto compare = IsEqualFloat(value, other.value); compare == 0) {
            if (str == other.str) { // std::string は文字列長さ大きいと比較コストが高いので最後に比較
                return true;
            }
        }
    }

    return false;
}

std::strong_ordering Test::IsEqualFloat(const float a, const float b) noexcept {
    if (a == b) {
        return std::strong_ordering::equivalent; // 同じ値
    }

    auto aNaN = IsNaN(a);
    auto bNaN = IsNaN(b);

    // NaN の場合は等価ではないが、例外として両方NaNの場合は等価とする
    if (aNaN && bNaN) {
        return std::strong_ordering::equivalent;
    }

    if (aNaN || bNaN) {
        return aNaN ? std::strong_ordering::less : std::strong_ordering::greater;
    }

    // イプシロン比較
    if (IsEqual(a, b)) {
        return std::strong_ordering::equivalent;
    }

    if (a < b) {
        return std::strong_ordering::less;
    }

    if (a > b) {
        return std::strong_ordering::greater;
    }

    // それ以外は等価だが、通常はここに到達しないはず
    std::unreachable(); // C++23 の std::unreachable() を使用して、ここに到達しないことを明示
}

// イプシロン比較（絶対誤差）
bool Test::IsEqual(const float a, const float b) noexcept {
    if (a == b) {
        return true; // 同じ値
    }

    return std::abs(a - b) <= ABSOLUTE_EPSILON;
}

// 大きな値の float で相対誤差を使用したい場合
// 使用していないが参考用に実装
bool Test::IsEqualLargeFloat(const float a, const float b) noexcept {
    if (a == b) {
        return true; // 同じ値
    }

    // 大きな値の比較では相対誤差を使用
    auto relative = (std::max)(std::abs(a), std::abs(b)) * RELATIVE_EPSILON;

    // 相対誤差の比較
    return std::abs(a - b) <= (std::max)(relative, ABSOLUTE_EPSILON);
}

// NaN判定
bool Test::IsNaN(const float value) noexcept {
    return std::isnan(value);
}

#endif
