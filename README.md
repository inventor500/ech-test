# ech-test

This is a simple program that checks for ECH support on the provided domains.

After confirming that a domain has the proper ECH information in its DNS record, it will attempt to connect to the server with ECH. Currently, only HTTPS is supported by this program (though other TLS-wrapped connections can support ECH).

## Usage

Example:
```shell
$ ech-test example.org crypto.cloudflare.com

# example.org            false
# crypto.cloudflare.com  true
```

Output:

| Value | Meaning                                                     |
|-------|-------------------------------------------------------------|
| false | ECH is not supported                                        |
| true  | ECH is supported                                            |
| null  | Unable to complete test (usually failure in DNS resolution) |


## Compilation

Requires the following libraries to be available:

| Name | Purpose        |
|------|----------------|
| ldns | DNS resolution |
| curl | HTTPS requests |

This program uses CMake for compilation.

