#include "dns.hpp"
#include "request.hpp"
#include <iostream>
#include <iomanip>
#include <optional>
#include <numeric>
#include <algorithm>

std::optional<bool> doTest(const std::string& domain, const ldns_resolver *res) {
	try {
		std::string ech = domain;
		doQuery(ech, res);
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
	ldns_resolver *res;
	ldns_status s = ldns_resolver_new_frm_file(&res, NULL); // Included as part of dns.hpp
	if (s != LDNS_STATUS_OK) {
		ldns_resolver_deep_free(res);
		std::cerr << "Unable to create DNS resolver: " << ldns_get_errorstr_by_id(s) << std::endl;
	}
	for (int i = 1; i < argc; i++) {
		std::optional<bool> result = doTest({argv[i]}, res);
		std::cout << std::left << std::setw(maxDomainLength+1) << argv[i] << " ";
		if (result.has_value()) {
			std::cout << std::boolalpha << result.value() << std::endl;
		} else {
			std::cout << "null" << std::endl;
		}
	}
	ldns_resolver_deep_free(res);
	return 0;
}
