{
  description = "An user-friendly Graphical Interface";

  inputs = {
    flake-parts.url = "github:hercules-ci/flake-parts";
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = inputs.nixpkgs.lib.systems.flakeExposed;
      perSystem =
        { pkgs, self', ... }:
        {
          packages = {
            default = self'.packages.gepetto-viewer-corba;
            gepetto-viewer-corba = pkgs.python3Packages.gepetto-viewer-corba.overrideAttrs (_: {
              src = pkgs.lib.fileset.toSource {
                root = ./.;
                fileset = pkgs.lib.fileset.unions [
                  ./blender
                  ./CMakeLists.txt
                  ./doc
                  ./examples
                  ./idl
                  ./include
                  ./package.xml
                  ./plugins
                  ./src
                  ./tests
                ];
              };
            });
          };
        };
    };
}
