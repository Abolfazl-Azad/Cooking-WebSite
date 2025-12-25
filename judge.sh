#!/bin/bash

# نام فایل اجرایی
EXECUTABLE="./UTCOOK"

# نام فایل‌های CSV در پوشه اصلی (طبق تصویر)
INGREDIENTS="ingredients.csv"
RECIPES="recipes.csv"

# اصلاح مسیر تست‌ها بر اساس تصویر VS Code شما
# پوشه اول SampleTest و داخل آن دوباره پوشه SampleTest است
TEST_DIR="./SampleTest/SampleTest"

INPUT_FILE="$TEST_DIR/sample.in"
EXPECTED_OUTPUT="$TEST_DIR/sample.out"
MY_OUTPUT="out.out"

# بررسی وجود فایل اجرایی
if [ ! -f "$EXECUTABLE" ]; then
    echo "Error: Executable $EXECUTABLE not found. Please run 'make' first."
    exit 1
fi

# بررسی وجود فایل‌های ورودی تست
if [ ! -f "$INPUT_FILE" ]; then
    echo "Error: Input file not found at $INPUT_FILE"
    echo "Current directory: $(pwd)"
    echo "Contents of SampleTest/SampleTest:"
    ls "$TEST_DIR" 2>/dev/null
    exit 1
fi

echo "Running Test..."

# اجرای برنامه
$EXECUTABLE "$INGREDIENTS" "$RECIPES" < "$INPUT_FILE" > "$MY_OUTPUT"

echo "Comparing results..."

# مقایسه با استفاده از diff
# سوییچ -w باعث می‌شود فاصله‌های خالی (space/tab) نادیده گرفته شوند
# سوییچ -B خطوط خالی را نادیده می‌گیرد
if diff -w -B "$MY_OUTPUT" "$EXPECTED_OUTPUT" > /dev/null; then
    echo -e "\e[32mPASS: Output matches sample.out!\e[0m"
else
    echo -e "\e[31mFAIL: Output does not match.\e[0m"
    echo "Differences (Your Output | Expected Output):"
    echo "-------------------------------------------"
    diff -y -w -B "$MY_OUTPUT" "$EXPECTED_OUTPUT"
fi