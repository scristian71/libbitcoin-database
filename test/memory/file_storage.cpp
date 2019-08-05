/**
 * Copyright (c) 2011-2019 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <boost/test/unit_test.hpp>

#include <bitcoin/database.hpp>
#include "../utility/utility.hpp"

using namespace bc;
using namespace bc::database;
using namespace bc::system;

// Test directory
#define DIRECTORY "file_storage"

struct file_storage_directory_setup_fixture
{
    file_storage_directory_setup_fixture()
    {
        test::clear_path(DIRECTORY);
        log::initialize();
    }
};

BOOST_FIXTURE_TEST_SUITE(file_storage_tests, file_storage_directory_setup_fixture)

BOOST_AUTO_TEST_CASE(file_storage__constructor1__always__leaves_file)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(test::exists(file));
}

BOOST_AUTO_TEST_CASE(file_storage__constructor2__always__leaves_file)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file, 42, 42);
    BOOST_REQUIRE(test::exists(file));
}

BOOST_AUTO_TEST_CASE(file_storage__open__from_closed__success)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.open());
}

BOOST_AUTO_TEST_CASE(file_storage__open__from_opened__failure)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.open());
    BOOST_REQUIRE(!instance.open());
}

BOOST_AUTO_TEST_CASE(file_storage__close__from_closed__success)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.close());
}

BOOST_AUTO_TEST_CASE(file_storage__close__from_opened__success)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.open());
    BOOST_REQUIRE(instance.close());
}

BOOST_AUTO_TEST_CASE(file_storage__size__default__default_capacity)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE_EQUAL(instance.capacity(), file_storage::default_capacity);
}

BOOST_AUTO_TEST_CASE(file_storage__resize__closed__throws_runtime_error)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE_THROW(instance.resize(42), std::runtime_error);
}

// TODO: externally verify open/closed file size 42.
BOOST_AUTO_TEST_CASE(file_storage__resize__open__expected)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.open());
    BOOST_REQUIRE(instance.resize(42));
    BOOST_REQUIRE_EQUAL(instance.capacity(), 42u);
}

BOOST_AUTO_TEST_CASE(file_storage__reserve__closed__throws_runtime_error)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE_THROW(instance.reserve(42), std::runtime_error);
}

// TODO: externally verify open file size 150, closed file size 100.
BOOST_AUTO_TEST_CASE(file_storage__reserve__open_default_minimum__expected_file_size)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.open());
    BOOST_REQUIRE(instance.reserve(100));
    BOOST_REQUIRE_EQUAL(instance.capacity(), 150u);
}

// TODO: externally verify open file size 100, closed file size 42.
BOOST_AUTO_TEST_CASE(file_storage__reserve__minimum_no_expansion__expected_file_size)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file, 100, 0);
    BOOST_REQUIRE(instance.open());
    BOOST_REQUIRE(instance.reserve(42));
    BOOST_REQUIRE_EQUAL(instance.capacity(), 100u);
}

// TODO: externally verify open file size 142, closed file size 100.
BOOST_AUTO_TEST_CASE(file_storage__reserve__no_minimum_expansion__expected_file_size)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file, 0, 42);
    BOOST_REQUIRE(instance.open());
    BOOST_REQUIRE(instance.reserve(100));
    BOOST_REQUIRE_EQUAL(instance.capacity(), 142u);
}

// Causes boost assert.
////BOOST_AUTO_TEST_CASE(file_storage__access__closed__throws_runtime_error)
////{
////    static const std::string file = DIRECTORY "/" + TEST_NAME;
////    BOOST_REQUIRE(test::create(file));
////    file_storage instance(file);
////    BOOST_REQUIRE_THROW(instance.access(), std::runtime_error);
////}

// TODO: externally verify file size.
BOOST_AUTO_TEST_CASE(file_storage__access__open__expected)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.open());
    BOOST_REQUIRE(instance.access());
}

BOOST_AUTO_TEST_CASE(file_storage__flush__closed__success)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.flush());
}

BOOST_AUTO_TEST_CASE(file_storage__flush__open__success)
{
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.open());
    BOOST_REQUIRE(instance.flush());
}

BOOST_AUTO_TEST_CASE(file_storage__write__read__expected)
{
    const uint64_t expected = 0x0102030405060708;
    static const std::string file = DIRECTORY "/" + TEST_NAME;
    BOOST_REQUIRE(test::create(file));
    file_storage instance(file);
    BOOST_REQUIRE(instance.open());
    auto memory = instance.reserve(sizeof(uint64_t));
    BOOST_REQUIRE(memory);
    auto serial = make_unsafe_serializer(memory->buffer());
    serial.write_big_endian<uint64_t>(expected);
    memory.reset();
    BOOST_REQUIRE(instance.flush());
    memory = instance.access();
    auto deserial = make_unsafe_deserializer(memory->buffer());
    BOOST_REQUIRE_EQUAL(deserial.read_big_endian<uint64_t>(), expected);
}

BOOST_AUTO_TEST_SUITE_END()
