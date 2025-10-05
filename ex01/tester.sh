#!/bin/bash

GREEN="\033[32m"
BLUE="\033[34m"
RESET="\033[0m"
RED="\033[31m"


printf "$BLUE%s""Test[0] => 3 2 1 + *\n"
out=$(./RPN "3 2 1 + *")
if [ "$out" = "9" ]; then
  printf "$GREEN%s""      Good\n"$RESET
else
{
  printf "$RED%s""      Wrong\n"$RESET
    printf	"$BLUE%s""Your Output =   [$out]\n"$RESET
    printf	"$GREEN%s""Correct Value = [9]\n"$RESET
}
fi


printf "$BLUE%s""Test[1] => 2 4 8 + *\n"
out=$(./RPN "2 4 8 + *")
if [ "$out" = "24" ]; then
    printf "$GREEN%s""      Good\n"$RESET
else
{
    printf "$RED%s""        Wrong\n"$RESET
    printf	"$BLUE%s""Your Output =   [$out]\n"$RESET
    printf	"$GREEN%s""Correct Value = [24]\n"$RESET
}
fi


printf "$BLUE%s""Test[2] => 2 5 * 4 + 3 2 * 1 + / \n"
out=$(./RPN "2 5 * 4 + 3 2 * 1 + / ")
if [ "$out" = "2" ]; then
    printf "$GREEN%s""      Good\n"$RESET
else
{
    printf "$RED%s""        Wrong\n"$RESET
    printf	"$BLUE%s""Your Output =   [$out]\n"$RESET
    printf	"$GREEN%s""Correct Value = [2]\n"$RESET
}
fi

printf "$BLUE%s""Test[3] => 12 * 2 / 5 + 46 * 6 / 8 * 2 / + 2 * 2 -\n"
err=$(./RPN "12 * 2 / 5 + 46 * 6 / 8 * 2 / + 2 * 2 -" 2>&1 >/dev/null)
if [ "$err" = "Error" ]; then
    printf "$GREEN%s""      Good\n"$RESET
else
{
    printf "$RED%s""        Wrong\n"$RESET
    printf "$BLUE%s""Your Output =   [$out]\n"$RESET
    printf "$GREEN%s""Correct Value = [Error]\n"$RESET
}
fi

printf "$BLUE%s""Test[4] => 89 * 9 - 9 - 9 - 4 - 1 +\n"
err=$(./RPN "89 * 9 - 9 - 9 - 4 - 1 +" 2>&1 >/dev/null)
if [ "$err" = "Error" ]; then
    printf "$GREEN%s""      Good\n"$RESET
else
{
    printf "$RED%s""        Wrong\n"$RESET
    printf "$BLUE%s""Your Output =   [$out]\n"$RESET
    printf "$GREEN%s""Correct Value = [Error]\n"$RESET
}
fi

printf "$BLUE%s""Test[5] => 98 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -\n"
# ▼ stderr だけをキャプチャ（順番に注意：2>&1 >/dev/null）
err=$(./RPN "98 * 4 * 4 / 2 + 9 - 8 - 8 - 1 - 6 -" 2>&1 >/dev/null)
if [ "$err" = "Error" ]; then
    printf "$GREEN%s""      Good\n"$RESET
else
{
    printf "$RED%s""        Wrong\n"$RESET
    printf "$BLUE%s""Your stderr =    [$err]\n"$RESET
    printf "$GREEN%s""Correct stderr = [Error]\n"$RESET
}
fi