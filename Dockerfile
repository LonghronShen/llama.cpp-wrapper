FROM ubuntu:22.04 as builder

WORKDIR /app

COPY ./utilities/bootstrap.sh bootstrap.sh

RUN bash ./utilities/bootstrap.sh

COPY . .

RUN bash ./utilities/build.sh

FROM ubuntu:22.04 as runner

WORKDIR /app

COPY --from=builder /app/build/bin/llama-cpp-wrapper-server-exe llama-cpp-wrapper-server-exe

EXPOSE 5000

CMD ["./llama-cpp-wrapper-server-exe"]
