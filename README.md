<img src="https://img.shields.io/github/license/HANON-games/Shiden"> <img src="https://img.shields.io/github/v/release/HANON-games/Shiden"> <img src="https://img.shields.io/github/release-date/HANON-games/Shiden"> <img src="https://img.shields.io/github/created-at/HANON-games/Shiden"> <img src="https://img.shields.io/github/downloads/HANON-games/Shiden/total"> <img src="https://img.shields.io/github/stars/HANON-games/Shiden?style=flat"> <img src="https://img.shields.io/github/forks/HANON-games/Shiden?style=flat"> <img src="https://img.shields.io/github/repo-size/HANON-games/Shiden"> <img src="https://img.shields.io/github/languages/top/HANON-games/Shiden">

# Shiden Visual Novel Editor

Japanese / [English](https://github.com/HANON-games/Shiden/blob/main/README_en.md)

<br>
<p align="center">
    <img src="https://github.com/user-attachments/assets/e1b16acb-e6e2-4f85-ae70-af718be2836d" width="60%" />
</p>
<br>

## はじめに

Shiden Visual Novel Editor はUE5で簡単にノベルゲームを製作できるプラグインです。<br>
シンプルな作りなのでノベルゲーム以外にも柔軟に対応可能です。

![Shiden Visual Novel Editor](https://github.com/user-attachments/assets/3029cdc2-5460-45dd-8463-c78356569b76)

## 特徴

- 簡易プレビュー付きのエディタ
    - 直感的に操作可能なエディタを同梱しています。
    - 現在の編集行をプレビューできるのでその場で結果を確認しながら編集できます(一部コマンドはプレビュー非対応)。
    - プリセットとしてよく使うコマンドとパラメータの組み合わせを保存できます。
    - マクロ機能により、よく使うコマンドを一つのコマンドとしてまとめることができます。
    - CSV/JSON形式のインポート/エクスポート機能があります。
- ノベルゲームに必要な各種機能
    - 37種類の基本的なコマンド
    - 基本的なセーブ機能
    - シンプルな変数機能
        - セーブスロットごとに管理する変数と、システム変数のどちらも標準搭載しています。
    - 行レベルの既読管理機能
    - 並列処理 (厳密には並行処理) 機能
    - 簡易バックログ機能
    - 簡易フェード機能
- 高い拡張性
    - コマンドは C++/Blueprint のどちらでも実装可能で、プラグイン本体の改造なしに簡単にコマンドを追加出来ます。
    - 追加コマンドをプラグインとして配布することも可能です。
    - エディタ部分は Blueprint で実装されているため、簡単に拡張できます。

## 動作環境

- UE5.3 ～ 5.7
- Windows, Mac, Android

iOS でも動く可能性が高いですが uplugin ファイルの編集が必要です。

## 導入方法

[Releases](https://github.com/HANON-games/Shiden/releases/latest) からダウンロードしたファイルを解凍し、Shiden フォルダをプロジェクトの Plugins フォルダに入れてください。

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
