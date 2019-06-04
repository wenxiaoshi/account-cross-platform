//
// Created by dw on 19-2-28.
//

#include "network_utils.h"

#include <string>

// TokenAuthenticator 用来支持令牌认证
// https://grpc.io/docs/guides/auth.html
class TokenAuthenticator : public grpc::MetadataCredentialsPlugin {
public:
    TokenAuthenticator(const std::string& token) : token_(token) {}

    grpc::Status GetMetadata(grpc::string_ref service_url, grpc::string_ref method_name, const grpc::AuthContext& channel_auth_context, std::multimap<grpc::string, grpc::string>* metadata) override {
        metadata->insert(std::make_pair("authorization-token", token_));
        return grpc::Status::OK;
    }

private:
    std::string token_;
};

std::shared_ptr<grpc::Channel> utils::NetworkUtils::channel = NULL;

std::shared_ptr<grpc::Channel> utils::NetworkUtils::getNetworkChannel(){

    if (channel != NULL){
        return channel;
    }

    const std::string MY_PEM = "-----BEGIN CERTIFICATE-----\n"
                               "MIIC7DCCAdQCCQCxgpkVF5hELDANBgkqhkiG9w0BAQUFADA4MRUwEwYDVQQDDAx3\n"
                               "ZWNoYXQtbG9naW4xHzAdBgkqhkiG9w0BCQEWEDM3NTU5MTU4MEBxcS5jb20wHhcN\n"
                               "MTkwMjA1MTUxMTU1WhcNMjkwMjAyMTUxMTU1WjA4MRUwEwYDVQQDDAx3ZWNoYXQt\n"
                               "bG9naW4xHzAdBgkqhkiG9w0BCQEWEDM3NTU5MTU4MEBxcS5jb20wggEiMA0GCSqG\n"
                               "SIb3DQEBAQUAA4IBDwAwggEKAoIBAQDPGB6XpC+l+p8UdbuXwWBWTzxB3yh6Dsnd\n"
                               "yi/9eBurocCy0w1vjZX6GlDCBohS9ZP+mTSz0TNyB6JB+rqqjDhAXqB45K4arcPs\n"
                               "bKHmgJmXl8Iiar+AHPNlRWvwNCwq1NlEz6sMYmfZ9EJLOnS6sVmjC3oU3putj+4u\n"
                               "UHDk1XXkowyIBJwRjuRfHc5vkOnEPZ08kPh06vpTa/SxomVITB1d+W52vrKrtf2j\n"
                               "vOSadIHpsMREfvdH7L8zzxHtBr6sYK+iLfJhwD54+Pa8epJRNY3KZeG8EDIn0xYr\n"
                               "TwA+hJXm68z27whJCeirN14bnDPXFon+eq1wTacpNzdzMhIZyefnAgMBAAEwDQYJ\n"
                               "KoZIhvcNAQEFBQADggEBAMErKxZZakhkpFUkPVUxSv3pQV4Sz0rwmFXIOmO5VPlt\n"
                               "iyT65IRk3FEGDrhMEYQCJf2sSfrJeGT7a8AkF71vBI+ESk4wackwMlAlHY4/Daij\n"
                               "RM5kIF2uJz1Lq1ALIEeaMwQF72uTeLSEvjWqH0/vXRgPHquh0ZdUgfP0vOxh/Zt+\n"
                               "9sgcwueRcNGsNKAW1yFphVaM3n3OQu7MYMp7TY1CngMZR7bSpFG2qxRCh/WIhZlB\n"
                               "Q4b98lu1RVF2fBFLbnITu7B5JC0rhyBHL7+sBwh7NSVRNFkTaauD54xCJ5UOgWZ2\n"
                               "ELjzue/xsOF2R/LmH1V85YGTU/vS2dr6Mon3bDMu/js=\n"
                               "-----END CERTIFICATE-----";

    const std::string TOKEN = "wechat-login";

    const std::string host("47.92.146.40");
    const int port = 50051;

    const int host_port_buf_size = 1024;
    char host_port[host_port_buf_size];
    snprintf(host_port, host_port_buf_size, "%s:%d", host.c_str(), port);


    grpc::SslCredentialsOptions ssl_options;
    ssl_options.pem_root_certs = MY_PEM;

    // Create a default SSL ChannelCredentials object.
    auto channel_creds = grpc::SslCredentials(ssl_options);

    grpc::ChannelArguments cargs;
    cargs.SetSslTargetNameOverride(TOKEN); // common name

    auto call_creds = grpc::MetadataCredentialsFromPlugin(std::unique_ptr<grpc::MetadataCredentialsPlugin>(new TokenAuthenticator(TOKEN)));
    auto compsited_creds = grpc::CompositeChannelCredentials(channel_creds, call_creds);

    // Create a channel using the credentials created in the previous step.
    channel = grpc::CreateCustomChannel(host_port, compsited_creds, cargs);

    return channel;
}
