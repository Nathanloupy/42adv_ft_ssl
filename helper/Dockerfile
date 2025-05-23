FROM debian:10.12-slim

WORKDIR /app

RUN apt-get update && apt-get upgrade -y && \
    apt-get install -y build-essential wget texinfo openssl git valgrind libssl-dev

RUN git config --global user.email 94622983+Nathanloupy@users.noreply.github.com && \
    git config --global user.name "Nathanloupy"

CMD ["tail", "-f", "/dev/null"]