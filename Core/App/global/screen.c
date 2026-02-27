//
// Created by keysk on 2025/11/24.
//

#include "screen.h"

SensorHistory_t soilHistory = { {0}, 0 }; // 初始化土壤缓冲区
SensorHistory_t rainHistory = { {0}, 0 }; // 【新增】：初始化降雨量缓冲区
SensorHistory_t lightHistory = { {0}, 0 }; // 【新增】：初始化光照缓冲区

volatile uint32_t ui_keep_awake_ms = 6000;

// 全局变量定义
ScreenPage pageIndex = PAGE_HOME1; // 当前页面索引，默认为首页

/**
 * @brief 切换到下一个页面
 *
 * 实现页面循环切换：首页 -> 阈值设置页 -> 首页
 * 当到达最后一个页面时，自动回到首页
 *
 * 调用时机：InputTask检测到KEY1按下时调用
 */
void ScreenPage_NextPage() {
    pageIndex++;
    // 如果到达结束标志，循环回到首页
    if (pageIndex == PAGE_End) {
        pageIndex = PAGE_HOME1;
    }
}

// 全局变量定义
RangeEditIndex rangeEditIndex = RANGE_EDIT_TEMPERATURE_MIN; // 当前阈值编辑索引，默认为最小温度

/**
 * @brief 切换到下一个阈值编辑项
 *
 * 在阈值设置页中，按顺序切换到下一个可编辑的阈值项
 * 当到达最后一个项时，循环回到第一项
 *
 * 调用时机：在阈值设置页的浏览模式下，InputTask检测到旋钮右旋时调用
 */
void RangeEditIndex_Next() {
    rangeEditIndex++;
    // 如果到达结束标志，循环回到第一项
    if (rangeEditIndex == RANGE_EDIT_END) {
        rangeEditIndex = RANGE_EDIT_TEMPERATURE_MIN;
    }
}

/**
 * @brief 切换到上一个阈值编辑项
 *
 * 在阈值设置页中，按顺序切换到上一个可编辑的阈值项
 * 当在第一项时，循环跳到最后一项
 *
 * 调用时机：在阈值设置页的浏览模式下，InputTask检测到旋钮左旋时调用
 */
void RangeEditIndex_Prev() {
    if (rangeEditIndex == 0) {
        // 如果在第一项，跳到最后一项
        rangeEditIndex = RANGE_EDIT_END - 1;
    } else {
        // 否则切换到上一项
        rangeEditIndex--;
    }
}

// 全局变量定义
RangeEditState rangeEditState = RANGE_EDIT_STATE_NORMAL; // 当前阈值编辑状态，默认为浏览模式

/**
 * @brief 进入阈值编辑状态
 *
 * 切换到编辑模式，此时用户可以通过旋钮修改当前选中阈值项的值
 * 在编辑模式下，ScreenTask会在选中的值下方显示闪烁的下划线
 */
void RangeEditState_EnterEditing() {
    rangeEditState = RANGE_EDIT_STATE_EDITING;
}

/**
 * @brief 退出阈值编辑状态
 *
 * 切换回浏览模式，此时用户可以通过旋钮切换选中的阈值项
 * 在浏览模式下，ScreenTask会在选中的值下方显示固定的下划线
 */
void RangeEditState_QuitEditing() {
    rangeEditState = RANGE_EDIT_STATE_NORMAL;
}

/**
 * @brief 切换阈值编辑状态
 *
 * 在浏览模式和编辑模式之间切换
 * 如果当前是浏览模式，则进入编辑模式；如果当前是编辑模式，则退出到浏览模式
 *
 * 调用时机：InputTask检测到KEY3按下时调用
 */
void RangeEditState_Toggle() {
    if (rangeEditState == RANGE_EDIT_STATE_NORMAL) {
        RangeEditState_EnterEditing();
    } else {
        RangeEditState_QuitEditing();
    }
}
