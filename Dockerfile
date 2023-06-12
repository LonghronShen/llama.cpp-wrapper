ARG BASE_IMAGE=ubuntu:22.04
FROM ${BASE_IMAGE} as builder

ARG HTTPS_PROXY=

WORKDIR /app

# RUN sed -i 's@//.*archive.ubuntu.com@//mirrors.ustc.edu.cn@g' /etc/apt/sources.list && \
#     sed -i 's/security.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list

COPY ./utilities/bootstrap.sh ./utilities/bootstrap.sh

RUN bash ./utilities/bootstrap.sh

COPY . .

RUN bash ./utilities/build.sh

# =================================================================
ARG BASE_IMAGE=ubuntu:22.04
FROM ${BASE_IMAGE} as runner

ARG TARGETPLATFORM
ENV TARGETPLATFORM=${TARGETPLATFORM:-linux/amd64}
ARG TARGETARCH
ENV TARGETARCH=${TARGETARCH:-amd64}
ARG BUILDPLATFORM

ARG HTTPS_PROXY=

# RUN sed -i 's@//.*archive.ubuntu.com@//mirrors.ustc.edu.cn@g' /etc/apt/sources.list && \
#     sed -i 's/security.ubuntu.com/mirrors.ustc.edu.cn/g' /etc/apt/sources.list

WORKDIR /app

COPY --from=builder /app/build/bin/llama-cpp-wrapper-server-exe llama-cpp-wrapper-server-exe
COPY --from=builder /app/utilities/install_runtime_deps.sh install_runtime_deps.sh

RUN chmod a+x install_runtime_deps.sh && ./install_runtime_deps.sh

EXPOSE 5000

CMD ["./llama-cpp-wrapper-server-exe"]
