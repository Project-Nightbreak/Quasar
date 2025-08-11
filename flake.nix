{
  description = "Quasar Search Engine Dev Environment";

  inputs.nixpkgs.url = "nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }: {
    devShells.default = nixpkgs.lib.mkShell {
      packages = with nixpkgs; [
        cmake
        gnumake
        gcc
        curl
        pkg-config
      ];
    };
  };
}
