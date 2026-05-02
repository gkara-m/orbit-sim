{ pkgs ? import <nixpkgs> {} }: 

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    # C++
    cmake
    ninja
    clang

    # TypeScript
    nodejs_20
    typescript
    typescript-language-server
  ];

  # buildInputs = with pkgs; [
  #   nlohmann_json
  # ];

  shellHook = ''
    export PATH="$PWD/node_modules/.bin:$PATH"
  '';
}
