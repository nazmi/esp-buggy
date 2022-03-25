FROM nazmiropi/esp-buggy:base

LABEL   org.opencontainers.image.authors="Nazmi <muhammad.binmohdropi@student.manchester.ac.uk>" \
        org.opencontainers.image.title="Line Following Robot" \
        org.opencontainers.image.description="Compile and Deploy Mbed Application" \
        org.opencontainers.image.url="https://github.com/nazmi/esp-buggy.git"

ENV BASE=/root/esp-buggy
COPY src ${BASE}/src/
COPY include ${BASE}/include/
COPY mbed-os.lib ${BASE}
COPY mbed_app.json ${BASE}

WORKDIR ${BASE}
RUN     set -e \
        && mbed config --global cache none \
        && mbed new . \
        && mbed deploy \
        && mbed compile -t GCC_ARM -m NUCLEO_F401RE --profile release \
        && rm -rf mbed-os \
        && : # last line



