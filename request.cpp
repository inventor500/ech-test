/*
     ech-test - Check if ECH is supported for a domain.
     Copyright (C) 2024 Isaac Ganoung

     This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

     This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.
*/


#include "request.hpp"
#include <stdexcept>
#include <curl/curl.h>

void initCurl() {
	curl_global_init(CURL_GLOBAL_DEFAULT);
}

void destructCurl() {
	curl_global_cleanup();
}

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
