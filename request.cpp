#include "request.hpp"
#include <stdexcept>
#include <curl/curl.h>

bool testConnect(const std::string& url, const std::string& echValue) {
	CURL *curl = curl_easy_init();
	if (!curl) {
		throw std::runtime_error("Unable to initialize curl");
	}
	// TODO: Allow for arbitrary TLS-enabled protocols
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
	// TODO: Does this actually enforce ECH?
	curl_easy_setopt(curl, CURLOPT_ECH, ("ecl:" + echValue).c_str());
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);
	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1);
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	if (res == CURLE_OK) {
		return true;
	} else if (res == CURLE_ECH_REQUIRED) {
		return false;
	}
	const char* curlerror = curl_easy_strerror(res);
	throw std::runtime_error("Curl threw an error: " + std::string{curlerror});
}
