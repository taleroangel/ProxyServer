/**
 * @file ProxyServer.cpp
 * @authors     Ángel David Talero Peñuela
 *              Samir Alejandro Sánchez
 *              Gerardo Hiladgo Carroll
 * @brief Servidor Proxy - Proyecto final Comunicaciones y Redes
 * @copyright Copyright (c) 2021. All rights reserved.
 * 
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Departamento de Ingeniería de Sistemas
 * Bogotá D.C - Colombia
 * 
 * Copyright © 2021
 */

/* ------------------------------- Libraries ------------------------------- */

#include <iostream>
#include <cstdlib>
#include <thread>
#include <cstring>
#include <string>

#include "Exception.hpp"
#include "Server.hpp"
#include "Client.hpp"
#include "signal.h"

/* -------------------------- Function prototypes -------------------------- */
void mostrarUso();
std::string manejarArgumentos(
    int argc,
    char const *argv[],
    Connection::Port &port,
    bool &archivo);
void interruptHandler(int opt);

/* --------------------------- Global variables --------------------------- */
volatile bool *_main_http_handler;
volatile bool _main_detach = false;

/* --------------------------------- Main --------------------------------- */

int main(int argc, char const *argv[])
{
    Connection::Port puerto;
    bool archivovirtuales = false;

    std::string nombre_archivo =
        manejarArgumentos(argc, argv, puerto, archivovirtuales);

    std::cout
        << "Bienvenido al ServidorProxy" << std::endl;
    std::cout
        << "Se abrirá una conexión por el puerto: " << puerto << std::endl
        << std::endl;

    // Crear la lista de peticiones HTTP
    HTTP *http_class = new HTTP();

    // Crear páginas web virtuales
    VirtWebPage *paginasVirtuales;
    bool initialized = false;

    if (archivovirtuales)
    {
        try
        {
            paginasVirtuales = new VirtWebPage(nombre_archivo);
            http_class->attachVirtWebPage(paginasVirtuales);
            initialized = true;
        }
        catch (FileException &e)
        {
            perror(e.what());
        }
    }

    // Crear los Servidores y Clientes
    Server *servidor;
    Client *cliente;

    try // Maneja la propagación de excepciones
    {
        // Crear un servidor
        servidor = new Server(puerto);
        cliente = new Client(HTTP::TCPport);
    }
    catch (SocketException &e)
    {
        perror(e.what());

        delete http_class;
        return EXIT_FAILURE;
    }

    // Llamar al servidor desde un hilo
    std::cout
        << "El servidor está listo para recibir peticiones..." << std::endl;
    std::thread server_call(&Server::listener, servidor, http_class);

    std::cout
        << "El servidor está listo para enviar peticiones..." << std::endl;
    std::thread client_call(&Client::listener, cliente, http_class);

    // Activar Interrupciones
    _main_http_handler = (volatile bool *)&http_class->activeListen;
    signal(SIGINT, interruptHandler);

    server_call.detach();
    client_call.detach();

    while (!_main_detach)
    {
        // No hacer nada mientras estén los hilos
    }

    std::cout << "\nCerrando el Servidor Proxy..." << std::endl;

    // Eliminar el servidor
    delete http_class;
    delete servidor;
    delete cliente;

    if (initialized)
        delete paginasVirtuales;

    // Salir
    return EXIT_SUCCESS;
}

/* -------------------------- Function definition -------------------------- */
void mostrarUso()
{
    std::cerr << "Uso: ./ProxyServer [puerto (opcional)]" << std::endl
              << "El puerto por defecto es: " << DEFAULT_PORT << std::endl;
    exit(1);
}

std::string manejarArgumentos(
    int argc, char const *argv[],
    Connection::Port &port, bool &archivo)
{
    archivo = false;
    char buffer[16] = "null";

    switch (argc)
    {
    // Archivo
    case 3:
        strcpy(buffer, argv[2]);
        archivo = true;

    // Sólo PUERTO
    case 2:

        try
        {
            port = std::atoi(argv[1]);
            if (port <= 0)
            {
                throw std::exception();
            }
        }

        catch (std::exception &e)
        {
            mostrarUso();
        }

        break;

    case 1: // Sin argumentos
        port = DEFAULT_PORT;
        break;

    default:
        mostrarUso();
        break;
    }

    std::string nombre_archivo(buffer);
    return nombre_archivo;
}

void interruptHandler(int opt)
{
    *_main_http_handler = false;
    _main_detach = true;
}