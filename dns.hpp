#pragma once
#include <string>
#include <ldns/ldns.h>

void doQuery(std::string& domain, ldns_resolver* res);
