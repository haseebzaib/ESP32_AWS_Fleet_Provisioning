#include <pgmspace.h>


// Amazon Certificate
#define THING_GROUP "esp32dkc"
#define AWS_IOT_SUB_TOPIC "cmd/esp32dkc"
#define AWS_IOT_PUB_TOPIC "device/767397926741/data/sensor"
#define AWS_IOT_ENDPOINT "a1rbcit24wsv6t-ats.iot.us-east-1.amazonaws.com"


#define AWS_SAVE_CERT_SPIFF_LOC "/aws.json"


#define AWS_JSON_ACCEPTED "$aws/certificates/create/json/accepted"
#define AWS_JSON_REJECTED "$aws/certificates/create/json/rejected"
#define AWS_JSON_TEMPLATE_ACCEPTED "$aws/provisioning-templates/test_sensors_provisioning_template/provision/json/accepted"
#define AWS_JSON_TEMPLATE_REJECTED "$aws/provisioning-templates/test_sensors_provisioning_template/provision/json/rejected"
#define AWS_JSON_ "$aws/certificates/create/json"
#define AWS_JSON_REGISTER_THING "$aws/provisioning-templates/test_sensors_provisioning_template/provision/json"


// extern const char AWS_CERT_CA[] asm("_binary_certs_ca_pem_start");
// extern const char AWS_CERT_CRT[] asm("_binary_certs_cert_pem_start");
// extern const char AWS_CERT_PRIVATE[] asm("_binary_certs_private_pem_key_start");


//Amazon Root CA 1
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


// Device Certificate                                               //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDdzCCAl+gAwIBAgIUMzuEb6K6Qaac7Xv20gq+BTnuedUwDQYJKoZIhvcNAQEL
BQAwfjELMAkGA1UEBhMCVVMxEzARBgNVBAgMCldhc2hpbmd0b24xEDAOBgNVBAcM
B1NlYXR0bGUxGDAWBgNVBAoMD0FtYXpvbi5jb20gSW5jLjEgMB4GA1UECwwXQW1h
em9uIElvVCBQcm92aXNpb25pbmcxDDAKBgNVBAUTAzEuMDAeFw0yNDA0MTUwOTE5
MDBaFw0yNDA0MTUwOTI2MDBaMEsxSTBHBgNVBAMMQDIwMTgxZDkzYjlkMWViOTE4
ZmI3NmJmNGNmYjE2MTkwNmM5YzQ2OWE2NDg0MWRkNTc4MzY0NTNiYjhjZjZiY2Qw
ggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDBoTbmPynj4w/ULQ9spq1q
R17tdvGcTGNO3iQ5i9d32UIbQ8GO35VD1vAnKHJcGJumtcQAQYQRE1iW/+cUfPUG
8WltQWoswK6DcC+ot5S0wxBal+q0FFrYIclcwDajnW+ekUi7YkMoaEY9YnMs3OHt
l/cPmUD8A1K4cN/13xBwlYQNjP2Fn77gCOczJD9Fu8ZEFi1P6WaLccphCt3KtcVK
8GNKrDGKGBhI2Sl02fJMHCCErLQzNdnaanhZDdrT81ki8fouH8PzD/HnJ4XMBoYl
2Ekb7Qn+hz6kDGgfpRpJoqaRrYeTlTz05IV1P5YD4t5ZfJ2xakfRGR4iMg8sC0xL
AgMBAAGjIDAeMAwGA1UdEwEB/wQCMAAwDgYDVR0PAQH/BAQDAgeAMA0GCSqGSIb3
DQEBCwUAA4IBAQAb2grUubHuvMxIQUNiDkcEaIEVMvFytpCecMJ8OQFgDKtWeLm3
OqLL8/MVZct1XBHTRjosy4UFQBduJ9f9IqYfC4fzRqVTUtaHLhx55oXO06BxWnL3
wvU14dqF8j9Ugbe6JXkc/qUeyvGUO7R+YH3k3jH0ghFNyOULH+YTSzMLzDC34GkI
RNsq2ZdI2G71RKhD8QQQmcoJa7uai8qzvRXv1s1o6p1uTAZTxDbUiQjNtcBTaCiq
0U9nGMkg13ks3y03VD5RA3QGb7fEhYy9pxQe9i3t9IiWY2kFfgfhBXxIPb2ev6xg
s37jRiBXb8VmVgrwM/yBxRTwI1hI6WdIaxKb
-----END CERTIFICATE-----
)KEY";



// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAwaE25j8p4+MP1C0PbKatakde7XbxnExjTt4kOYvXd9lCG0PB
jt+VQ9bwJyhyXBibprXEAEGEERNYlv/nFHz1BvFpbUFqLMCug3AvqLeUtMMQWpfq
tBRa2CHJXMA2o51vnpFIu2JDKGhGPWJzLNzh7Zf3D5lA/ANSuHDf9d8QcJWEDYz9
hZ++4AjnMyQ/RbvGRBYtT+lmi3HKYQrdyrXFSvBjSqwxihgYSNkpdNnyTBwghKy0
MzXZ2mp4WQ3a0/NZIvH6Lh/D8w/x5yeFzAaGJdhJG+0J/oc+pAxoH6UaSaKmka2H
k5U89OSFdT+WA+LeWXydsWpH0RkeIjIPLAtMSwIDAQABAoIBAByrql1dbO55aJaa
NXZ5F6Rs/Plca1ohVXMQIX0D39u3J7qxxa9KG+f3o4eozYYlfqaos2T2Tn/YxX1W
VvkJp55nkJzE2tvVjAGaUvRAepxpZbqEVBRmDy2BkzdQnsaLQUPRb1uPqTIKfpwI
P9WcvzdXERYWMzEdrNnynLbjWKPSvcFztAKytbRS7VivFn/rKezHM7c2GFxL06m7
H7upsKxwl9i2rYGoqkD3lILtuAImw6nt4lQwxJoLTGiMNSlnwxQf9ouh+r7Oe5fg
eC0Cirbn3VW/XVRHfbhXzOsoWof7yeoVsGPOr3et+hlE2IWzbhM47HsUpnZT62fE
tX3mXUECgYEA6UrrhUXdoMgUDhvsr5b73GxKcJMEULMj9vGmTbSKzM5FeAyu3FUc
nYc2QrsXuNHDE7+BenAP7ACPsic7U2uhrkrrxxRN7DZajKbLQbwuRpTj+x1u0/BC
e2OM1uu/W4bcpli48ezfz/wST1cvagEBCM5qw+/dacA6BFkuVkXaHFcCgYEA1Hn9
+am81Ky6WAu0GbvKastQeQic6yZ98hnzZyzsYudsjAaF7VzrICyWuiDm3oUkxJLm
GdbnpjtcZdsQ2SVJh38X22vnM4WXyR6yXyR0pDKMW5Bp3pYOQoSFVnQgsDCP7Dqn
fLVLms/yCFcS3kdeQJRh2GJWlnFB0zK+/kPLly0CgYEAk0z+0LOAcJhkMVRoaaYH
ePHXPqVVQC8cMjv6KW4phY9oRZicf/otizlohVHkdIndhoaQlmJTa0wfbvleeBmp
iIc9wAGOAet/2Pcnt2W+QkqYeGHpLGCzWX1N3BYWSRAqGc5DWF2cI5KhbP87rM7d
fFX5C6ZUQwVgnIZCv65zPRkCgYBlUYLFUaZEt9Tn9TS9bKn6QNcvz+abzzARiURi
964E/5voLZghdsR8BCcfvF66zzyYLABjY/H0d6v4ufyPv1MPtJAYxpxqGhMmCWg/
LDAeuCfr5bJGPMGyAGVgs2F/ArkuiU2ijwTtXqloRrUtn1SXcOozd7kYZgtgfXtL
3eIwRQKBgFkoLQMFq4q9Tq/XUWwwTQM+snOS31sXH02k45eSQqAeBbm/HU2SswOG
GcS2MVE/AUxrtD/BBO0JPlP3qL9Y536LJDN7iMH2sqvZqRvquWHGzqTvoenh6c6t
Y62LlFVQ2eIU21XxEgbpXFrvJC87ZmSHtkLD/65yi3OgKYTr/j5U
-----END RSA PRIVATE KEY-----
)KEY";