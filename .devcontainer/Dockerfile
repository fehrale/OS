# Используем базовый образ с GCC и CMake
FROM gcc:14.2.0

# Устанавливаем необходимые зависимости
RUN apt-get update && apt-get install -y \
    cmake \
    strace \
    build-essential \
    libzmq3-dev \
    && rm -rf /var/lib/apt/lists/*

# Склонировать cppzmq (header-only)
RUN git clone --depth=1 https://github.com/zeromq/cppzmq /usr/local/include/cppzmq \
    && rm -rf /usr/local/include/cppzmq/.git

# Устанавливаем INIReader
RUN git clone https://github.com/benhoyt/inih.git /tmp/inih \
    && rm -rf /usr/local/include/inih/.git

# Устанавливаем рабочую директорию
WORKDIR /workspace