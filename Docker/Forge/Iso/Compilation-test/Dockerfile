FROM debian:latest
WORKDIR /
ADD test.tar test.tar
RUN apt-get update &&  apt-get -y install libjson-c-dev \
    gcc \
    make
CMD ["tar","xvf","test.tar"]
