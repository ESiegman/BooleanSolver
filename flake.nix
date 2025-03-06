{
  description = "C++ Boolean Algebra Solver";

  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";

  outputs = { self, nixpkgs }: 
  let
    pkgs = nixpkgs.legacyPackages.x86_64-linux;
  in {
    devShells.x86_64-linux.default = pkgs.mkShell {
      name = "cpp-dev-env";
      buildInputs = [
        pkgs.cmake
        pkgs.gcc
      ];
    };
  };
}

