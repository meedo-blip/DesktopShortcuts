#!/bin/bash

# Check if Qt5Widgets is available
if pkg-config --exists Qt5Widgets; then
    echo "Qt5Widgets found, proceeding with build..."
else
    echo "Qt5Widgets not found, installing..."
    sudo dnf install -y qt5-qtbase-devel
fi

# Build the application
echo "Building the application..."
g++ src/main.cpp -o shortcut `pkg-config --cflags --libs Qt5Widgets`

if [ $? -eq 0 ]; then
    echo "Build successful! Run ./shortcut to start the application."
else
    echo "Build failed!"
    exit 1
fi
