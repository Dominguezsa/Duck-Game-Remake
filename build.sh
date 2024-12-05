#!/bin/bash

# Define BUILD_TYPE to control the build mode
BUILD_TYPE=Release

echo "Starting building process..."

echo "Building client..."
echo "Building server..."
echo "Building editor..."
make BUILD_TYPE=${BUILD_TYPE} all

echo "Both client and server builds completed successfully!"
