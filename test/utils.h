#ifndef ENCFS_TEST_UTILS_H_
#define ENCFS_TEST_UTILS_H_
#include <gtest/gtest.h>
#include <vector>


namespace encfs
{
namespace test_utils
{
template <class T>
::testing::AssertionResult IsVectorEqual(const std::vector<T> a,
                                         const std::vector<T> b)
{
    ::testing::AssertionResult result = ::testing::AssertionFailure();
    if (a.size() != b.size()) {
        result << "Difference found: size " << a.size() << " != " << b.size()
               << ".";
        return result;
    }
    for (decltype(a.size()) i = 0; i < a.size(); ++i) {
        if (a[i] != b[i]) {
            result << "Difference found: index [" << i << "] with a: " << a[i]
                   << ", b: " << b[i] << ".";
            return result;
        }
    }
    return ::testing::AssertionSuccess();
}


}  // namespace test_utils
}  // namespace encfs

#endif /* ENCFS_TEST_UTILS_H_ */
