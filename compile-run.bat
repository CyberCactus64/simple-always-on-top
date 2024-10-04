@echo off

echo Compilazione iniziata...
g++ simple-AlwaysOnTop.cpp -o simple-AOT.exe

:: Verifica se la compilazione è andata a buon fine
if %ERRORLEVEL% neq 0 (
    echo Errore nella compilazione.
    exit /b %ERRORLEVEL%
)

echo Esecuzione codice...
simple-AOT.exe

:: Verifica se l'esecuzione è andata a buon fine
if %ERRORLEVEL% neq 0 (
    echo Errore durante l'esecuzione del programma.
    exit /b %ERRORLEVEL%
)