/**
 * @file Connection.hpp
 * @authors     Ángel David Talero Peñuela
 *              Samir Alejandro Sánchez
 *              Gerardo Hiladgo Carroll
 * 
 * @brief Interfaz para Servidor y Cliente, hace uso de la librería de
 * POSIX por lo que esta implementación sólo funcionará en Sistemas Operativos 
 * tipo UNIX como Linux, MacOS o BSD. Para usar en Windows usar WSL o CygWin
 * 
 * @copyright Copyright (c) 2021. All rights reserved.
 * 
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Departamento de Ingeniería de Sistemas
 * Bogotá D.C - Colombia
 * 
 * Copyright © 2021
 */

#ifndef _CONNECTION_HPP_
#define _CONNECTION_HPP_

#include <cstdint>
#include <string>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#define DEFAULT_PORT 8024U      /*< Default connection port*/
#define LISTEN_ANY_IP "0.0.0.0" /*< Listen to every port IP address*/

/**
 * @class Connection
 * @brief Interfax (Clase virtual pura) con los métodos básicos que ambos
 * Cliente y Servidor deben de tener en el proxy
 */
class Connection
{
    /* ---- Typedefs ---- */
public:
    using FileDescriptor = int16_t; /*< Tipo de dato para FileDescriptors de POSIX */
    using Port = uint16_t;          /*< Tipo de datos para puertos*/
    using IPaddress = std::string;  /*< Tipo de datos para direcciónes IP*/

    /* ---- Public Methods ---- */
public:
    /**
     * @brief Crear un Socket para TCP/IPv4
     * 
     * @param ipv4 Dirección IPv4 para conectar el Socket, por defecto
     * 0.0.0.0 (definida en la macro LISTEN_ANY_IP), que es la dirección IP
     * que especifíca que se escuchará a cualquier dirección IP
     * 
     * @param port Número del puerto TCP, por defecto 8024, este puerto no tiene
     * ninguna asignación por lo que está libre (definido en la macro
     * DEFAULT_PORT)
     * 
     * @throw SocketException Puede retornar una excepción de este tipo
     * indicando que falló la creación del Socket, (véase @link SocketException )
     * 
     * @throw ConnectionException Puede retornar una excepción de este tipo
     * indicando que no se pudo conectar a determinada IP o puerto, (véase 
     * @link ConnectionException )
     * 
     * @return FileDescriptor File Descriptor del Socket
     */
    virtual FileDescriptor createSocket(
        IPaddress ipv4 = LISTEN_ANY_IP,
        Port port = DEFAULT_PORT) noexcept(false) = 0;

    /**
     * @brief Cerrar el Socket
     * 
     */
    virtual void closeSocket() = 0;

    /* ---- Operator Overloading ---- */
public:
    /**
     * @brief Sobrecarga del operador ()(int) para llamar a la clase
     * como una función desde un hilo </br>
     * Esta función será la encargada de manipular las conexiones desde un hilo
     * por lo que las operaciones Connect/Accept se dan aquí
     * @param mode Parámetro reservado
     */
    virtual void operator()(int mode) = 0;
};

#endif //_CONNECTION_HPP_