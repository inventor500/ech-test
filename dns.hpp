#pragma once
#include <string>
#include <ldns/ldns.h>

void doQuery(std::string& domain, const ldns_resolver* res);
