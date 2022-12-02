# ProxyServer
Proyecto 'Comunicaciones y Redes' 2021-01\
Proxy HTTP asíncrono implementado en C++ con el uso de hilos __std::thread__, llamadas al sistema  de UNIX y Sockets de bajo nivel.

![Intro](/docs/intro.png)

## Funcionamiento del Proxy
Las peticiones se escuchan por un puerto específicado (8024 por defecto), las peticiones entrantes son atendidas por un hilo de la clase __Server__, las peticiones luego son encoladas y leídas por un hilo de la clase __HTTP__, se resuelven las queries DNS y se envía la petición a la clase __Client__ quien finalmente envía la petición al Servidor.

![Server](/docs/server.png)

## Documento de Diseño
Los detalles de implementación y uso se encuentran en el siguiente artículo:\
[Artículo de Diseño (PDF)](/docs/project_article_spanish.pdf)