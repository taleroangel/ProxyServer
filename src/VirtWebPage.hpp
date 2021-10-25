/**
 * @file Server.hpp
 * @authors     Ángel David Talero Peñuela
 *              Samir Alejandro Sánchez
 *              Gerardo Hiladgo Carroll
 * 
 * @brief Clase encargada de manejar los sitios web virtuales
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

#ifndef _VIRTWEBPAGE_HPP_
#define _VIRTWEBPAGE_HPP_

#include <string>
#include <vector>

class VirtWebPage
{
    /* ---- Typedefs privados ---- */
private:
    /**
     * @struct entry
     * @brief Estructura de entradas
     */
    struct entry
    {
        std::string input;  /*< URL enviada por el navegador*/
        std::string output; /*< URL equivalente*/
    };

    /* ---- Variables privadas ---- */
private:
    std::vector<entry> entries; /*< Todas las entradas*/

    /* ---- Constructores ---- */
public:
    /**
     * @brief Crear entradas de página web virtuales
     * 
     * @param filename Nombre de archivo a leer
     */
    explicit VirtWebPage(std::string filename) noexcept(false);

    /**
     * @brief Destruir las entradas de páginas web virtuales
     * 
     */
    ~VirtWebPage();

    /* ---- Métodos públicos ---- */
public:
    /**
     * @brief Obtener la pagina web virtual
     * 
     * @param url URL que mandó el navegador
     * @throw AddressException La dirección que se solicitó no existe
     * 
     * @return std::string URL real
     */
    std::string getVirtualPage(std::string url) noexcept(false);
};

#endif //_VIRTWEBPAGE_HPP_