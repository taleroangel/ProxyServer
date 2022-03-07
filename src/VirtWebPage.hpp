/**
 * @file Server.hpp
 * @author Angel D. Talero (angelgotalero@outlook.com)
 * @brief Clase encargada de manejar los sitios web virtuales
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