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

/**
 * @class HTTP
 * @brief Clase compartida entre hilos Servidor y Cliente
 * que maneja las peticiones en orden
 */
class HTTP
{
    /* ---- Public constants ---- */
public:
    static constexpr uint16_t
        RequestSize = 512U; /*< Tamaño en bytes máximo de una petición HTTP*/

    static constexpr uint16_t
        ResponseSize = 1024U; /*< Tamaño en bytes máximo de una respuesta HTTP*/

    static constexpr uint16_t
        TCPport = 80U; /*< Puerto TCP por defecto para HTTP*/

    /* ---- Public typedefs ---- */
public:
    using ResponseData = uint32_t; /*< Tipo de dato que almacena el tamaño de una respuesta de forma dinámica*/

    /**
     * @struct Request
     * @brief Petición de Cliente -> Servidor
     * Ya debe venir tokenizada
     */
    struct Request
    {
        char request[RequestSize]; /*< RAW data, petición HTTP sin modificar*/
        std::string IPv4_Dest;     /*< Dirección IPv4 de destino*/
        std::string URLname;       /*< URL para el DNS*/

        /**
         * @brief Construir una petición HTML
         * 
         * @param HTMLresquest RAW Html request
         * @param IPv4_destination IPV4 de destino
         * @param url_name URL para el DNS
         */
        explicit Request(
            const char HTMLresquest[RequestSize],
            std::string IPv4_destination,
            std::string url_name)
            : IPv4_Dest(IPv4_destination), URLname(url_name)
        {
            strcpy(request, HTMLresquest);
        }
    };

    /**
     * @struct Response
     * @brief Estructura de respuesta
     */
    struct Response
    {
        std::string response; /*< Buffer de respuesta GET*/
        uint32_t response_size;   /*< Tamaño del buffer GET*/
    };

    /* ---- Private member variables ---- */
private:
    std::queue<Request> request_q;   /*< Cola con las peticiones*/
    std::queue<Response> response_q; /*< Cola con las respuestas*/
    sem_t request_n;                 /*< Semáforo con las peticiones disponibles*/
    sem_t response_n;                /*< Semáforo con las respuestas disponibles*/

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
     * @param http_msg Mensaje GET original
     * @param head_msg Referencia a string de destino
     * @throw AddressException No es una petición GET
     */
    static void create_head_message(
        const char *http_msg,
        char *head_msg) noexcept(false);

    /**
     * @brief Obtener el parámetro 'Content-Length' antes de realizar una
     * petición GET
     * 
     * @param head_msg Mensaje HEAD recibido
     * 
     * @throw AddressException No existe parámetro de largo de contenido
     * 
     * @return ResponseData Tamaño de los datos de 'Content-Length' + el tamaño
     * de el mensaje de HEAD
     */
    static ResponseData get_head_size(
        const char head_msg[ResponseSize]) noexcept(false);

    /**
     * @brief Obtener el hostname de un paquete HTTP
     * 
     * @param http_data string tipo C con el request HTTP RAW
     * @param hostname Apuntador a un string donde guardar el hostname
     */
    static void get_hostname(
        const char http_data[HTTP::RequestSize],
        char *hostname);

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
    void pushRequest(Request);

    /**
     * @brief Eliminar una petición de la lista
     */
    void popRequest();

    /**
     * @brief Sacar una petición de la lista para que el cliente pueda leerla
     * Función multihilos, si no hay recursos disponibles el hilo se bloquea
     * 
     * @return Request Petición a sacar de la cola
     */
    Request getRequest();

    /**
     * @brief Empujar una respuesta a la cola
     * Función multihilos, incrementa la cantidad de recursos disponibles
     */
    void pushResponse(Response);

    /**
     * @brief Eliminar una petición de la lista
     */
    void popResponse();

    /**
     * @brief Sacar una respuesta de la lista para que el servidor pueda leerla
     * Función multihilos, si no hay recursos disponibles el hilo se bloquea
     * 
     * @return Response Respuesta a sacar de la cola
     */
    Response getResponse();
};

#endif //_HTTP_HPP_