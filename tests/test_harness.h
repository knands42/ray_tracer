#pragma once
// A minimal test harness — no external dependencies.
// Drop this in your project and include it from a test .cpp file.

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>

struct TestResult {
    std::string name;
    bool passed;
    std::string message;
};

static std::vector<TestResult> g_results;

#define ASSERT_TRUE(expr)                                                      \
    if (!(expr)) {                                                             \
        return {#expr, false, "ASSERT_TRUE failed: " #expr};                   \
    }

#define ASSERT_FALSE(expr)                                                     \
    if ((expr)) {                                                              \
        return {#expr, false, "ASSERT_FALSE failed: " #expr};                  \
    }

#define ASSERT_NEAR(a, b, eps)                                                 \
    if (std::fabs((a) - (b)) > (eps)) {                                        \
        return {#a " ~= " #b, false,                                           \
                "ASSERT_NEAR failed: " #a "=" + std::to_string(a) +            \
                " vs " #b "=" + std::to_string(b) + " eps=" + std::to_string(eps)}; \
    }

#define ASSERT_GT(a, b)                                                        \
    if (!((a) > (b))) {                                                        \
        return {#a " > " #b, false,                                            \
                "ASSERT_GT failed: " #a "=" + std::to_string(a) +              \
                " vs " #b "=" + std::to_string(b)};                            \
    }

// Define a test case. Usage:
//   TEST(my_test_name) { ... return {"", true, ""}; }
#define TEST(name)                                                             \
    static TestResult test_##name();                                          \
    struct test_##name##_registrar {                                           \
        test_##name##_registrar() {                                            \
            g_results.push_back(test_##name());                                \
        }                                                                      \
    } test_##name##_instance;                                                  \
    static TestResult test_##name()

inline int run_all_tests() {
    int failures = 0;
    for (auto& r : g_results) {
        if (r.passed) {
            std::cout << "  [PASS] " << r.name << "\n";
        } else {
            std::cout << "  [FAIL] " << r.name;
            if (!r.message.empty())
                std::cout << " — " << r.message;
            std::cout << "\n";
            ++failures;
        }
    }
    std::cout << "\n" << (g_results.size() - failures) << "/" << g_results.size()
              << " passed";
    if (failures) std::cout << " (" << failures << " FAILURES)";
    std::cout << "\n";
    return failures;
}
