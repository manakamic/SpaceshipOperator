#pragma once
#include <compare>
#include <string>
#include <utility> // std::move
#include <cmath> // std::fabs, std::abs
#include <limits> // std::numeric_limits
//#define DEFAULT_OPERATOR

class Test {
public:
    Test() = default;
    Test(int n, float v, std::string s) : num(n), value(v), str(std::move(s)) {}
    ~Test() = default;

    // コピーコンストラクタと代入演算子
    Test(const Test& other) = default;
    Test& operator=(const Test& other) = default;

    // ムーブコンストラクタと代入演算子
    Test(Test&& other) noexcept = default;
    Test& operator=(Test&& other) noexcept = default;

    // 三方比較演算子
#ifdef DEFAULT_OPERATOR
    // float(NaN があり得る) を含むため std::partial_ordering になる
    // std::strong_ordering にしたい場合は、NaN の扱いを変更する必要がある
    auto operator <=>(const Test& other) const = default;
    // <=> を定義すると、比較演算子も自動的に生成される
    // (値の比較 -> 同インスタンスのチェックでは無い事に注意)
    // が明示的に定義する
    bool operator ==(const Test& other) const = default;
#else
    // std::strong_ordering にしたい場合
    // float の比較を明示的に行う必要がある
    std::strong_ordering operator <=>(const Test& other) const;
    bool operator ==(const Test& other) const;

private:
    static std::strong_ordering IsEqualFloat(const float a, const float b) noexcept;
    static bool IsEqual(const float a, const float b) noexcept;
    static bool IsEqualLargeFloat(const float a, const float b) noexcept;
    static bool IsNaN(const float value) noexcept;
#endif

private:
    int num{ 0 };
    float value{ 0.0f };
    std::string str;
};

