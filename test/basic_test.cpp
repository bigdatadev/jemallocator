/*
 * Copyright (C) 2015 Christopher Gilbert <christopher.john.gilbert@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "catch.hpp"

#include <jemallocator/jemallocator.hpp>

namespace je = jemallocator;
namespace jepolicy = jemallocator::jepolicy;


TEST_CASE("jemallocator behaves as a standard allocator", "[basic]") {
	SECTION("constructor") {
		je::jemallocator<int, jepolicy::empty::policy> test_1;

		SECTION("copy construct from same type") {
			je::jemallocator<int, jepolicy::empty::policy> test_2(test_1);
		}
		SECTION("copy construct from different type") {
			je::jemallocator<char, jepolicy::empty::policy> test_2(test_1);
		}
	}
	SECTION("allocation and deallocation") {
		je::jemallocator<int, jepolicy::empty::policy> alloc;

		SECTION("allocate and deallocate") {
			int* ptr = alloc.allocate(1);
			REQUIRE(ptr != nullptr);

			SECTION("address") {
				REQUIRE(alloc.address(*ptr) == ptr);
			}
			SECTION("construct and destroy same type") {
				alloc.construct(&ptr[0], 1337);
				REQUIRE(ptr[0] == 1337);
				alloc.destroy(&ptr[0]);
			}
			SECTION("construct and destroy different type") {
				alloc.construct((char*)ptr, 'c');
				REQUIRE((char)*ptr == 'c');
				alloc.destroy((char*)ptr);
			}
			alloc.deallocate(ptr, 1);
		}
		SECTION("max size") {
			REQUIRE(alloc.max_size() == (size_t(0) - size_t(1)) / sizeof(int));
		}
	}
	SECTION("equality") {
		je::jemallocator<int, jepolicy::empty::policy> test_1;

		SECTION("instances should be interchangeable") {
			je::jemallocator<char, jepolicy::empty::policy> test_2;
			REQUIRE(test_1 == test_2);
		}
		SECTION("specialisations should be interchangeable") {
			je::jemallocator<long, jepolicy::arena<0>::policy> test_2;
			REQUIRE(test_1 == test_2);
		}
	}
}
