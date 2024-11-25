#!/bin/bash

# Backup function
backup() {
    local bakname=$1
    local bakdir="bak/$bakname"
    
    # Check if backup name was provided
    if [ -z "$bakname" ]; then
        echo "Usage: ./build.sh backup [name]"
        exit 1
    fi
    
    echo "Creating backup directory: $bakdir"
    mkdir -p "$bakdir"
    
    echo "Backing up files and directories..."
    cp build.sh "$bakdir/build.sh.bak"
    cp compile_shaders.sh "$bakdir/compile_shaders.sh.bak"
    cp genie.lua "$bakdir/genie.lua.bak"
    cp -r shaders "$bakdir/shaders.bak"
    cp -r src "$bakdir/src.bak"
    
    echo "Backup completed in: $bakdir"
}

# Restore function
restore() {
    local bakname=$1
    local bakdir="bak/$bakname"
    
    # Check if backup name was provided
    if [ -z "$bakname" ]; then
        echo "Usage: ./build.sh restore [name]"
        exit 1
    fi
    
    # Check if backup exists
    if [ ! -d "$bakdir" ]; then
        echo "Error: Backup '$bakname' not found in bak directory"
        exit 1
    fi
    
    echo "Restoring from backup: $bakdir"
    cp "$bakdir/compile_shaders.sh.bak" compile_shaders.sh
    chmod +x compile_shaders.sh  # Make compile_shaders.sh executable
    cp "$bakdir/genie.lua.bak" genie.lua
    rm -rf shaders && cp -r "$bakdir/shaders.bak" shaders
    rm -rf src && cp -r "$bakdir/src.bak" src
    
    echo "Restore completed from: $bakdir"
}

# Clean project files function
clean_project_files() {
    echo "Cleaning project files..."
    rm -rf bgfx_project
    rm -f compile_shaders.sh genie.lua
    rm -rf shaders src
    rm -f *.make Makefile
    rm -rf obj bin
    echo "Project files cleaned"
}

# Main script
case "$1" in
    "backup")
        backup "$2"
        exit 0
        ;;
    "restore")
        restore "$2"
        exit 0
        ;;
    "clean-project-files")
        clean_project_files
        exit 0
        ;;
esac

# Enter 3rdparty directory
cd 3rdparty

# Clone repositories if they don't exist
if [ ! -d "bgfx" ]; then
    echo "Cloning bgfx..."
    git clone https://github.com/bkaradzic/bgfx.git
fi

if [ ! -d "bimg" ]; then
    echo "Cloning bimg..."
    git clone https://github.com/bkaradzic/bimg.git
fi

if [ ! -d "bx" ]; then
    echo "Cloning bx..."
    git clone https://github.com/bkaradzic/bx.git
fi

if [ ! -d "genie" ]; then
    echo "Cloning genie..."
    git clone https://github.com/bkaradzic/genie.git
fi

# Build bgfx
echo "Building bgfx..."
cd bgfx
make linux-gcc -j13

# Build genie
echo "Building genie..."
cd ../genie
make -j13

# Return to project root
cd ../..

echo "Build completed!"