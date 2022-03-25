FROM nazmiropi/esp-buggy:tools

LABEL   org.opencontainers.image.authors="Nazmi <muhammad.binmohdropi@student.manchester.ac.uk>" \
        org.opencontainers.image.title="Line Following Robot" \
        org.opencontainers.image.description="Compile and Deploy Mbed Application" \
        org.opencontainers.image.url="https://github.com/nazmi/esp-buggy.git"

WORKDIR /root/esp-buggy

COPY . ./

RUN     set -e \
        && mbed cache off \
        && mbed new . \
        && mbed deploy \
        && mbed compile -t GCC_ARM -m NUCLEO_F401RE --profile release \
        && rm -rf mbed-os \
        && : # last line