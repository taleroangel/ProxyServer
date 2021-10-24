#include "HTTP.hpp"
#include "Exception.hpp"

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

// Macros
#define _HTTP_HOST_FIND "\r\nHost:"
#define _HTTP_LINE_FEED "\r\n"
#define _HTTP_GET "GET"
#define _HTTP_HEAD "HEAD"
#define _HTTP_CONTENT_LENGTH "\r\nContent-Length:"

HTTP::HTTP()
{
    // Inicializar recursos
    sem_init(&this->request_n, 0, 0);
    sem_init(&this->response_n, 0, 0);
}

HTTP::~HTTP()
{
    sem_destroy(&this->request_n);
    sem_destroy(&this->response_n);
}

std::string HTTP::resolveDNS(std::string url, uint8_t attempt)
{
    struct hostent *hostname;
    struct in_addr **addr_list;

    // Obtener la IP
    hostname = gethostbyname(url.c_str());

    // Verificar que exista
    if (hostname == NULL)
        throw AddressException("Dirección no válida");

    // Guardar lista de ip's
    addr_list =
        (struct in_addr **)hostname->h_addr_list;

    // Direccion IPv4
    std::string ipv4;

    try
    {
        ipv4 = inet_ntoa(*addr_list[attempt]);
    }
    catch (std::out_of_range &e)
    {
        throw e;
    }

    // Retornar direccion ipv4
    std::cout << std::endl
              << "Servicio DNS: " << url << " -> " << ipv4 << std::endl;
    return ipv4;
}

void HTTP::create_head_message(const char *http_msg, char *head_msg)
{
    std::string http(http_msg);
    int inicio = http.find(_HTTP_GET);

    if (inicio == std::string::npos)
    {
        throw AddressException("No content lenght");
    }

    int final = inicio + sizeof(_HTTP_GET) - 1;

    http.replace(inicio, final, _HTTP_HEAD);
    strcpy(head_msg, http.c_str());
}

HTTP::ResponseData HTTP::get_head_size(
    const char head_msg[HTTP::ResponseSize])
{
    std::string data(head_msg);
    int start = data.find(_HTTP_CONTENT_LENGTH) + sizeof(_HTTP_CONTENT_LENGTH);

    if (start == std::string::npos)
    {
        throw AddressException("No content lenght");
    }

    data = data.substr(start, data.size());

    int finish = data.find(_HTTP_LINE_FEED);
    data = data.substr(0, finish);

    return std::stoi(data);
}

void HTTP::get_hostname(
    const char http_data[HTTP::RequestSize],
    char *hostname)
{
    // String con los datos
    std::string data(http_data);
    int start = data.find(_HTTP_HOST_FIND) + sizeof(_HTTP_HOST_FIND);
    data = data.substr(start, data.size());
    int finish = data.find(_HTTP_LINE_FEED);
    data = data.substr(0, finish);

    // Copy into hostname
    strcpy(hostname, data.c_str());
}

void HTTP::pushRequest(Request request)
{
    std::cout << "Se ha empilado una petición (Desbloquea Cliente)" << std::endl;
    this->request_q.push(request);
    sem_post(&this->request_n);
}

void HTTP::popRequest()
{
    this->request_q.pop();
}

HTTP::Request HTTP::getRequest()
{
    std::cout << "Se ha desempilado una petición (Bloquea Cliente)" << std::endl;
    sem_wait(&this->request_n);
    return this->request_q.front();
}

void HTTP::pushResponse(HTTP::Response response)
{
    std::cout << "Se ha empilado una respuesta (Desbloquea Servidor)" << std::endl;
    this->response_q.push(response);
    sem_post(&this->response_n);
}

void HTTP::popResponse()
{
    this->response_q.pop();
}

HTTP::Response HTTP::getResponse()
{
    std::cout << "Se ha desempilado una (Bloquea Servidor)" << std::endl;
    sem_wait(&this->response_n);
    return this->response_q.front();
}