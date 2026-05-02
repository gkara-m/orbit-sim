{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    cmake
    ninja
    clang
    nodejs_20
    typescript
    typescript-language-server
  ];

  buildInputs = with pkgs; [
    libGL
    libx11
    libxcursor
    libxi
    libxinerama
    libxrandr
    alsa-lib
  ];
  shellHook = ''
    export PATH="$PWD/node_modules/.bin:$PATH"
    export CC=clang
    export CXX=clang++
    
    # Bridge the gap between Arch Host and Nix Shell
    export LD_LIBRARY_PATH="/usr/lib:/usr/lib32:${pkgs.lib.makeLibraryPath (with pkgs; [
      libGL
      libx11
      libxcursor
      libxi
      libxinerama
      libxrandr
    ])}:$LD_LIBRARY_PATH"
  '';
}
