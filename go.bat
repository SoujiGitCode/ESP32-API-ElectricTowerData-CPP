@echo off

REM Comprobar si se pasó un argumento.
if "%1"=="" goto auto_run
if "%1"=="-o" goto menu
if "%1"=="-u" goto uploadfs

:menu
echo =====================================
echo Plataforma ESP32 - Atajos PlatformIO
echo =====================================
echo 1. Compilar, subir y abrir monitor serie
echo 2. Subir firmware al ESP32
echo 3. Abrir monitor serie
echo 4. Cargar archivos al sistema de archivos (SPIFFS/LittleFS)
echo 5. Salir
echo =====================================
set /p choice="Elige una opción: "

if "%choice%"=="1" goto compile_upload_monitor
if "%choice%"=="2" goto upload
if "%choice%"=="3" goto monitor
if "%choice%"=="4" goto uploadfs
if "%choice%"=="5" goto end
goto menu

:auto_run
echo Ejecutando compilación, subida y monitor serie...
goto compile_upload_monitor

:compile_upload_monitor
echo Compilando el proyecto...
pio run

echo Subiendo firmware al ESP32...
pio run --target upload

echo Abriendo el monitor serie...
pio device monitor
goto end

:upload
echo Subiendo firmware al ESP32...
pio run --target upload
goto menu

:monitor
echo Abriendo el monitor serie...
pio device monitor
goto menu

:uploadfs
echo Cargando archivos al sistema de archivos (SPIFFS/LittleFS)...
pio run --target uploadfs
goto end

:end
exit
