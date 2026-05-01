{ pkgs ? import <nixpkgs> {} }: 

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    clang
  ];

  buildInputs = with pkgs; [
    nlohmann_json
  ];
}
