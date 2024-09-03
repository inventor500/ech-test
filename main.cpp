/*
     ech-test - Check if ECH is supported for a domain.
     Copyright (C) 2024 Isaac Ganoung

     This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

     This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/


#include "dns.hpp"
#include "request.hpp"
#include <iostream>
#include <iomanip>
#include <optional>
#include <numeric>
#include <algorithm>

std::optional<bool> doTest(const std::string& domain, Resolver* res) {
	try {
		std::string ech = domain;
		res->doQuery(ech);
		if (ech == "") { // No DNS entry
			return false;
		}
		return testConnect("https://" + domain, ech);
	} catch (const std::runtime_error& err) {
		return std::nullopt;
	}
}

int main(int argc, const char *argv[]) {
	if (argc == 1) {
		std::cout << "Usage: " << argv[0] << " <domain>" << std::endl;
		return 1;
	}
	size_t maxDomainLength = std::transform_reduce(
		argv+1, argv+argc, 0,
		[](size_t a, size_t b){return std::max(a, b);},
		[](const char* c){return std::string{c}.size();});
	if (maxDomainLength == 0) {
		std::cerr << "Received no valid input domains" << std::endl;
	}
	Resolver* res = createResolver();
	try {
		for (int i = 1; i < argc; i++) {
			std::optional<bool> result = doTest({argv[i]}, res);
			std::cout << std::left << std::setw(maxDomainLength+1) << argv[i] << " ";
			if (result.has_value()) {
				std::cout << std::boolalpha << result.value() << std::endl;
			} else {
				std::cout << "null" << std::endl;
			}
		}
	} catch (const std::exception& e) { // Make sure that res gets freed
		delete res;
		throw;
	}
	delete res;
	return 0;
}
