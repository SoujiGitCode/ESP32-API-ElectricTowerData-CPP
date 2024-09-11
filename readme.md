# Proyecto ESP32 con PlatformIO

Este documento proporciona una guía rápida para compilar, subir el firmware, monitorear el puerto serie y cargar archivos en el sistema de archivos del ESP32 usando **PlatformIO**.

## Requisitos previos

- Asegúrate de tener **Visual Studio Code** instalado con la extensión **PlatformIO**.
- Conexión adecuada de tu **ESP32** al equipo.

## Comandos esenciales de PlatformIO

### 1. Abrir la terminal en PlatformIO

Para abrir la terminal desde PlatformIO, sigue estos pasos:

1. En Visual Studio Code, abre **PlatformIO**.
2. Ve a **Miscellaneous** → **New Terminal** para abrir una nueva terminal.

### 2. Compilar el proyecto

Para compilar tu proyecto ESP32, ejecuta el siguiente comando en la terminal de **PlatformIO**:

```bash
pio run

```

Una vez que hayas compilado correctamente tu proyecto, puedes subir el firmware al ESP32 con el siguiente comando:

```bash
pio run --target upload
```

### 4. Abrir el monitor serie

Para visualizar los mensajes que tu ESP32 imprime en la consola serial, usa el siguiente comando:

```bash
pio device monitor

```
Este comando abrirá el monitor serie para mostrar la salida de tu ESP32.

### 5. Cargar archivos en el sistema de archivos del ESP32

Si deseas cargar archivos como **HTML, CSS, JavaScript** u otros archivos estáticos al ESP32 utilizando **SPIFFS** o **LittleFS**, sigue estos pasos:

1. Asegúrate de tener los archivos estáticos en una carpeta llamada `data` dentro de tu proyecto.
2. Ejecuta el siguiente comando para cargar esos archivos al sistema de archivos del ESP32:

```bash
pio run --target uploadfs
