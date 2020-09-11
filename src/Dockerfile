# escape=`

FROM ubuntu

ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt-get update && apt-get upgrade; `
apt-get -y install make; `
apt-get -y install texinfo; `
apt-get -y install build-essential; `
apt-get -y install nasm; `
apt-get -y install gcc; `
apt-get -y install g++; `
apt-get -y install bison; `
apt-get -y install flex; `
apt-get -y install gmp; `
apt-get -y install mpfr; `
apt-get -y install mpc; `
apt-get -y install libisl15; `
apt-get -y install cloog-isl; `
apt-get -y install libgmp3-dev; `
apt-get -y install libgmp-dev; `
apt-get -y install libisl-dev; `
apt-get -y install libcloog-isl-dev; `
apt-get -y install libmpc-dev; `
apt-get -y install libmpfr-dev; `
apt-get -y install xorriso; `
apt-get -y install curl
RUN apt-get -y install grub-common
RUN mkdir $HOME/src
RUN curl ftp://ftp.gnu.org/gnu/binutils/binutils-2.29.tar.gz -o $HOME/src/binutils-2.29.tar.gz
RUN curl ftp://ftp.gnu.org/gnu/gcc/gcc-7.2.0/gcc-7.2.0.tar.gz -o $HOME/src/gcc-7.2.0.tar.gz

RUN tar -xf $HOME/src/binutils-2.29.tar.gz -C $HOME/src/.
RUN tar -xf $HOME/src/gcc-7.2.0.tar.gz -C $HOME/src/.
RUN ls $HOME/src/
#Preparation
ENV PREFIX $HOME/opt/cross
ENV TARGET i686-elf
ENV PATH $PREFIX/bin:$PATH

#Binutils
RUN echo Building binutils; `
cd $HOME/src; `
mkdir build-binutils; `
cd build-binutils; `
../binutils-2.29/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
RUN cd $HOME/src/build-binutils; `
make
RUN cd $HOME/src/build-binutils; `
make install

#GCC
RUN cd $HOME/src; `
 
# The $PREFIX/bin dir _must_ be in the PATH. We did that above.
which -- $TARGET-as || echo $TARGET-as is not in the PATH; `
 
mkdir build-gcc
RUN cd $HOME/src/build-gcc; `
../gcc-7.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c --without-headers --without-isl
RUN cd $HOME/src/build-gcc; `
make all-gcc
RUN cd $HOME/src/build-gcc; `
make all-target-libgcc
RUN cd $HOME/src/build-gcc; `
make install-gcc
RUN cd $HOME/src/build-gcc; `
make install-target-libgcc

RUN echo Removing source files...; `
rm -rf /root/src
RUN apt-get -y install grub-pc-bin
WORKDIR /src
CMD make all
