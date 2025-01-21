# MonitoreoSensoresIoT
Este proyecto tiene como objetivo la monitorización de sensores de temperatura y humedad en tiempo real utilizando un ESP32, un puente H para el control de un motor de ventilación y ThingsBoard como plataforma de visualización de datos en un servidor Linux. Los datos se transmiten mediante el protocolo MQTT para una comunicación eficiente y de bajo consumo de ancho de banda.

A través de este sistema, se pueden visualizar las mediciones en un dashboard interactivo y controlar el estado del ventilador dependiendo de la temperatura registrada. 

El presente documento contiene ciertos aspectos necesarios a considerar al momento de querer realizar un proyecto similar.
Para empezar. lo que debemos tomar en cuenta es la distribucción de los pines que serán usados para proporcionar corriente y leer los datos de los sensores.

Adicional a eso debemos tener en cuenta lo necesario para realizar la instalación de la plataforma Thingsboard en la distribución de Linux y la versión con la que contemos, para ello podemos valernos de un tutorial para realizar el proceso correctamente.
A continuación detallaremos ciertos comandos que nos ayudarían a instalar Thingsboard de maanera estandar en Linux:

# Instalación del Servidor ThingsBoard en Linux
Si el sistema operativo está recién instalado, sigue estos pasos para configurar ThingsBoard:
* Actualizaión del sistema: sudo apt update && sudo apt upgrade -y
* Instalar dependencias esenciales: sudo apt install openjdk-11-jdk postgresql postgresql-contrib mosquitto mosquitto-clients -y
* Descargar Thinsboard: wget https://github.com/thingsboard/thingsboard/releases/download/v3.5/thingsboard-3.5.deb
* Instalar Thingsboard: sudo dpkg -i thingsboard-3.5.deb
* Configrar PostgreSQL: sudo -i -u postgres psql
* Dentro de la terminal de PostgreSQL, ejecutar:
  CREATE DATABASE thingsboard;
  CREATE USER tb_user WITH PASSWORD 'password';
  ALTER ROLE tb_user SET client_encoding TO 'utf8';
  ALTER ROLE tb_user SET default_transaction_isolation TO 'read committed';
  ALTER ROLE tb_user SET timezone TO 'UTC';
  GRANT ALL PRIVILEGES ON DATABASE thingsboard TO tb_user;
  \q

* Configurar ThingsBoard para usar PostgreSQL: sudo nano /etc/thingsboard/conf/thingsboard.conf
* Modificar los siguientes valores:
  DATABASE_TS_TYPE=sql
  SPRING_DATASOURCE_URL=jdbc:postgresql://localhost:5432/thingsboard
  SPRING_DATASOURCE_USERNAME=tb_user
  SPRING_DATASOURCE_PASSWORD=password

 * Inicar Thingsboard: sudo service thingsboard start
 * Acceder a la interfaz web:
  Abrir un navegador y acceder a: http://localhost:8080
  Credenciales por defecto:
    Usuario: tenant@thingsboard.org
    Contraseña: tenant


Teniendo en cuenta todo lo que fue mencionado en este apartado y asegurándonos de usar los pines correctos del módulo con el que estemos trabajando, no deberíamos tener problemas al momento de replicar el proyecto mencionado en este apartado.
