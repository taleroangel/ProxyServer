#include "HTTP.hpp"
#include "Exception.hpp"

#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

// Macros
#define _HTTP_LINE_FEED "\r\n"
#define _HTTP_HOST_FIND "\r\nHost:"
#define _HTTP_CONTENT_LENGTH "\r\nContent-Length:"

HTTP::HTTP()
    : activeListen(true), virtual_pages(nullptr)
{
    sem_init(&this->sempahore_request, false, 0);
    sem_init(&this->semaphore_response, false, 0);
}

HTTP::~HTTP()
{
    this->activeListen = false;
    sem_destroy(&this->sempahore_request);
    sem_destroy(&this->semaphore_response);
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
              << "Servicio DNS: " << url << " -> " << ipv4 << std::endl
              << std::endl;

    return ipv4;
}

void HTTP::get_hostname(
    const char http_data[HTTP::RequestSize],
    char *hostname)
{
    // String con los datos
    std::string data(http_data);
    unsigned int start = data.find(_HTTP_HOST_FIND) + sizeof(_HTTP_HOST_FIND);

    if (start == std::string::npos)
        throw std::out_of_range("Invalid HOST");

    data = data.substr(start, data.size());
    unsigned int finish = data.find(_HTTP_LINE_FEED);
    data = data.substr(0, finish);

    // Copy into hostname
    strcpy(hostname, data.c_str());
}

void HTTP::pushRequest(Data request)
{
    std::cout << "Se ha empilado una petición (Desbloquea Cliente)" << std::endl;
    this->request_q.push(request);
    sem_post(&this->sempahore_request);
}

void HTTP::popRequest()
{
    this->request_q.pop();
}

HTTP::Data HTTP::getRequest()
{
    std::cout << "Se ha desempilado una petición (Bloquea Cliente)" << std::endl;
    sem_wait(&this->sempahore_request);
    return this->request_q.front();
}

void HTTP::pushResponse(HTTP::Data response)
{
    std::cout << "Se ha empilado una respuesta (Desbloquea Servidor)" << std::endl;
    this->response_q.push(response);
    sem_post(&this->semaphore_response);
}

void HTTP::popResponse()
{
    this->response_q.pop();
}

HTTP::Data HTTP::getResponse()
{
    std::cout << "Se ha desempilado una respuesta (Bloquea Servidor)" << std::endl;
    sem_wait(&this->semaphore_response);
    return this->response_q.front();
}

bool HTTP::isVirtPage()
{
    if (this->virtual_pages == nullptr)
        return false;

    return true;
}

VirtWebPage *HTTP::getVirtWebPage()
{
    return this->virtual_pages;
}

void HTTP::attachVirtWebPage(VirtWebPage *virtual_manager)
{
    this->virtual_pages = virtual_manager;
}