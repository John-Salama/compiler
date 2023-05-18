@echo off

echo Installing live-server...
call npm install -g live-server
echo.

echo Starting server and client...
cd "%~dp0compiler server" && start /B node app.js
echo.

cd "%~dp0.."
cd "%~dp0compiler client" && call live-server