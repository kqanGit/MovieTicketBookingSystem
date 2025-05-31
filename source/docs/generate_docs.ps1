# Movie Ticket Booking System - Documentation Generator
# PowerShell script for generating API documentation with Doxygen
#
# RECOMMENDED METHOD: Use this PowerShell script instead of .bat files
# This script provides cross-platform compatibility and better error handling
#
# Usage Examples:
#   .\generate_docs.ps1                    # Generate documentation
#   .\generate_docs.ps1 -InstallDoxygen    # Install Doxygen and generate docs
#   .\generate_docs.ps1 -OpenDocs          # Generate and open documentation
#   .\generate_docs.ps1 -Clean             # Clean previous output

param(
    [switch]$InstallDoxygen,
    [switch]$OpenDocs,
    [switch]$Clean
)

Write-Host ""
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host " Movie Ticket Booking System Documentation" -ForegroundColor White
Write-Host " Generating API Documentation with Doxygen" -ForegroundColor White
Write-Host "===============================================" -ForegroundColor Cyan
Write-Host ""

# Function to check if Doxygen is installed
function Test-DoxygenInstalled {
    try {
        $null = Get-Command doxygen -ErrorAction Stop
        return $true
    }
    catch {
        return $false
    }
}

# Function to install Doxygen via Chocolatey
function Install-DoxygenChocolatey {
    Write-Host "[INFO] Attempting to install Doxygen via Chocolatey..." -ForegroundColor Yellow
    
    # Check if Chocolatey is installed
    try {
        $null = Get-Command choco -ErrorAction Stop
        Write-Host "[INFO] Chocolatey found, installing Doxygen..." -ForegroundColor Green
        choco install doxygen.install -y
        return $true
    }
    catch {
        Write-Host "[WARNING] Chocolatey not found." -ForegroundColor Yellow
        return $false
    }
}

# Function to provide manual installation instructions
function Show-ManualInstallInstructions {
    Write-Host "[ERROR] Doxygen is not installed or not in PATH" -ForegroundColor Red
    Write-Host ""
    Write-Host "Manual Installation Options:" -ForegroundColor White
    Write-Host ""
    Write-Host "Option 1 - Download from Official Website:" -ForegroundColor Yellow
    Write-Host "1. Visit: https://www.doxygen.nl/download.html" -ForegroundColor White
    Write-Host "2. Download the Windows installer" -ForegroundColor White
    Write-Host "3. Run the installer and follow the setup wizard" -ForegroundColor White
    Write-Host "4. Make sure to add Doxygen to your PATH during installation" -ForegroundColor White
    Write-Host ""
    Write-Host "Option 2 - Install via Chocolatey:" -ForegroundColor Yellow
    Write-Host "1. Install Chocolatey: https://chocolatey.org/install" -ForegroundColor White
    Write-Host "2. Run: choco install doxygen.install" -ForegroundColor White
    Write-Host ""
    Write-Host "Option 3 - Install via Winget:" -ForegroundColor Yellow
    Write-Host "1. Run: winget install doxygen" -ForegroundColor White
    Write-Host ""
    Write-Host "After installation, restart PowerShell and try again." -ForegroundColor Green
}

# Change to docs directory
$docsPath = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $docsPath

# Handle InstallDoxygen parameter
if ($InstallDoxygen) {
    if (Test-DoxygenInstalled) {
        Write-Host "[INFO] Doxygen is already installed!" -ForegroundColor Green
        doxygen --version
    }
    else {
        if (-not (Install-DoxygenChocolatey)) {
            Show-ManualInstallInstructions
        }
    }
    return
}

# Handle Clean parameter
if ($Clean) {
    Write-Host "[INFO] Cleaning previous documentation..." -ForegroundColor Yellow
    if (Test-Path "output") {
        Remove-Item -Path "output" -Recurse -Force
        Write-Host "[INFO] Previous documentation cleaned successfully!" -ForegroundColor Green
    }
    else {
        Write-Host "[INFO] No previous documentation found to clean." -ForegroundColor Yellow
    }
    return
}

