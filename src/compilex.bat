@echo off
setlocal enabledelayedexpansion
set "BASE_DIR=%~dp0"
if exist "%BASE_DIR%compilex_config.bat" (call "%BASE_DIR%compilex_config.bat") else (echo [CompileX] Critical Error: Config missing. & exit /b 1)
if "%~1" == "" goto :no_file
if /I "%~1" == "--version" goto :show_version
if /I "%~1" == "--clean" goto :do_clean
if /I "%~1" == "--log" goto :show_log
set "filename=%~1"
set "extension=%~x1"
set "exe_name=%~n1.exe"
if not exist "!filename!" (
    echo [CompileX] Error: Source file "!filename!" not found.
    exit /b 1
)
if /I "!extension!" == ".py" goto :run_python
if /I "!extension!" == ".cpp" (set "LANG=C++" & set "COMPILER=%GXX_PATH%" & set "FLAGS=-std=c++23 -O3 -s -I"%SYNTAX_INCLUDE%" -o "!exe_name!"")
if /I "!extension!" == ".c" (set "LANG=C" & set "COMPILER=%GCC_PATH%" & set "FLAGS=-O3 -s -I"%SYNTAX_INCLUDE%" -o "!exe_name!"")
if /I "!extension!" == ".f90" (set "LANG=Fortran" & set "COMPILER=%GFORTRAN_PATH%" & set "FLAGS=-O3 -o "!exe_name!"")
if /I "!extension!" == ".cs" (set "LANG=C#" & set "COMPILER=%CSC_PATH%" & set "FLAGS=/nologo /optimize+ /out:"!exe_name!"")
echo [CompileX] Compiling !filename! (!LANG!)...
"!COMPILER!" !FLAGS! "!filename!"
set "comp_status=!errorlevel!"
if !comp_status! equ 0 (
    echo ---------------------------------------------------
    echo [CompileX] Success: Build complete.
    findstr /x "!exe_name!" .compilex_built >nul 2>&1
    if errorlevel 1 echo !exe_name! >> .compilex_built
    echo [%date% %time%] Compiled !exe_name! (!LANG!^) >> compilex_history.log
    echo [CompileX] Executing binary...
    echo ---------------------------------------------------
    if exist "!exe_name!" "!exe_name!"
    exit /b 0
) else (
    echo ---------------------------------------------------
    echo [CompileX] Error: Compilation failed.
    echo [%date% %time%] Failed to compile !filename! (!LANG!^) >> compilex_history.log
    exit /b 1
)
:run_python
echo [CompileX] Executing !filename! (Python)...
echo ---------------------------------------------------
"%PYTHON_PATH%" "!filename!"
echo ---------------------------------------------------
echo [%date% %time%] Executed !filename! (Python) >> compilex_history.log
exit /b 0
:show_version
echo ---------------------------------------------------
echo [CompileX Engine]
echo Version: 2.0.0-LTS "Multi-Language Integration"
echo Developer: hypernova-developer
echo Focus: C++, C, Fortran, C#, Python
echo ---------------------------------------------------
exit /b 0
:do_clean
echo [CompileX] Initializing targeted cleanup...
if exist ".compilex_built" (
    for /f "usebackq tokens=*" %%a in (".compilex_built") do (if exist "%%a" (del /q "%%a" & echo [CompileX] Deleted: %%a))
    del /q .compilex_built
    echo [CompileX] Cleanup finished.
) else (echo [CompileX] No artifacts found.)
exit /b 0
:show_log
echo [CompileX] Operation History:
echo ---------------------------------------------------
if exist "compilex_history.log" (type "compilex_history.log") else (echo [CompileX] Log empty.)
echo ---------------------------------------------------
exit /b 0
:no_file
echo [CompileX] Error: No input file.
exit /b 1