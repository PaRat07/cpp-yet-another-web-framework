FROM ubuntu:latest
RUN apt update
RUN apt upgrade -y
RUN apt -y install clang ninja-build curl zip unzip tar wget cmake
RUN wget -qO /tmp/vcpkg.tar.gz https://github.com/microsoft/vcpkg/archive/master.tar.gz
RUN mkdir /opt/vcpkg && tar xf /tmp/vcpkg.tar.gz --strip-components=1 -C /opt/vcpkg && rm -rf /tmp/vcpkg.tar.gz
RUN /opt/vcpkg/bootstrap-vcpkg.sh && ln -s /opt/vcpkg/vcpkg /usr/local/bin/vcpkg
RUN wget -qO /tmp/mold.tar.gz https://github.com/rui314/mold/releases/download/v2.34.1/mold-2.34.1-x86_64-linux.tar.gz
RUN mkdir /opt/mold && tar xf /tmp/mold.tar.gz -C /opt/mold --strip-components=1
RUN rm -rf /tmp/mold.tar.gz
RUN ln -s /opt/mold/bin/mold /usr/local/bin/mold
WORKDIR /home
COPY ./vcpkg.json .
COPY ./vcpkg-configuration.json .
RUN apt -y install git pkg-config
RUN vcpkg install
RUN apt install -y gdb
EXPOSE 1234:1234