#pragma once

#include "lib_mini_appfx/app.hpp"
#include "lib_mini_appfx/page.hpp"
#include "lib_mini_appfx/component.hpp"

// 作成したページの表示部分だけケアするライブラリ
// タッチ操作等のイベントを受け取って表示中の要素に対するイベント判定を実施する
// 表示に関するイベントはライブラリ内で消費して表示更新を自動的に実行する
