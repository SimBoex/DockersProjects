# Use a recent Ubuntu Long Term Support (LTS) release as the base image
FROM ubuntu:22.04

# Set environment variables to avoid interactive prompts during apt installs
ENV DEBIAN_FRONTEND=noninteractive

# Update package list and install essential build tools
# - build-essential: Includes g++, make, etc.
# - cmake: The build system generator
# - git: For cloning repositories (like googletest)
# - apt-utils: For a more robust apt experience in scripts
# - libstdc++-11-dev: Explicitly ensure C++17 standard library support (for std::filesystem)
#   (Ubuntu 22.04 typically uses GCC 11, which provides C++17 filesystem with libstdc++-11-dev)
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    apt-utils \
    libstdc++-11-dev \
    libgtest-dev \
    google-mock \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory inside the container
WORKDIR /app

# Define the default command to run when the container starts without a specific command
CMD ["bash"]