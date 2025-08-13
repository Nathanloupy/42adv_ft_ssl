# ft_ssl

This project aims to recode part of the OpenSSL program, taking its version 1.1.1 as a reference. It is split into three different assignments: `ft_ssl_md5`, `ft_ssl_des` and`ft_ssl_rsa`, implemeting the respective following `digest`, `cipher` and `standard` commands.

### Usage

```
$> make
$> ./bin/ft_ssl help
Standard commands
genrsa

Message Digest commands
md5
sha256
whirlpool

Cipher commands
base64
des
des-ecb
des-cbc
des3
des3-ecb
des3-cbc
```

### Examples

```bash
# Compute an MD5 digest from a string
$> ./bin/ft_ssl md5 -s "hello"

# Base64 encode/decode a file
$> echo -n "hello world" > msg.txt
$> ./bin/ft_ssl base64 -e -i msg.txt -o msg.b64
$> ./bin/ft_ssl base64 -d -i msg.b64

# DES-CBC encrypt/decrypt with passphrase and IV (Base64 output)
$> echo -n "secret data" > plain.txt
$> ./bin/ft_ssl des-cbc -e -a -i plain.txt -o cipher.b64 -p "loupy" -v 0001020304050607 -s 0011223344556677
$> ./bin/ft_ssl des-cbc -d -a -i cipher.b64 -o decrypted.txt -p "loupy" -v 0001020304050607

# Generate an RSA private key to a file
$> ./bin/ft_ssl genrsa -o private.pem
```

## Grading

This project is part of my curriculum at 42 School.

### **Project:** `ft_ssl_md5`
- **Date of completion:** 2025-04-15
- **Grade:** 125/100

### **Project:** `ft_ssl_des`
- **Date of completion:** 2025-06-13
- **Grade:** 110/100

### **Project:** `ft_ssl_rsa`
- **Date of completion:** Ongoing
- **Grade:** N/A