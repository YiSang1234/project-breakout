#pragma once

// 游戏状态枚举，用于状态机切换
enum class GameStateType {
    MENU,            // 主菜单
    PLAYING,         // 游戏中
    PAUSED,          // 暂停
    GAME_OVER,       // 游戏结束（生命耗尽或超时）
    LEVEL_SCORE,     // 关卡与分数选择界面
    GAME_WIN,        // 通关胜利
    EDITOR,          // 关卡编辑器
    ARCHIVE_SELECT,  // 存档选择界面
    DEBUG            // 调试模式
};