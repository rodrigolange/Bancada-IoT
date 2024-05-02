#include <pgmspace.h>

#define SECRET

// THINNAME (created on AWS IoT Core) here... It will be the CLIENTID
#define THINGNAME "lange-IoT-SensorPainelSolar"

// SSID and Password of your wifi network
const char WIFI_SSID[] = "Lange-IoT";
const char WIFI_PASSWORD[] = "senha123";

const char AWS_IOT_ENDPOINT[] = "ayskvuv5aj6jn-ats.iot.eu-central-1.amazonaws.com";

// Amazon Root CA 1 - https://www.amazontrust.com/repository/AmazonRootCA1.pem
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

// Device Certificate - Present in kit - name like this: THINGNAME.cert.pem
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUGgDLm6B9FtKnoLNDmVW1CDoFQRkwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTIzMDkwMjIwMzI1
MVoXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAOJBLbGhgxQazqMiCdJk
FbLiN52NaT/5fvhmAm41xsmvOX6VnCZeRcSzt6sH5i4OU0TO8R7/PEq64JdmSr7R
ET65E+nrQwXiTTCirfHspR2okAycq3ELn5BcsiUxhZj+wT6aGUXWvJ7cg0wMkKVY
mALTRr72zA48+lyqscdjoJ1d4EZOfQGBDeySAXDPn07u5m4GzsD41W5IALNEQVoR
yWXHRFsBYs8blD0bJcsuj3chu56XMYbM30FIQ0SaKbUs/srr+01l0qs9MGCqXUv2
xbkLmbzWg32GrMXAEZTgAk9D4ekpZoa+r5GO15GkUsiyIlUehdZRP7GkPL6Jyll8
4ZkCAwEAAaNgMF4wHwYDVR0jBBgwFoAUsl4CoboMOz6V9FbRm4gIZnebS+IwHQYD
VR0OBBYEFA/7NUHax6R0aeTk1RbW2GWdtS4IMAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQBQY3DGg4GLO9uGsd3tSrhWK7ix
wxUNBW+sn4Qa3SIjTloWRt4QUqgW2l9lk/auwEGplI26f55Pu/vqKg04KiSehA8J
1LLUIkuZ79hiP3KQb8s+9sU+JRoEZc3uRkOUb+LjtRtWKd+zZw+usioC77roxFK5
E5C0fDgGhOAsXHgHKjZw6ohTspKSxdqe8ICiwyEs1qWhYAbavreOLYDmb1hdksPo
RxOBLS/9Nhuo3DKseEOBNTq7eSQv9j0sFPk5IqvFaUEfJLSlbBXYGecqRyRFYmAq
APhfb+u11/RiwSIbTdO/A/80U1JYxlWThZ6MgW3jmp9xa/18ZbAU4Ec9pXXl
-----END CERTIFICATE-----
)KEY";

// Device Private Key - Present in kit - name like this: THINGNAME.private.key
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEA4kEtsaGDFBrOoyIJ0mQVsuI3nY1pP/l++GYCbjXGya85fpWc
Jl5FxLO3qwfmLg5TRM7xHv88Srrgl2ZKvtERPrkT6etDBeJNMKKt8eylHaiQDJyr
cQufkFyyJTGFmP7BPpoZRda8ntyDTAyQpViYAtNGvvbMDjz6XKqxx2OgnV3gRk59
AYEN7JIBcM+fTu7mbgbOwPjVbkgAs0RBWhHJZcdEWwFizxuUPRslyy6PdyG7npcx
hszfQUhDRJoptSz+yuv7TWXSqz0wYKpdS/bFuQuZvNaDfYasxcARlOACT0Ph6Slm
hr6vkY7XkaRSyLIiVR6F1lE/saQ8vonKWXzhmQIDAQABAoIBAAY9A9SE/oc8Yfwv
gRPS/KItCJmIf6Fq8KxKq8B8LRn8gdpSp7aLHONajTLjCd61OGiKSC2xnej1lCG4
iVEPlKklEz585SExZy1zHtavzrGtpyMh4rxANMsXMzZBUkywoYVnXFEHpbW/oMMl
CYv1kGNrjeTQCzpNRh3t+u7jeVc9ZRnULSMCK09ZIgKeS1pjnftEtAPcWv6mtFhT
U6NeYNfx/rODyGrQ6Sg9rXhZUeQ5jRmgMeVL/rjGWWBSbzx76uVHX3D6dfrRWbFA
oFu0H4zD4VXBLYNTCbJMBPhtlEP11oZ17LWW46Qoe8PmJA8mwFYTV1nLlQ8FX2MY
bVsIvDECgYEA/nebO0mj7hyG3TCgQfh+CPQNqa88fAt7OQPGwvKgqbmlBXY5CT3K
3Nm1ifYmeB9OhQbqsxZudlHkfNI6cfNbTlYpgdjzX+GDcWogumKqJriFvyjfb0Im
Vs8Gsn7FQzWg731wuWH67z0njhoHyrZXM6plGH9PuHrcwguHYYeyvj0CgYEA454R
VNFSPcYQVSmduwb3rwZFv7HFlAlyrXjaAqBkM5oqFnH11gMH5q9bKn3KtqVl8Exn
kVps25RtPun4pMjhgK9foKXneYx6EDJrX7ZfAKEd/p0+KPyJs2tcHSVTvePtrjwO
XQbJhvI1JW21MDGw/9Vpv1kj+ZlSQmOzJrw+Io0CgYAk+9M24bQOply/hxKzo98F
hBr8P3jF+pE1LyRTLxpEsLOYQJ93hWCXipjE7V6X9MzfrqqrzqXNUoen0JIn/5uU
nhc4RQTSOZ91+1u4WyCpsgXWS6V1sSKLWVl61cvrwb6bMBsTBneHD9dQ4t2hC7FO
1AQPdGlswYaWOUC63xMObQKBgQCfxg3eXsx+5jACx6KbbQmhhyKgq7XH81XXJPTL
UTDk8/tFJBtvbVM3f7Hbws7vpGxI4Cj24+VzwN1qgyvw57066l+S0cIJFS19fxbV
6ZsIQxBd8Jx4gsRZ+CuBnfkJWxw06J0geaoumofpzPeZa9gVzQVctsLE6vXN+lQA
BbUL5QKBgDM4/XzDRvr5Ri0Qp4qsRJaATlDwRaQUnmzSAyismZhq/9w1TjrH0Muj
aTmQASEnJnQOZuRy/TmlKQpT/8HOBLjwRUMGzfmQYQGl5cMaFnVcErjrjF0E/z1+
zlmd7vq9bHxX+6V1816Mcb61Kt/2ls6mh5kOlc4NT3oUmT5XxV8p
-----END RSA PRIVATE KEY-----
)KEY";