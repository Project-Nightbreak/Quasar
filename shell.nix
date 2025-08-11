{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = [
    pkgs.gcc
    pkgs.cmake
    pkgs.pkg-config
    pkgs.boost
    pkgs.curl
  ];

  shellHook = ''
    echo "Welcome to Quasar devshell 🚀"
  '';
}
