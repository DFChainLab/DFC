ARG baseEnvDeocker=dfcio/builder

FROM $baseEnvDeocker as builder

ARG branch=master
ARG symbol=DFC
ARG precision=8

RUN git clone -b $branch https://github.com/FutureSharesCloud/dfc.git --recursive \
    && cd dfc && echo "$branch:$(git rev-parse HEAD)" > /etc/dfcio-version \
    && cmake -H. -B"/tmp/build" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/tmp/build -DBUILD_MONGO_DB_PLUGIN=true -DBUILD_KAFKA_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol -DCORE_SYMBOL_PRECISION=$precision \
    && cmake --build /tmp/build --target install


FROM ubuntu:18.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates && rm -rf /var/lib/apt/lists/*
COPY --from=builder /usr/local/lib/* /usr/local/lib/
COPY --from=builder /tmp/build/bin /opt/dfcio/bin
COPY --from=builder /tmp/build/contracts /contracts
COPY --from=builder /dfc/Docker/config.ini /
COPY --from=builder /etc/dfcio-version /etc
COPY --from=builder /dfc/Docker/noddfcd.sh /opt/dfcio/bin/noddfcd.sh
ENV DFCIO_ROOT=/opt/dfcio
RUN chmod +x /opt/dfcio/bin/noddfcd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
ENV PATH /opt/dfcio/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
