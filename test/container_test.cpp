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

#include <deque>
#include <forward_list>
#include <list>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <stack>
#include <string>

namespace je = jemallocator;
namespace jepolicy = jemallocator::jepolicy;

/*
 * Useful allocator alias
 */
template<class T>
using basic_allocator = je::jemallocator<T, jepolicy::empty::policy>;

template<class T>
using default_allocator = basic_allocator<T>;

/*
 * Useful container aliases
 */
template<class T, class Alloc = default_allocator<T>>
using forward_list = std::forward_list<T, Alloc>;

template<class T, class Alloc = default_allocator<T>>
using deque = std::deque<T, Alloc>;

template<class T, class Alloc = default_allocator<T>>
using list = std::list<T, Alloc>;

template<class T, class Alloc = default_allocator<T>>
using vector = std::vector<T, Alloc>;

template<class Key, class T, class Alloc = default_allocator<std::pair<const Key, T>>>
using map = std::map<Key, T, std::less<Key>, Alloc>;

template<class Key, class T, class Alloc = default_allocator<std::pair<const Key, T>>>
using multimap = std::multimap<Key, T, std::less<Key>, Alloc>;

template<class T, class Alloc = default_allocator<T>>
using multiset = std::multiset<T, std::less<T>, Alloc>;

template<class T, class Alloc = default_allocator<T>>
using set = std::set<T, std::less<T>, Alloc>;

template<class Key, class T, class Alloc = default_allocator<std::pair<const Key, T>>>
using unordered_map = std::unordered_map<Key, T, std::hash<Key>, std::equal_to<Key>, Alloc>;

template<class Key, class T, class Alloc = default_allocator<std::pair<const Key, T>>>
using unordered_multimap = std::unordered_multimap<Key, T, std::hash<Key>, std::equal_to<Key>, Alloc>;

template<class Key, class Alloc = default_allocator<Key>>
using unordered_multiset = std::unordered_multiset<Key, std::hash<Key>, std::equal_to<Key>, Alloc>;

template<class Key, class Alloc = default_allocator<Key>>
using unordered_set = std::unordered_set<Key, std::hash<Key>, std::equal_to<Key>, Alloc>;

template<class T, class Alloc = default_allocator<T>>
using stack = std::stack<T, Alloc>;

template<class T, class Alloc = default_allocator<T>>
using queue = std::queue<T, Alloc>;

template<class T, class Alloc = default_allocator<T>>
using priority_queue = std::priority_queue<T, std::vector<T, Alloc>, std::less<typename std::vector<T, Alloc>::value_type>>;

template<class T, class Alloc = default_allocator<T>>
using basic_string = std::basic_string<T, std::char_traits<T>, Alloc>;

using string = basic_string<char>;
using wstring = basic_string<wchar_t>;
using u16string = basic_string<char16_t>;
using u32string = basic_string<char32_t>;


TEST_CASE("jemallocator will work with standard containers", "[containers]") {
	SECTION("sequence containers") {
		SECTION("forward_list") {
			forward_list<int> container;
			container.push_front(0);
		}
		SECTION("deque") {
			deque<int> container;
			container.push_back(0);
		}
		SECTION("list") {
			list<int> container;
			container.push_back(0);
		}
		SECTION("vector") {
			vector<int> container;
			container.push_back(0);
		}
	}
	SECTION("associative containers") {
		SECTION("map") {
			map<int, int> container;
			container[0] = 0;
		}
		SECTION("multimap") {
			multimap<int, int> container;
			container.insert(std::make_pair(0, 0));
		}
		SECTION("multiset") {
			multiset<int> container;
			container.insert(0);
		}
		SECTION("set") {
			set<int> container;
			container.insert(0);
		}
	}
	SECTION("unordered associative containers") {
		SECTION("unordered_map") {
			unordered_map<int, int> container;
			container[0] = 0;
		}
		SECTION("unordered_multimap") {
			unordered_multimap<int, int> container;
			container.insert(std::make_pair(0, 0));
		}
		SECTION("unordered_multiset") {
			unordered_multiset<int> container;
			container.insert(0);
		}
		SECTION("unordered_set") {
			unordered_set<int> container;
			container.insert(0);
		}
	}
	SECTION("container adaptors") {
		SECTION("stack") {
			stack<int> container;
		}
		SECTION("queue") {
			queue<int> container;
		}
		SECTION("priority_queue") {
			priority_queue<int> container;
		}
	}
	SECTION("strings") {
		SECTION("string") {
			basic_string<char> container;
		}
		SECTION("wstring") {
			basic_string<wchar_t> container;
		}
	}
}
