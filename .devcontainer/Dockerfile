FROM mcr.microsoft.com/vscode/devcontainers/base:ubuntu-20.04

# Android SDK

ENV ANDROID_HOME=/opt/android-sdk-linux \
  LANG=en_US.UTF-8 \
  LC_ALL=en_US.UTF-8 \
  LANGUAGE=en_US:en

ENV ANDROID_SDK_ROOT=$ANDROID_HOME \
  PATH=${PATH}:${ANDROID_HOME}/cmdline-tools/tools/bin:${ANDROID_HOME}/platform-tools:${ANDROID_HOME}/emulator

# comes from https://developer.android.com/studio/#command-tools
ENV ANDROID_SDK_TOOLS_VERSION 6609375

RUN set -o xtrace \
  && cd /opt \
  && apt update \
  && apt install -y openjdk-8-jdk \
  && apt install -y sudo wget zip unzip git openssh-client curl bc software-properties-common build-essential ruby-full ruby-bundler lib32stdc++6 libstdc++6 libpulse0 libglu1-mesa locales lcov libsqlite3-0 --no-install-recommends \
  # for x86 emulators
  && apt install -y libxtst6 libnss3-dev libnspr4 libxss1 libasound2 libatk-bridge2.0-0 libgtk-3-0 libgdk-pixbuf2.0-0 \
  && rm -rf /var/lib/apt/lists/* \
  && sh -c 'echo "en_US.UTF-8 UTF-8" > /etc/locale.gen' \
  && locale-gen \
  && update-locale LANG=en_US.UTF-8 \
  && wget -q https://dl.google.com/android/repository/commandlinetools-linux-${ANDROID_SDK_TOOLS_VERSION}_latest.zip -O android-sdk-tools.zip \
  && mkdir -p ${ANDROID_HOME}/cmdline-tools/ \
  && unzip -q android-sdk-tools.zip -d ${ANDROID_HOME}/cmdline-tools/ \
  && chown -R root:root $ANDROID_HOME \
  && rm android-sdk-tools.zip \
  # && echo '%sudo ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers \
  && yes | sdkmanager --licenses \
  && wget -O /usr/bin/android-wait-for-emulator https://raw.githubusercontent.com/travis-ci/travis-cookbooks/master/community-cookbooks/android-sdk/files/default/android-wait-for-emulator \
  && chmod +x /usr/bin/android-wait-for-emulator \
  && touch /root/.android/repositories.cfg \
  && sdkmanager platform-tools \
  && sdkmanager emulator \
  && mkdir -p /root/.android \
  && touch /root/.android/repositories.cfg

ENV ANDROID_PLATFORM_VERSION 30
ENV ANDROID_BUILD_TOOLS_VERSION 30.0.2

RUN yes | sdkmanager \
  "platforms;android-$ANDROID_PLATFORM_VERSION" \
  "build-tools;$ANDROID_BUILD_TOOLS_VERSION"

# Flutter

USER vscode

ENV FLUTTER_VERSION 2.0.0

ENV FLUTTER_HOME /home/vscode/sdks/flutter
ENV FLUTTER_ROOT $FLUTTER_HOME

ENV PATH ${PATH}:${FLUTTER_HOME}/bin:${FLUTTER_HOME}/bin/cache/dart-sdk/bin

RUN git clone --branch ${FLUTTER_VERSION} https://github.com/flutter/flutter.git ${FLUTTER_HOME}

RUN yes | flutter doctor --android-licenses && flutter doctor

USER root

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
