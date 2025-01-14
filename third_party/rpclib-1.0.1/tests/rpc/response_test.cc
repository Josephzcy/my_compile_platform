#include "gtest/gtest.h"

#include "testutils.h"
#include <atomic>
#include <memory>
#include <thread>

#include "rpc/detail/response.h"

using namespace rpc::testutils;
using namespace rpc::detail;

TEST(response, object_ctor) {
    auto o = make_unpacked(3, 42, "foo", "bar");
    response r(std::move(o));
    EXPECT_EQ(r.get_id(), 42);
    std::string error = r.get_error()->get().as<std::string>();
    EXPECT_TRUE(error == "foo");

    std::string result = r.get_result()->get().as<std::string>();
    EXPECT_TRUE(result == "bar");
}

TEST(response, writing) {
    auto obj = make_unpacked(1, 42, "foo", "bar");
    response r(std::move(obj));
    auto buf1 = r.get_data();
    msgpack::zone z;
    response::response_type same_obj(1, 42,
                                     msgpack::object("foo", z),
                                     msgpack::object("bar", z));
    msgpack::sbuffer buf2;
    msgpack::pack(buf2, same_obj);

    EXPECT_EQ(buf1.size(), buf2.size());
    EXPECT_EQ(0, memcmp(buf2.data(), buf2.data(), buf1.size()));
}
