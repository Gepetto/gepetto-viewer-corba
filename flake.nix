{
  description = "An user-friendly Graphical Interface";

  inputs = {
    gepetto.url = "github:gepetto/nix";
    flake-parts.follows = "gepetto/flake-parts";
    nixpkgs.follows = "gepetto/nixpkgs";
    nix-ros-overlay.follows = "gepetto/nix-ros-overlay";
    systems.follows = "gepetto/systems";
    treefmt-nix.follows = "gepetto/treefmt-nix";
  };

  outputs =
    inputs:
    inputs.flake-parts.lib.mkFlake { inherit inputs; } {
      systems = import inputs.systems;
      imports = [ inputs.gepetto.flakeModule ];
      perSystem =
        {
          lib,
          pkgs,
          self',
          ...
        }:
        {
          packages = {
            default = self'.packages.gepetto-viewer-corba;
            gepetto-viewer-corba = pkgs.python3Packages.gepetto-viewer-corba.overrideAttrs {
              src = lib.fileset.toSource {
                root = ./.;
                fileset = lib.fileset.unions [
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
            };
          };
        };
    };
}
