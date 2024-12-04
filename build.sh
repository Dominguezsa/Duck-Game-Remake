#!/bin/bash

# Define BUILD_TYPE to control the build mode
BUILD_TYPE=Release

echo "Starting building process..."

echo "Building server..."
make BUILD_TYPE=${BUILD_TYPE} compile-server

echo "Building client..."
make BUILD_TYPE=${BUILD_TYPE} compile-client

echo "Building editor..."
make BUILD_TYPE=${BUILD_TYPE} compile-editor

echo "Both client and server builds completed successfully!"
