{
  description = "paper on rotate";

  inputs = {
    nixpkgs.url = "nixpkgs/nixos-25.11";
    utils.url = "github:numtide/flake-utils";
    # utils.inputs.nixpkgs.follows = "nixpkgs";
  };

  outputs = { self, nixpkgs, utils }:
    utils.lib.eachDefaultSystem 
    ( system:
      let
        pkgs = import nixpkgs {
          inherit system;
        };
        name = "rotate";
      in
      rec
      {
        devShell = pkgs.mkShell rec {
          inherit name;
          packages = with pkgs; [
            binutils-unwrapped
            clang
            clang-tools
            gcc
            texlive.combined.scheme-full
            papi
            libcxx
            boost
            gmp
            gnumake
          ];
        };
      }
    );
}
