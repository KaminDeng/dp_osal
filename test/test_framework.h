//
// Created by kamin.deng on 2024/8/22.
//

#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

#include <string>
#include <vector>
#include <functional>

#include "osal_debug.h"
#include "osal_test_framework_config.h"

void runAllTests();
void TestOSALPackFunc(std::function<int(void *)> taskFunction);

#define OSAL_ASSERT_TRUE(condition) OSAL_ASSERT_EQ(condition, true)
#define OSAL_ASSERT_FALSE(condition) OSAL_ASSERT_EQ(condition, false)

#define OSAL_ASSERT_EQ(expected, actual)                                                            \
    if ((expected) != (actual)) {                                                                   \
        OSAL_LOGI("FAILED (%s == %s, file %s, line %d)\n", #expected, #actual, __FILE__, __LINE__); \
        return 1;                                                                                   \
    }

class TestCase {
public:
    virtual ~TestCase() {}

    virtual int run() = 0;

    virtual std::string name() const = 0;

    int run_flag = 0;
};

class TestRegistry {
public:
    static TestRegistry &instance() {
        static TestRegistry instance;
        return instance;
    }

    void addTest(TestCase *test) { tests.push_back(test); }

    const std::vector<TestCase *> &getTests() const { return tests; }

private:
    TestRegistry() {}

    std::vector<TestCase *> tests;
};

#define TEST_CASE(test_name)                                     \
    class test_name : public TestCase {                          \
    public:                                                      \
        test_name() {                                            \
            TestRegistry::instance().addTest(this);              \
            run_flag = test_name##Enabled;                       \
        }                                                        \
        int run() override;                                      \
        std::string name() const override { return #test_name; } \
    };                                                           \
    test_name instance_##test_name;                              \
    int test_name::run()

#endif  // TEST_FRAMEWORK_H
