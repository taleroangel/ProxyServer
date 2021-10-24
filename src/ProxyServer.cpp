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

#include "Exception.hpp"
#include "Server.hpp"
#include "Client.hpp"

/* -------------------------- Function prototypes -------------------------- */
void mostrarUso();
bool manejarArgumentos(int argc, char const *argv[], Connection::Port &port);

/* --------------------------------- Main --------------------------------- */

int main(int argc, char const *argv[])
{
    Connection::Port puerto;
    if (manejarArgumentos(argc, argv, puerto))
        puerto = DEFAULT_PORT;

    std::cout
        << "Se abrirá una conexión por el puerto: " << puerto << std::endl;

    // Crear la lista de peticiones HTTP
    HTTP *http_class = new HTTP();
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

    // Esperar al hilo
    server_call.join();
    client_call.join();

    // Eliminar el servidor
    delete http_class;
    delete servidor;
    delete cliente;

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

bool manejarArgumentos(
    int argc, char const *argv[],
    Connection::Port &port)
{
    if (argc == 1)
        return true;

    // Sólo recibe un argumento
    else if (argc != 2)
        mostrarUso();

    // Obtener el argumento
    port = std::atoi(argv[1]);
    if (port <= 0)
    {
        std::cerr << "El puerto no es válido!" << std::endl;
        mostrarUso();
    }

    return false;
}