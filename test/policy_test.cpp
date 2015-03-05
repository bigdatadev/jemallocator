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

#include <vector>

namespace je = jemallocator;
namespace jepolicy = jemallocator::jepolicy;

template<class T, class Alloc>
using vector = std::vector<T, Alloc>;

class static_counter {
	static size_t s_allocations;
	static size_t s_alloc_bytes;
public:
	void audit_allocate(const std::type_info& type, size_t n, size_t bytes, int flags) {
		s_allocations += n;
		s_alloc_bytes += bytes;
	}
	void audit_deallocate(const std::type_info& type, size_t n, size_t bytes) {
		s_allocations -= n;
		s_alloc_bytes -= bytes;
	}
	static int get_allocations() { return s_allocations; }
	static int get_alloc_bytes() { return s_alloc_bytes; }
};

size_t static_counter::s_allocations;
size_t static_counter::s_alloc_bytes;

class flag_checker {
	static int s_flags;
public:
	void audit_allocate(const std::type_info& type, size_t n, size_t bytes, int flags) {
		s_flags = flags;
	}
	void audit_deallocate(const std::type_info& type, size_t n, size_t bytes) {}
	static void expect(int f) { REQUIRE(s_flags == f); }
};

int flag_checker::s_flags;

TEST_CASE("standard jemallocator policies work", "[policy]") {
	typedef int value_type;
	SECTION("audit policy") {
		using test_allocator = je::jemallocator<value_type, jepolicy::audit<static_counter>::policy>;
		vector<value_type, test_allocator>t;
		REQUIRE(static_counter::get_allocations() == 0);

		t.reserve(64);
		REQUIRE(static_counter::get_allocations() >= 64);
		REQUIRE(static_counter::get_alloc_bytes() == static_counter::get_allocations() * sizeof(int));
	}
	SECTION("empty policy") {
		using test_allocator = je::jemallocator<value_type, jepolicy::empty::policy, jepolicy::audit<flag_checker>::policy>;
		vector<value_type, test_allocator> t;
		t.reserve(64);
		flag_checker::expect(0);
	}
	SECTION("arena policy") {
		using test_allocator = je::jemallocator<value_type, jepolicy::arena<0>::policy, jepolicy::audit<flag_checker>::policy>;
		vector<value_type, test_allocator> t;
		t.reserve(64);
		flag_checker::expect(MALLOCX_ARENA(0));
	}
	SECTION("align policy") {
		using test_allocator = je::jemallocator<value_type, jepolicy::align<16>::policy, jepolicy::audit<flag_checker>::policy>;
		vector<value_type, test_allocator> t;
		t.reserve(64);
		flag_checker::expect(MALLOCX_ALIGN(16));
	}
	SECTION("zero policy") {
		using test_allocator = je::jemallocator<value_type, jepolicy::zero<true>::policy, jepolicy::audit<flag_checker>::policy>;
		vector<value_type, test_allocator> t;
		t.reserve(64);
		flag_checker::expect(MALLOCX_ZERO);
	}
	SECTION("thread cache policy") {
		using test_allocator = je::jemallocator<value_type, jepolicy::tcache<false>::policy, jepolicy::audit<flag_checker>::policy>;
		vector<value_type, test_allocator> t;
		t.reserve(64);
		flag_checker::expect(MALLOCX_TCACHE_NONE);
	}
	SECTION("bad alloc policy") {
		using test_allocator = je::jemallocator<value_type, jepolicy::badalloc<void>::policy>;
		vector<value_type, test_allocator>t;
		t.reserve(64);
	}
}

TEST_CASE("jemallocator policies can be composited", "[policy]") {
	typedef int value_type;
	SECTION("empty policy") {
		using test_allocator = je::jemallocator<value_type,
				jepolicy::arena<0>::policy,
				jepolicy::align<16>::policy,
				jepolicy::zero<true>::policy,
				jepolicy::badalloc<void>::policy,
				jepolicy::audit<static_counter>::policy,
				jepolicy::audit<flag_checker>::policy>;
		vector<value_type, test_allocator> t;
		t.reserve(64);
		REQUIRE(static_counter::get_allocations() == 64);
		REQUIRE(static_counter::get_alloc_bytes() == static_counter::get_allocations() * sizeof(int));
		flag_checker::expect(MALLOCX_ARENA(0) | MALLOCX_ALIGN(16) | MALLOCX_ZERO);
	}
}
