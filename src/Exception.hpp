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
#include <stdexcept>
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
 * @class AddressException
 * @brief Excepción al intentar Conectar el socket a determinada dirección
 * IP no existente o un Puerto ocupado
 */
class AddressException : public std::exception
{
    std::string message; /*< Error message */

public:
    /**
     * @brief Constructor de la Excepción de Conexión
     * 
     * @param error Mensaje de error (Opcional)
     */
    explicit AddressException(
        const std::string &error = "Error de direcciones IPv4")
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
 * @class FileException
 * @brief Excepción al intentar abrir un archivo no existente
 */
class FileException : public std::exception
{
    std::string message; /*< Error message */

public:
    /**
     * @brief Constructor de la Excepción de Conexión
     * 
     * @param error Mensaje de error (Opcional)
     */
    explicit FileException(
        const std::string &error = "Error de archivo")
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