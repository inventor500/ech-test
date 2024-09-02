#include <iostream>
#include <iomanip>
#include "dns.hpp"

int main(int argc, const char *argv[]) {
	if (argc == 1) {
		std::cout << "Usage: " << argv[0] << " <domain>" << std::endl;
		return 1;
	}
	ldns_resolver *res;
	ldns_status s = ldns_resolver_new_frm_file(&res, NULL);
	if (s != LDNS_STATUS_OK) {
		ldns_resolver_deep_free(res);
		std::cerr << "Unable to create resolver: " << ldns_get_errorstr_by_id(s) << std::endl;
	}
	for (int i = 1; i < argc; i++) {
		try {
			std::string domain{argv[i]};
			doQuery(domain, res);
			std::cout << std::left << std::setw(25) << argv[i] << std::boolalpha << " " << (domain != "") << std::endl;
		} catch (const std::runtime_error& err) {
			std::cerr << std::left << std::setw(25) << argv[i] << " null" << std::endl;
		}
	}
	ldns_resolver_deep_free(res);
	return 0;
}

// Returns just the ech section if it exists, otherwise returns an empty string
std::string containsEch(const std::string&& result) {
	size_t begin = result.find(" ech=");
	if (begin == std::string::npos) { // Not found
		return "";
	}
	begin++; // Remove the space
	size_t end = result.find(" ", begin);
	// ECH was the last value, no newline at end
	if (end == std::string::npos) {
		// ldns_rr2str puts a new line at the end, so this should not be hit
		// But it doesn't hurt to be careful
		return result.substr(begin);
	}
	return result.substr(begin, end - begin);
}

// Do a check for ECH. The input string is swapped with the ECH return value if it is found.
void doQuery(std::string& domain, ldns_resolver *res) {
	ldns_rdf *dm = ldns_dname_new_frm_str(domain.c_str());
	ldns_pkt *p = ldns_resolver_search(res, dm, LDNS_RR_TYPE_HTTPS, LDNS_RR_CLASS_IN, LDNS_RD);
	ldns_rr_list *https = ldns_pkt_rr_list_by_type(p, LDNS_RR_TYPE_HTTPS, LDNS_SECTION_ANSWER);
	if (https == nullptr) {
		ldns_rdf_deep_free(dm);
		ldns_pkt_free(p);
		throw std::runtime_error("No records were returned");
	}
	domain = "";
	if (https->_rr_count > 0) {
		ldns_rr_list_sort(https);
		for (size_t i = 0; i < ldns_rr_list_rr_count(https); i++) {
			char *temp = ldns_rr2str(ldns_rr_list_rr(https, i));
			std::string ech = containsEch(std::string{temp});
			if (ech.size() != 0) {
				std::swap(domain, ech);
				free(temp); // Free before breaking, because the lower free won't be hit
				break;
			}
			free(temp); // Free the c string
		}
	}
	ldns_rr_list_deep_free(https);
	ldns_rdf_deep_free(dm);
	ldns_pkt_free(p);
}
