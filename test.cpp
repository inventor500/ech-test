/*
     ech-test - Check if ECH is supported for a domain.
     Copyright (C) 2024 Isaac Ganoung

     This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

     This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/

#include <gtest/gtest.h>
#include "dns.hpp"
#include "request.hpp"

// Test what happens if the ECH value returned by the DNS is completely invalid
TEST(CurlConnection, InvalidECHValue) {
	std::string url{"https://example.com"},
		echValue{"ThisIsNotAValidECHKey"};
	ASSERT_FALSE(testConnect(url, echValue));
}

// Test what happens if the ECH value is real, but for the wrong domain
TEST(CurlConnection, WrongECHValue) {
	std::string url{"https://example.com"},
		echValue{"AEX+DQBBXgAgACAZsl1TUpkPk6VyO8sDX3HHwzw8dE67n2+zi6v8kykuGAAEAAEAAQASY2xvdWRmbGFyZS1lY2guY29tAAA="};
	ASSERT_FALSE(testConnect(url, echValue));
}

int main(void) {
	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
}
