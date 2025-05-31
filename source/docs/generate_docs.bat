@echo off
REM =============================================================================
REM Movie Ticket Booking System - Documentation Generator
REM Object-Oriented Programming Project
REM =============================================================================

echo.
echo ===============================================
echo  Movie Ticket Booking System Documentation
echo  Generating API Documentation with Doxygen
echo ===============================================
echo.

REM Check if doxygen is installed
where doxygen >nul 2>nul
if %errorlevel% neq 0 (
    echo [ERROR] Doxygen is not installed or not in PATH
    echo.
    echo Please install Doxygen from: https://www.doxygen.nl/download.html
    echo.
    echo Installation instructions:
    echo 1. Download Doxygen installer from the official website
    echo 2. Run the installer and follow the setup wizard
    echo 3. Make sure to add Doxygen to your PATH during installation
    echo 4. Restart your command prompt and try again
    echo.
    pause
    exit /b 1
)

echo [INFO] Doxygen found - generating documentation...
echo.

REM Change to docs directory
cd /d "%~dp0"

REM Clean previous output
if exist "output" (
    echo [INFO] Cleaning previous documentation...
    rmdir /s /q "output"
)

REM Generate documentation
echo [INFO] Running Doxygen...
doxygen Doxyfile

REM Check if generation was successful
if %errorlevel% equ 0 (
    echo.
    echo [SUCCESS] Documentation generated successfully!
    echo.
    echo Documentation location: %~dp0output\html\index.html
    echo.
    
    REM Ask user if they want to open the documentation
    set /p "open_docs=Do you want to open the documentation now? (Y/N): "
    if /i "%open_docs%"=="Y" (
        echo [INFO] Opening documentation in default browser...
        start "" "output\html\index.html"
    )
    
    echo.
    echo [INFO] Documentation structure:
    echo   - HTML Documentation: output\html\index.html
    echo   - Class Diagrams: Available in the HTML output
    echo   - File Structure: Browse files section in documentation
    echo   - API Reference: Classes and namespaces sections
    echo.
    echo [TIP] You can also browse the documentation by opening:
    echo       %~dp0output\html\index.html
    
) else (
    echo.
    echo [ERROR] Documentation generation failed!
    echo.
    echo Common issues:
    echo 1. Make sure all source files are accessible
    echo 2. Check that Doxygen configuration is correct
    echo 3. Verify that input paths in Doxyfile are valid
    echo.
    echo For troubleshooting, check the Doxygen output above for specific errors.
)

echo.
echo Press any key to exit...
pause >nul
