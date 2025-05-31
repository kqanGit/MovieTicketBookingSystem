# Documentation Generation Guide

This directory contains tools and configuration for generating comprehensive API documentation for the Movie Ticket Booking System.

## Quick Start

### Method 1: PowerShell Script (Recommended)

```powershell
# Navigate to docs directory
cd source/docs

# Generate documentation
.\generate_docs.ps1

# Generate and open in browser
.\generate_docs.ps1 -OpenDocs
```

### Method 2: Direct Doxygen Command

```powershell
# Navigate to docs directory  
cd source/docs

# Run Doxygen directly
doxygen Doxyfile
```

## Available Options

### PowerShell Script Parameters

- **No parameters**: Generate documentation using current settings
- **`-InstallDoxygen`**: Automatically install Doxygen (via Chocolatey) if not found
- **`-OpenDocs`**: Generate documentation and open in default browser
- **`-Clean`**: Remove previous documentation output

### Examples

```powershell
# Basic documentation generation
.\generate_docs.ps1

# Install Doxygen and generate docs
.\generate_docs.ps1 -InstallDoxygen

# Generate and immediately view
.\generate_docs.ps1 -OpenDocs

# Clean previous output
.\generate_docs.ps1 -Clean

# Combine parameters
.\generate_docs.ps1 -Clean -OpenDocs
```

## Prerequisites

### Required Software
- **Doxygen**: Download from https://www.doxygen.nl/download.html
- **PowerShell**: Included with Windows, available for Linux/macOS

### Optional Software
- **Chocolatey**: For automatic Doxygen installation
- **GraphViz**: For enhanced diagram generation (automatically included with Doxygen)

## Output Structure

After generation, documentation will be available at:

```
source/docs/output/
├── html/
│   ├── index.html          # Main documentation entry point
│   ├── annotated.html      # Class list
│   ├── classes.html        # Class index
│   ├── files.html          # File index
│   └── ...                 # Additional HTML files
└── latex/                  # LaTeX output (if enabled)
```

## Configuration

### Doxyfile Settings

The `Doxyfile` contains all Doxygen configuration. Key settings:

- **INPUT**: Source code directories to scan
- **OUTPUT_DIRECTORY**: Where to generate documentation
- **GENERATE_HTML**: Enable HTML output (enabled)
- **GENERATE_LATEX**: Enable LaTeX output (disabled by default)
- **EXTRACT_ALL**: Extract documentation for all entities
- **SOURCE_BROWSER**: Include source code browser

### Customization

To modify documentation generation:

1. Edit `Doxyfile` with desired settings
2. Modify `mainpage.dox` for main page content
3. Update `generate_docs.ps1` for additional automation

## Troubleshooting

### Common Issues

**"Doxygen not found"**
- Install Doxygen: https://www.doxygen.nl/download.html
- Add Doxygen to system PATH
- Run with `-InstallDoxygen` parameter

**"PowerShell execution policy"**
```powershell
# Enable script execution (run as Administrator)
Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope LocalMachine
```

**"Permission denied"**
```powershell
# Check file permissions
Get-Acl .\generate_docs.ps1

# Or run PowerShell as Administrator
```

### Manual Installation

If automated installation fails:

1. **Windows**: Download installer from Doxygen website
2. **Chocolatey**: `choco install doxygen.install`
3. **Linux**: `sudo apt-get install doxygen` (Ubuntu/Debian)
4. **macOS**: `brew install doxygen`

## Files Description

- **`Doxyfile`**: Main Doxygen configuration file
- **`generate_docs.ps1`**: PowerShell automation script (RECOMMENDED)
- **`mainpage.dox`**: Main documentation page content
- **`README.md`**: This file - usage instructions
- **`output/`**: Generated documentation (created after first run)

## Integration with IDE

### Visual Studio Code
1. Install "Doxygen Documentation Generator" extension
2. Use `Ctrl+Shift+P` → "Generate Doxygen Documentation"
3. Or use terminal: `.\generate_docs.ps1 -OpenDocs`

### Command Line Integration
```powershell
# Add to PowerShell profile for global access
function Generate-Docs { 
    Set-Location "source/docs"
    .\generate_docs.ps1 @args
}
```

## Best Practices

1. **Always use PowerShell script** instead of direct Doxygen commands
2. **Review output** in browser after generation
3. **Keep Doxyfile updated** with new source directories
4. **Clean output** periodically to ensure fresh generation
5. **Document code** with proper Doxygen comments for best results

## Support

For issues with documentation generation:
1. Check this README for common solutions
2. Verify Doxygen installation and PATH configuration
3. Review PowerShell execution policy settings
4. Contact development team for project-specific issues
