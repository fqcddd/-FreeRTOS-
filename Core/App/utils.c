#include "utils.h"

/**
 * 将浮点数转换为整数和小数部分（保留1位小数）
 * @param value 浮点数值
 * @param intPart 输出的整数部分指针
 * @param decPart 输出的小数部分指针（0-9）
 */
void floatToIntDec(float value, int *intPart, int *decPart) {
    *intPart = (int)value;
    float decimal = value - (float)(*intPart);
    if (decimal < 0) {
        decimal = -decimal; // 处理负数的小数部分
    }
    *decPart = (int)(decimal * 10 + 0.5f); // 四舍五入到1位小数
    if (*decPart >= 10) {
        *decPart = 0;
        (*intPart)++;
    } else if (*decPart < 0) {
        *decPart = 0;
    }
}

/**
 * @brief 将双精度浮点数(double)转换为整数和小数部分（保留1位小数）
 * @note 专为规避 sprintf("%f") 的高昂开销而设计，极速渲染 OLED
 * @param value 双精度浮点数值 (比如 BMP280 算出的气压或温度)
 * @param intPart 输出的整数部分指针
 * @param decPart 输出的小数部分指针（0-9）
 */
void doubleToIntDec(double value, int *intPart, int *decPart) {
    // 1. 提取整数部分
    *intPart = (int)value;

    // 2. 提取纯小数部分
    double decimal = value - (double)(*intPart);

    // 3. 处理负数情况，保证小数部分绝对值计算
    if (decimal < 0) {
        decimal = -decimal;
    }

    // 4. 四舍五入保留1位小数 (注意这里把 0.5f 改成了 0.5，对应 double 的精度)
    *decPart = (int)(decimal * 10.0 + 0.5);

    // 5. 处理进位：例如 23.96 四舍五入会变成 24.0
    if (*decPart >= 10) {
        *decPart = 0;

        // 【已修复原版 BUG】：处理负数的进位
        // 如果温度是 -1.96，四舍五入应该是 -2.0，而不是 0.0！
        if (value < 0) {
            (*intPart)--; // 负数进位是往下减
        } else {
            (*intPart)++; // 正数进位是往上加
        }
    }
}