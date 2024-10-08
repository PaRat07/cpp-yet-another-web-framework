FROM ubuntu:latest
RUN apt update && apt -y upgrade && apt install -y build-essential cmake tar wget curl zip git pkg-config ninja-build
WORKDIR /home
RUN wget -O /tmp/vcpkg.tar.gz https://github.com/microsoft/vcpkg/archive/master.tar.gz
RUN tar xf /tmp/vcpkg.tar.gz -C /opt
RUN /opt/vcpkg-master/bootstrap-vcpkg.sh
RUN ln -s /opt/vcpkg-master/vcpkg /usr/local/bin/vcpkg
COPY ./vcpkg.json ./vcpkg-configuration.json .
RUN vcpkg install
COPY . .
RUN mkdir ./build
WORKDIR ./build
RUN cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg-master/scripts/buildsystems/vcpkg.cmake
RUN cmake --build .
CMD /home/build/yawf
