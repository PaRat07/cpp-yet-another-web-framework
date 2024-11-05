FROM ubuntu:latest
RUN apt update
RUN apt upgrade -y
RUN apt -y install ninja-build curl zip unzip tar wget cmake git pkg-config gdb
RUN wget -qO /tmp/vcpkg.tar.gz https://github.com/microsoft/vcpkg/archive/master.tar.gz
RUN mkdir /opt/vcpkg && tar xf /tmp/vcpkg.tar.gz --strip-components=1 -C /opt/vcpkg && rm -rf /tmp/vcpkg.tar.gz
RUN /opt/vcpkg/bootstrap-vcpkg.sh && ln -s /opt/vcpkg/vcpkg /usr/local/bin/vcpkg
#RUN wget -qO /tmp/mold.tar.gz https://github.com/rui314/mold/releases/download/v2.34.1/mold-2.34.1-x86_64-linux.tar.gz
#RUN mkdir /opt/mold && tar xf /tmp/mold.tar.gz -C /opt/mold --strip-components=1
#RUN rm -rf /tmp/mold.tar.gz
#RUN ln -s /opt/mold/bin/mold /usr/local/bin/mold
RUN wget -qO /tmp/LLVM-19.1.2-Linux-X64.tar.xz https://github.com/llvm/llvm-project/releases/download/llvmorg-19.1.2/LLVM-19.1.2-Linux-X64.tar.xz
RUN apt install -y xz-utils
RUN mkdir /opt/llvm && tar -xf /tmp/LLVM-19.1.2-Linux-X64.tar.xz --strip-components=1 -C /opt/llvm && rm -rf /tmp/LLVM-19.1.2-Linux-X64.tar.xz
RUN apt install -y rsync
RUN rsync -a /opt/llvm/ /usr/
EXPOSE 1234:1234