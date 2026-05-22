{
  inputs = {
    nixpkgs-pinned.url = "github:NixOS/nixpkgs/f9b86f7794dec7ee309a48546c1bc5e0165009bf";
    gccpin.url = "github:NixOS/nixpkgs/118485230c02d13f62133bce43177ffd49495694";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  };

  outputs = { self, nixpkgs, nixpkgs-pinned, gccpin }:
  let
    system = "x86_64-linux";
    qemuPinPkgs = nixpkgs-pinned.legacyPackages.${system};
    pkgs = nixpkgs.legacyPackages.${system};
    gccpinPkgs = gccpin.legacyPackages.${system};
    crossPkgs = gccpinPkgs.pkgsCross.riscv64;
  in {
    devShells.${system}.default = pkgs.mkShell {
      name = "crossystem-dev-shell";

      nativeBuildInputs = [
        # Cross-compiler toolchain from pinned gcc nixpkgs
        crossPkgs.buildPackages.gcc
        crossPkgs.buildPackages.binutils

        # Host tools from current nixpkgs
        qemuPinPkgs.qemu
        qemuPinPkgs.gdb
        pkgs.gnumake
        pkgs.python311
      ];
      buildInputs = [];
    };
  };
}


