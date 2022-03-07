/**
 * @file Connection.hpp
 * @author Angel D.Talero (angelgotalero@outlook.com)
 * 
 * @brief Interfaz para Servidor y Cliente, hace uso de la librería de
 * POSIX por lo que esta implementación sólo funcionará en Sistemas Operativos 
 * tipo UNIX como Linux, MacOS o BSD. Para usar en Windows usar WSL o CygWin
 * 
 * @copyright Copyright (c) 2021. All rights reserved.
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
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

#include "HTTP.hpp"

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

    /* ---- Destructor ---- */
public:
    // Las interfaces en C++ DEBEN tener un contructor virtual definido
    virtual ~Connection(){};

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
     * @throw AddressException Puede retornar una excepción de este tipo
     * indicando que no se pudo conectar a determinada IP o puerto, (véase 
     * @link AddressException )
     */
    virtual void createSocket(
        IPaddress ipv4 = LISTEN_ANY_IP,
        Port port = DEFAULT_PORT) noexcept(false) = 0;

    /**
     * @brief Cerrar el Socket
     * 
     */
    virtual void closeSocket() noexcept(false) = 0;

    /**
     * @brief Escuchar a posibles conexiones
     * Call from a thread
     * 
     * @param http_class Referencia a la clase HTTP común entre hilos
     * Servidor y cliente
     */
    virtual void listener(HTTP * http_class) = 0;
};

#endif //_CONNECTION_HPP_