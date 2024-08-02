# Shiden Visual Novel Editor

[English doc](https://github.com/HANON-games/Shiden/blob/main/README_en.md)

## はじめに

Shiden Visual Novel Editor はUE5で簡単にノベルゲームを製作できるプラグインです。<br>
シンプルな作りなのでノベルゲーム以外にも柔軟に対応可能です。

![Shiden Visual Novel Editor](https://private-user-images.githubusercontent.com/174848262/354755058-4700a131-c693-4bf8-af7c-b507110794a9.png?jwt=eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJnaXRodWIuY29tIiwiYXVkIjoicmF3LmdpdGh1YnVzZXJjb250ZW50LmNvbSIsImtleSI6ImtleTUiLCJleHAiOjE3MjI2NDA4MTYsIm5iZiI6MTcyMjY0MDUxNiwicGF0aCI6Ii8xNzQ4NDgyNjIvMzU0NzU1MDU4LTQ3MDBhMTMxLWM2OTMtNGJmOC1hZjdjLWI1MDcxMTA3OTRhOS5wbmc_WC1BbXotQWxnb3JpdGhtPUFXUzQtSE1BQy1TSEEyNTYmWC1BbXotQ3JlZGVudGlhbD1BS0lBVkNPRFlMU0E1M1BRSzRaQSUyRjIwMjQwODAyJTJGdXMtZWFzdC0xJTJGczMlMkZhd3M0X3JlcXVlc3QmWC1BbXotRGF0ZT0yMDI0MDgwMlQyMzE1MTZaJlgtQW16LUV4cGlyZXM9MzAwJlgtQW16LVNpZ25hdHVyZT1mNTE2OTc0NGM2MjI1YTQzY2QwOWExZDk0NTU2MGU0YWFmMzdlYWU0ZTg5YjQ5ODA2NTIxZjAxZDU4MTZiZWEyJlgtQW16LVNpZ25lZEhlYWRlcnM9aG9zdCZhY3Rvcl9pZD0wJmtleV9pZD0wJnJlcG9faWQ9MCJ9.HIxqutjHC8H2rqDqcim_1yQ1fqS0mymAqAFy23M3cTs)

本プラグインはベータ版です。<br>
Version 1.0 リリース時に破壊的変更が入る可能性がありますので予めご了承ください。

## 特徴

- 簡易プレビュー付きのエディタ
    - 直感的に操作可能なエディタを同梱しています。
    - 現在の編集行をプレビューできるのでその場で結果を確認しながら編集できます(一部コマンドはプレビュー非対応)。
    - プリセットとしてよく使うコマンドとパラメータの組み合わせを保存できます。
    - CSV形式のインポート/エクスポート機能があります。
- ノベルゲームに必要な各種機能
    - 32種類の基本的なコマンド
    - 基本的なセーブ機能
    - シンプルな変数機能
        - セーブスロットごとに管理する変数と、システム変数のどちらも標準搭載しています。
    - 行レベルの既読管理機能
    - 並列処理 (厳密には並行処理) 機能
    - 簡易バックログ機能
    - 簡易フェード機能
- 高い拡張性
    - すべてのコマンドは Blueprint で記述されており、追加コマンドを簡単に実装できます。
    - 追加コマンドをプラグインとして配布することも可能です。
    - エディタ部分も Blueprint で実装されているため、簡単に拡張できます。

## 動作環境

- UE5.3 ～ UE5.4
- Windows, Android

Mac, iOS でも動く可能性が高いですが uplugin ファイルの編集が必要です。

## 導入方法

[Releases](https://github.com/HANON-games/Shiden/releases/latest) からダウンロードしたファイルを解凍し、Shiden フォルダをプロジェクトの Plugin フォルダに入れてください。

## 使い方

[Wiki](https://github.com/HANON-games/Shiden/wiki) をご覧ください。

## ライセンス

MIT

## 作った人

[葉乃音 (HANON)](https://twitter.com/HanonHeartKnows)

## 不具合報告

[issues](https://github.com/HANON-games/Shiden/issues) にご報告いただけますと幸いです。<br>
作者多忙のため、返信が遅れることがありますがご了承ください。

## 変更履歴

[Announcements - お知らせ](https://github.com/HANON-games/Shiden/discussions/categories/announcements-%E3%81%8A%E7%9F%A5%E3%82%89%E3%81%9B) をご覧ください。
