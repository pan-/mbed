/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "greentea-client/test_env.h"
#include "unity/unity.h"
#include "utest/utest.h"
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <math.h>

using namespace utest::v1;

template<size_t I>
struct memory_chunk_t { 
    uint8_t data[I];
};

template<size_t limit, size_t i = 0>
struct memory_allocation_test { 
    static bool execute() { 
        typedef memory_chunk_t<(1ULL << i)> chunk_t;

        printf("allocating %u bytes\r\n", 1 << i);
        chunk_t* ptr = new(std::nothrow) chunk_t;
        if (ptr) { 
            bool result = memory_allocation_test<limit, i + 1>::execute();
            delete ptr;
            return result;
        } else { 
            return true;
        }
    }
};

template<size_t limit>
struct memory_allocation_test<limit, limit> { 
    static bool execute() { 
        return false;
    }
};

void test_case_nothrow_new() {
    bool result = memory_allocation_test<32>::execute(); 
    TEST_ASSERT_TRUE(result);
}

utest::v1::status_t greentea_failure_handler(const Case *const source, const failure_t reason) {
    greentea_case_failure_abort_handler(source, reason);
    return STATUS_CONTINUE;
}

Case cases[] = {
    Case("new nothrow", test_case_nothrow_new, greentea_failure_handler),
};

utest::v1::status_t greentea_test_setup(const size_t number_of_cases) {
    GREENTEA_SETUP(5, "default_auto");
    return greentea_test_setup_handler(number_of_cases);
}

Specification specification(greentea_test_setup, cases, greentea_test_teardown_handler);

int main() {
    Harness::run(specification);
}
