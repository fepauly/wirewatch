FROM ubuntu:latest

RUN apt-get update && apt-get install -y \
    build-essential \      
    libpcap-dev \          
    curl \                 
    git \
    cmake \
    cppcheck                  


WORKDIR /usr/src/app

COPY . .

CMD ["bash"]
