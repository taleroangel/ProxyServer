/**
 * @file Exception.hpp
 * @authors     Ángel David Talero Peñuela
 *              Samir Alejandro Sánchez
 *              Gerardo Hiladgo Carroll
 * @brief Header que contiene las Excepciones personalizadas
 * @copyright Copyright (c) 2021. All rights reserved.
 * 
 * Pontificia Universidad Javeriana
 * Facultad de Ingeniería
 * Departamento de Ingeniería de Sistemas
 * Bogotá D.C - Colombia
 * 
 * Copyright © 2021
 */

#ifndef _EXCEPTION_HPP_
#define _EXCEPTION_HPP_

#include <exception>
#include <string>

/**
 * @class SocketException
 * @brief Excepción de fallo en creación de un Socket
 */
class SocketException : public std::exception
{
    std::string message; /*< Error message */

public:
    /**
     * @brief Constructor de la Excepción de Socket
     * 
     * @param error Mensaje de error (Opcional)
     */
    explicit SocketException(
        const std::string &error = "Error al crear el socket")
        : message(error)
    {
    }

    /**
     * @brief Mostrar el mensaje de error
     * 
     * @return const char* Mensaje de error
     */
    virtual const char *what() const throw()
    {
        return message.c_str();
    }
};

/**
 * @class ConnectionException
 * @brief Excepción al intentar Conectar el socket a determinada dirección
 * IP no existente o un Puerto ocupado
 */
class ConnectionException : public std::exception
{
    std::string message; /*< Error message */

public:
    /**
     * @brief Constructor de la Excepción de Conexión
     * 
     * @param error Mensaje de error (Opcional)
     */
    explicit ConnectionException(
        const std::string &error = "Error al conectar el socket")
        : message(error)
    {
    }

    /**
     * @brief Mostrar el mensaje de error
     * 
     * @return const char* Mensaje de error
     */
    virtual const char *what() const throw()
    {
        return message.c_str();
    }
};

#endif //_EXCEPTION_HPP_