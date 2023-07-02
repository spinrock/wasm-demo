# WASM-DEMO

## これは何

- WebAssembly 初心者による動きを学ぶためのサンプルソース
- GitLab Pages に出力している
  - https://spinrock.github.io/wasm-demo/

## 構成

- public : 出力先ディレクトリ。GitHub Pages もここをアーティファクトとしてアップロードしている
- src : C++ソース。これを emcc でコンパイルして public ディレクトリ側に出力している

## 更新方法

- html : public ディレクトリにあるソースを直接編集する
- wasm : src にある cpp ファイルを更新してコンパイルする
  ```console
  % emcc -sEXPORTED_RUNTIME_METHODS=stringToUTF8,lengthBytesUTF8 -sEXPORTED_FUNCTIONS=_malloc,_main src/todo.cpp -o public/index.js
  ```
