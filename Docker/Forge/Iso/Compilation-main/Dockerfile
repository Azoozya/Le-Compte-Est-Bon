FROM debian:latest
WORKDIR /
ADD main.tar main.tar
RUN apt-get update &&  apt-get -y install libjson-c-dev \
    gcc \
    make
CMD ["tar","xvf","main.tar"]
