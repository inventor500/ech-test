# ech-test

This is a simple program that checks for ECH support on the provided domains.

## Usage

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
