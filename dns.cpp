#include "dns.hpp"
#include <stdexcept>


// Returns just the ech section if it exists, otherwise returns an empty string
std::string containsEch(const std::string&& result) {
	size_t begin = result.find(" ech=");
	if (begin == std::string::npos) { // Not found
		return "";
	}
	begin+=5; // Remove the space and "ech="
	if (begin >= (result.size() - 1)) { // Not set to any value
		return "";
	}
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
void doQuery(std::string& domain, const ldns_resolver *res) {
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
