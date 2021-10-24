/**
 * @file HTTP.hpp
 * @authors     Ángel David Talero Peñuela
 *              Samir Alejandro Sánchez
 *              Gerardo Hiladgo Carroll
 * 
 * @brief Clase encargada de compartir peticiones HTTP entre Cliente y Servidor
 * Esta clase funciona con concurrencia/paralelismo
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

#ifndef _HTTP_HPP_
#define _HTTP_HPP_

#include <cstdint>
#include <queue>
#include <string>
#include <semaphore.h>
#include <cstring>
#include <memory>

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

    /* ---- Public typedefs ---- */
public:
    using DataSize = uint32_t; /*< Tipo de dato que almacena el tamaño de
    una respuesta de forma dinámica*/

    /**
     * @enum RequestType
     * @brief Obtener el tipo de petición HTTP
     */
    enum RequestType
    {
        GET,
        HEAD,
        POST,
        PUT,
        OTHER
    };

    /**
     * @struct Data
     * @brief Estructura de pe
     */
    struct Data
    {
        std::string buffer;   /*< Buffer de datos*/
        DataSize buffer_size; /*< Tamaño del buffer*/

        // Peticiones
        RequestType type;        /*< Tipo de petición*/
        std::string destination; /*< Dirección IP de destino para peticiones*/
        bool ignore;
    };

    /* ---- Private member variables ---- */
private:
    std::queue<Data> request_q;  /*< Cola con las peticiones*/
    std::queue<Data> response_q; /*< Cola con las respuestas*/
    sem_t request_n;             /*< Semáforo con las peticiones disponibles*/
    sem_t response_n;            /*< Semáforo con las respuestas disponibles*/

    /* ---- Static methods ---- */
public:
    /**
     * @brief Resolver el nombre de host DNS
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
     * @brief Crear un mensaje de tipo HEAD para conocer
     * el tamaño del buffer de una petición GET
     * @param http Mensaje GET original
     * @param head Referencia a string de destino
     * @throw AddressException No es una petición GET
     */
    static void createHeadFromGet(
        std::string http,
        char *head) noexcept(false);

    /**
     * @brief Obtener el parámetro 'Content-Length'
     * @param message Mensaje recibido
     * 
     * @throw AddressException No existe parámetro 'Content-Length'
     * 
     * @return DataSize Tamaño de los datos de 'Content-Length' + el tamaño
     * de el mensaje
     */
    static DataSize getContentLength(
        const char *message) noexcept(false);

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
     * @brief Obtener el tipo de petición HTTP
     * 
     * @param http_data Datos HTTP
     * @return RequestType Petición
     */
    static RequestType getType(std::string http_data);

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
};

#endif //_HTTP_HPP_