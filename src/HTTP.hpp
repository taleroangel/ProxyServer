/**
 * @file HTTP.hpp
 * @author Angel D.Talero (angelgotalero@outlook.com)
 * @brief Clase encargada de compartir peticiones HTTP entre Cliente y Servidor
 * Esta clase funciona con concurrencia/paralelismo
 * 
 * @copyright Copyright (c) 2021. All rights reserved.
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License. 
 */

#ifndef _HTTP_HPP_
#define _HTTP_HPP_

#include <cstdint>
#include <queue>
#include <string>
#include <semaphore.h>
#include <cstring>
#include <memory>

#include "VirtWebPage.hpp"

#define IP_MAX_SIZE 20

#define _HTTP_GET "GET"
#define _HTTP_HEAD "HEAD"
#define _HTTP_POST "POST"
#define _HTTP_PUT "PUT"

/**
 * @class HTTP
 * @brief Clase compartida entre hilos Servidor y Cliente
 * que maneja las peticiones en orden
 */
class HTTP
{
    /* ---- Public variables ---- */
public:
    volatile bool activeListen = true;

    /* ---- Public constants ---- */
public:
    static constexpr uint32_t
        RequestSize = 4096U; /*< Tamaño en bytes máximo de una petición HTTP*/

    static constexpr uint32_t
        ResponseSize = 4096U; /*< Tamaño en bytes máximo de una respuesta HTTP*/

    static constexpr uint16_t
        TCPport = 80U; /*< Puerto TCP por defecto para HTTP*/

    static constexpr uint32_t
        MaxSIZE = 536900000U; /*< Tamaño máximo de HTML permitido (512MB)*/

    /* ---- Public typedefs ---- */
public:
    using DataSize = uint32_t; /*< Tipo de dato que almacena el tamaño de
    una respuesta de forma dinámica*/

    /**
     * @struct Data
     * @brief Estructura de datos entre Cliente - Servidor
     */
    struct Data
    {
        std::string buffer;   /*< Buffer de datos*/
        DataSize buffer_size; /*< Tamaño del buffer*/

        std::string destination; /*< Dirección IP de destino para peticiones*/
        bool ignore;             /*< Flag para ignorar el paquete*/
    };

    /* ---- Private member variables ---- */
private:
    std::queue<Data> request_q; /*< Cola con las peticiones*/
    sem_t sempahore_request;    /*< Semáforo de peticiones*/

    std::queue<Data> response_q; /*< Cola con las respuestas*/
    sem_t semaphore_response;    /*< Semáforo de respuesta*/

    VirtWebPage *virtual_pages; /*< Soporte para Páginas Web virtuales*/

    /* ---- Static methods ---- */
public:
    /**
     * @brief Resolver el nombre de host DNS,
     * Si las páginas web virtuales están disponibles primero verifica en las
     * tables de 'Alias'
     * 
     * @param url Nombre de host DNS
     * @param attempt Cuando hay varias ip's disponibles se puede seleccionar
     * cual usar, por defecto se usa la primera
     * 
     * @throw AddressException La dirección especificada no existe
     * 
     * @return std::string Dirección IPv4
     */
    static std::string resolveDNS(
        std::string url, uint8_t attempt = 0) noexcept(false);

    /**
     * @brief Obtener el Hostname de destino de un paquete GET
     * 
     * @param http_data String con el Request tipo GET
     * @param hostname Apuntador a un string donde guardar el hostname
     */
    static void get_hostname(
        const char http_data[HTTP::RequestSize],
        char *hostname) noexcept(false);

    /**
     * @brief Obtener el parámetro Content-Length:
     * 
     * @param http_data Data recibida
     * @param c_size Tamaño de la data
     * @return uint32_t Tamaño esperado
     */
    static uint32_t get_content_lenght(
        const char *http_data, uint32_t c_size);

    /**
     * @brief Obtener el tamaño del header
     * 
     * @param http_data Data recibida
     * @param c_size  Tamaño de la data
     * @return unsigned int tamaño del header
     */
    static unsigned int get_header_size(
        const char *http_data, uint32_t c_size);

    /* ---- Constructor ---- */
public:
    /**
     * @brief Construir un nuevo objeto de tipo HTTP
     * 
     */
    explicit HTTP();

    /**
     * @brief Destruir el objeto HTTP
     * 
     */
    ~HTTP();

    /* ---- Public methods ---- */
public:
    /**
     * @brief Empujar una petición a la cola
     * Función multihilos, incrementa la cantidad de recursos disponibles
     */
    void pushRequest(Data);

    /**
     * @brief Eliminar una petición de la lista
     */
    void popRequest();

    /**
     * @brief Sacar una petición de la lista para que el cliente pueda leerla
     * Función multihilos, si no hay recursos disponibles el hilo se bloquea
     * 
     * @return Data Petición a sacar de la cola
     */
    Data getRequest();

    /**
     * @brief Empujar una respuesta a la cola
     * Función multihilos, incrementa la cantidad de recursos disponibles
     * @param Data Respuesta a empujar
     */
    void pushResponse(Data);

    /**
     * @brief Eliminar una petición de la lista
     */
    void popResponse();

    /**
     * @brief Sacar una respuesta de la lista para que el servidor pueda leerla
     * Función multihilos, si no hay recursos disponibles el hilo se bloquea
     * 
     * @return Data Respuesta a sacar de la cola
     */
    Data getResponse();

    /**
     * @brief Obtener la clase que maneja los sitios web virtuales
     * 
     * @return VirtWebPage* Clase de sitios web virtuales
     */
    VirtWebPage *getVirtWebPage();

    /**
     * @brief Verificar si se están usando páginas web virtuales
     * 
     * @return true Se están usando Páginas Virtuales
     * @return false No se están usando Páginas Virtuales
     */
    bool isVirtPage();

    /**
     * @brief Añadir un manejador de paginas virtuales
     * 
     * @param virtual_manager Manejador
     */
    void attachVirtWebPage(VirtWebPage *virtual_manager);
};

#endif //_HTTP_HPP_