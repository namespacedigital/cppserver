#!/bin/bash

# Certificate Authority (CA)
openssl genrsa -passout pass:qwerty -out  out/ca-secret.key 4096
openssl rsa -passin pass:qwerty -in  out/ca-secret.key -out  out/ca.key
openssl req -new -x509 -days 3650 -subj '/C=RO/ST=Romania/L=Iasi/O=Example root CA/OU=Example CA unit/CN=namespace.digital' -key  out/ca.key -out out/ca.crt
openssl pkcs12 -export -passout pass:qwerty -inkey  out/ca.key -in  out/ca.crt -out  out/ca.pfx
openssl pkcs12 -passin pass:qwerty -passout pass:qwerty -in  out/ca.pfx -out  out/ca.pem

# SSL Server certificate
openssl genrsa -passout pass:qwerty -out  out/server-secret.key 4096
openssl rsa -passin pass:qwerty -in  out/server-secret.key -out  out/server.key
openssl req -new -subj '/C=RO/ST=Romania/L=Iasi/O=Example server/OU=Example server unit/CN=cppserver.namespace.digital' -key  out/server.key -out  out/server.csr
openssl x509 -req -days 3650 -in  out/server.csr -CA  out/ca.crt -CAkey  out/ca.key -set_serial 01 -out  out/server.crt
openssl pkcs12 -export -passout pass:qwerty -inkey  out/server.key -in  out/server.crt -out  out/server.pfx
openssl pkcs12 -passin pass:qwerty -passout pass:qwerty -in  out/server.pfx -out  out/server.pem

# SSL Client certificate
openssl genrsa -passout pass:qwerty -out  out/client-secret.key 4096
openssl rsa -passin pass:qwerty -in  out/client-secret.key -out out/client.key
openssl req -new -subj '/C=RO/ST=Romania/L=Iasi/O=Example client/OU=Example client unit/CN=cppserver.namespace.digital' -key  out/client.key -out  out/client.csr
openssl x509 -req -days 3650 -in  out/client.csr -CA  out/ca.crt -CAkey  out/ca.key -set_serial 01 -out  out/client.crt
openssl pkcs12 -export -passout pass:qwerty -inkey  out/client.key -in  out/client.crt -out  out/client.pfx
openssl pkcs12 -passin pass:qwerty -passout pass:qwerty -in  out/client.pfx -out out/client.pem

# Diffie-Hellman (D-H) key exchange
openssl dhparam -out  out/dh4096.pem 4096