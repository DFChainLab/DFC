FROM dfcio/builder
ARG branch=master
ARG symbol=DFC
ARG precision=8

RUN git clone -b $branch https://github.com/FutureSharesCloud/dfc.git --recursive \
    && cd dfc && echo "$branch:$(git rev-parse HEAD)" > /etc/dfcio-version \
    && cmake -H. -B"/opt/dfcio" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/opt/dfcio -DBUILD_MONGO_DB_PLUGIN=true -DCORE_SYMBOL_NAME=$symbol -DCORE_SYMBOL_PRECISION=$precision  \
    && cmake --build /opt/dfcio --target install \
    && cp /dfc/Docker/config.ini / && ln -s /opt/dfcio/contracts /contracts && cp /dfc/Docker/noddfcd.sh /opt/dfcio/bin/noddfcd.sh && ln -s /dfc/tutorials /tutorials

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl ca-certificates vim psmisc python3-pip && rm -rf /var/lib/apt/lists/*
RUN pip3 install numpy
ENV DFCIO_ROOT=/opt/dfcio
RUN chmod +x /opt/dfcio/bin/noddfcd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
ENV PATH /opt/dfcio/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
