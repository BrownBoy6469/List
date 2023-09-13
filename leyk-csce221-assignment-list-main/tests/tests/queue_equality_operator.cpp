#include "executable.h"
#include "Queue.h"

#include <vector>
#include <queue>

std::queue<int> q;

TEST(queue_equality_operator) {
    Typegen t;

    for (size_t i = 0; i < TEST_ITER; i++) {
        // Generate a reference vector
        const size_t n = t.range(10ULL, 0x999ULL);
        std::vector<int> gt_1(n);
        t.fill(gt_1.begin(), gt_1.end());

        // Checking two equivalent queues
        Queue<int> q1;
        Queue<int> q2;

        {
            Memhook mh;

            // Populate the queues with the refernce vector data
            for (size_t j = 0; j < n; j++) {
                q1.push(gt_1[i]);
                q2.push(gt_1[i]);
            }

            // Check that the queues were properly setup
            ASSERT_EQ(gt_1.size(), q1.size());
            ASSERT_EQ(gt_1.size(), q2.size());
            ASSERT_EQ(2 * n, mh.n_allocs());
        }

        // The queues should have the same data
        ASSERT_EQ(true, q1 == q2);

        // Generate a different reference vector
        std::vector<int> gt_2(n);
        t.fill(gt_2.begin(), gt_2.end());

        Queue<int> q3;

        {
            Memhook mh;

            // Populate the queue with the reference vector data
            for (size_t j = 0; j < n; j++) {
                q3.push(gt_2[i]);
            }

            // Ensure that the queue was properly setup
            ASSERT_EQ(gt_2.size(), q3.size());
            ASSERT_EQ(n, mh.n_allocs());
        }

        // The queues should be different
        ASSERT_EQ(false, q1 == q3);
        ASSERT_EQ(false, q3 == q2);

        // Comparisons between queues of different sizes should be false
        // Generate a smaller reference vector
        const size_t n_smaller = t.range(0UL, n - 1);
        std::vector<int> gt_3(n_smaller);
        t.fill(gt_3.begin(), gt_3.end());

        Queue<int> q4;

        {
            Memhook mh;

            // Populate the queue with the smaller reference vector
            for (size_t j = 0; j < n_smaller; j++) {
                q4.push(gt_3[i]);
            }

            // Ensure the queue was properly setup
            ASSERT_EQ(gt_3.size(), q4.size());
            ASSERT_EQ(n_smaller, mh.n_allocs());
        }

        // Queues of different sizes should always be false
        ASSERT_EQ(false, q1 == q4);
        ASSERT_EQ(false, q4 == q3);

    }
}