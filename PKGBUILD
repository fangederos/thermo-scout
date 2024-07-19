# Maintainer: Matheus Lazzarotto <m.v.lazza@gmail.com>
pkgname=thermo-scout
pkgver=1.0.0
pkgrel=1
pkgdesc="Lightweight script designed for real-time monitoring and logging of temperature sensor data on Linux systems."
arch=('x86_64')
url="https://github.com/fangederos/thermo-scout"
license=('GPL')
depends=('lm-sensors')
makedepends=('make' 'gcc')
source=("https://github.com/fangederos/thermo-scout/releases/download/v1.0.0/thermo-scout-1.0.0.tar.gz")
sha256sums=('85cea451eec057fa7e734548ca3ba6d779ed5836a3f9de14b8394575ef0d7d8e') # Ensure this is correct

build() {
  cd "$srcdir/$pkgname-$pkgver"
  make
}

package() {
  cd "$srcdir/$pkgname-$pkgver"
  make DESTDIR="$pkgdir/" install
}
