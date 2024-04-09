/*
 * Copyright 2016 Facebook, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "common/serialize/MsgPacker.hpp"

#include "glog/logging.h"
#include "gtest/gtest.h"

//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////

TEST(msgPacker, types) {
	{
		msgpack::sbuffer buf;
		msgpack::packer<msgpack::sbuffer> packer{ buf };

		const char* pkey = NULL;
		const char pkey2[] = "const char[N] return N-1";
		char pkey3[] = "Char[N] call strnlen(,N-1)";
		int arr[] = { 100, 101, 102 };

		MsgPacker::packItem(packer,
			Item_Object{
				{ pkey, 1 },
				{ pkey2, 2 },
				{ pkey3, "hello" },
				{ "ItemArray", Item_Array
								{ 
									(int16_t)1, 
									2, 
									Item_Object
										{ 
											{ "ItemD", 3 } 
										} 
								} 
				},
				{ "IntArray", Item_Array{ 1, 2, 3, 4, 5 } },
				{ "IntArray2", Item_ArrayRange{ arr, arr + 3 } },
				{ "Bin", Item_BinaryData{ arr, arr + 3 } },
				{ "Int8", int8_t(~0) },
				{ "Int16", int16_t(~0) },
				{ "Int32", int32_t(~0) },
				{ "Int64", int64_t(~0) },
				{ "UInt8", uint8_t(~0) },
				{ "UInt16", uint16_t(~0) },
				{ "UInt32", uint32_t(~0) },
				{ "UInt64", uint64_t(~0) },
				{ "True", true },
				{ "False", false },
				{ "Float", 1.23f },
				{ "Double", 1.23 },
				{ "Null", nullptr }
			});
		msgpack::object_handle oh = msgpack::unpack(buf.data(), buf.size());
		msgpack::object deserialized = oh.get();
		std::map<std::string, msgpack::object> dst;
		deserialized.convert(dst);
		
		EXPECT_EQ(dst[""].as<int>(), 1);
		EXPECT_EQ(dst[pkey2].as<int>(), 2);
		EXPECT_EQ(dst[pkey3].as<std::string>(), "hello");
		{
			msgpack::object itemArray = dst["ItemArray"].convert();
			EXPECT_EQ(itemArray.via.array.size, 3);

			EXPECT_EQ(itemArray.via.array.ptr[0].as<int16_t>(), 1);
			EXPECT_EQ(itemArray.via.array.ptr[1].as<int>(), 2);
			msgpack::object itemD = itemArray.via.array.ptr[2].as<msgpack::object>();
			EXPECT_EQ(itemD.via.map.ptr[0].key.as<std::string>(), "ItemD");
			EXPECT_EQ(itemD.via.map.ptr[0].val.as<int>(), 3);
		}
		{
			int intArray[5];
			dst["IntArray"].convert(intArray);
			EXPECT_TRUE(intArray[0] == 1 && intArray[1] == 2 && intArray[2] == 3 && intArray[3] == 4 && intArray[4] == 5);
		}
		{
			int intArray2[3];
			dst["IntArray2"].convert(intArray2);
			EXPECT_EQ(dst["IntArray2"].via.array.size, 3);
			EXPECT_TRUE(intArray2[0] == 100 && intArray2[1] == 101 && intArray2[2] == 102);
		}
		{
			char bin[100];
			dst["Bin"].convert(bin);
			EXPECT_EQ(dst["Bin"].via.bin.size, 12);
			EXPECT_TRUE(((int*)bin)[0] == 100 && ((int*)bin)[1] == 101 && ((int*)bin)[2] == 102);
		}
		EXPECT_EQ(dst["Int8"].as<int64_t>(), int8_t(~0));
		EXPECT_EQ(dst["Int16"].as<int64_t>(), int16_t(~0));
		EXPECT_EQ(dst["Int32"].as<int64_t>(), int32_t(~0));
		EXPECT_EQ(dst["Int64"].as<int64_t>(), int64_t(~0));

		EXPECT_EQ(dst["UInt8"].as<uint64_t>(), uint8_t(~0));
		EXPECT_EQ(dst["UInt16"].as<uint64_t>(), uint16_t(~0));
		EXPECT_EQ(dst["UInt32"].as<uint64_t>(), uint32_t(~0));
		EXPECT_EQ(dst["UInt64"].as<uint64_t>(), uint64_t(~0));

		EXPECT_EQ(dst["Float"].as<float>(), 1.23f);
		EXPECT_EQ(dst["Double"].as<double>(), 1.23);

		EXPECT_TRUE(dst["Null"].is_nil());

		EXPECT_TRUE(dst["True"].via.boolean == true);
		EXPECT_TRUE(dst["False"].via.boolean == false);
	}
}