# Check if Doxygen is installed
if (-not (Test-DoxygenInstalled)) {
    Show-ManualInstallInstructions
    Write-Host ""
    Write-Host "TIP: Run this script with -InstallDoxygen to attempt automatic installation" -ForegroundColor Cyan
    Write-Host "Example: .\generate_docs.ps1 -InstallDoxygen" -ForegroundColor Cyan
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host "[INFO] Doxygen found - checking version..." -ForegroundColor Green
$doxygenVersion = doxygen --version
Write-Host "[INFO] Doxygen version: $doxygenVersion" -ForegroundColor Green
Write-Host ""

# Clean previous output if it exists
if (Test-Path "output") {
    Write-Host "[INFO] Cleaning previous documentation..." -ForegroundColor Yellow
    Remove-Item -Path "output" -Recurse -Force
}

# Generate documentation
Write-Host "[INFO] Running Doxygen..." -ForegroundColor Yellow
Write-Host ""

try {
    doxygen Doxyfile
    $doxygenExitCode = $LASTEXITCODE
}
catch {
    Write-Host "[ERROR] Failed to run Doxygen: $($_.Exception.Message)" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit 1
}

# Check if generation was successful
if ($doxygenExitCode -eq 0 -and (Test-Path "output\html\index.html")) {
    Write-Host ""
    Write-Host "[SUCCESS] Documentation generated successfully!" -ForegroundColor Green
    Write-Host ""
    
    $htmlPath = Join-Path $docsPath "output\html\index.html"
    Write-Host "Documentation location: $htmlPath" -ForegroundColor White
    Write-Host ""
    
    # Display documentation structure
    Write-Host "[INFO] Documentation structure:" -ForegroundColor Cyan
    Write-Host "  - HTML Documentation: output\html\index.html" -ForegroundColor White
    Write-Host "  - Class Diagrams: Available in the HTML output" -ForegroundColor White
    Write-Host "  - File Structure: Browse files section in documentation" -ForegroundColor White
    Write-Host "  - API Reference: Classes and namespaces sections" -ForegroundColor White
    Write-Host ""
    
    # Ask user if they want to open the documentation
    if ($OpenDocs) {
        $openChoice = "Y"
    }
    else {
        $openChoice = Read-Host "Do you want to open the documentation now? (Y/N)"
    }
    
    if ($openChoice -eq "Y" -or $openChoice -eq "y") {
        Write-Host "[INFO] Opening documentation in default browser..." -ForegroundColor Yellow
        Start-Process $htmlPath
    }
    
    Write-Host ""
    Write-Host "[TIP] You can also browse the documentation by opening:" -ForegroundColor Cyan
    Write-Host "      $htmlPath" -ForegroundColor White
    Write-Host ""
    Write-Host "[INFO] To regenerate documentation, run this script again" -ForegroundColor Cyan
    Write-Host "[INFO] To clean output, run: .\generate_docs.ps1 -Clean" -ForegroundColor Cyan
    
} else {
    Write-Host ""
    Write-Host "[ERROR] Documentation generation failed!" -ForegroundColor Red
    Write-Host ""
    Write-Host "Common issues:" -ForegroundColor Yellow
    Write-Host "1. Make sure all source files are accessible" -ForegroundColor White
    Write-Host "2. Check that Doxygen configuration is correct" -ForegroundColor White
    Write-Host "3. Verify that input paths in Doxyfile are valid" -ForegroundColor White
    Write-Host "4. Ensure sufficient disk space for output" -ForegroundColor White
    Write-Host ""
    Write-Host "For troubleshooting, check the Doxygen output above for specific errors." -ForegroundColor White
    Read-Host "Press Enter to exit"
    exit 1
}

Write-Host ""
Write-Host "Documentation generation completed!" -ForegroundColor Green
if (-not $OpenDocs) {
    Read-Host "Press Enter to exit"
}
