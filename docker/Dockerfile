FROM gcc:4.9

COPY . /usr/src/myapp
WORKDIR /usr/src/myapp

RUN gcc -o myapp sigkill_in_container.c
CMD ["./myapp"]
