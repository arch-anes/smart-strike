FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-20.04

# Node

RUN curl -sS https://dl.yarnpkg.com/debian/pubkey.gpg | sudo apt-key add - \
  && echo "deb https://dl.yarnpkg.com/debian/ stable main" | sudo tee /etc/apt/sources.list.d/yarn.list \
  && curl -sL https://deb.nodesource.com/setup_14.x | sudo -E bash - \
  && apt update \
  && apt install -y yarn nodejs \
  && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

# Tools

RUN apt update \
  && apt install -y clang-format clang-tidy fish python3-pip \
  && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*
