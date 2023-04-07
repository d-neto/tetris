# Tetris
![](https://img.shields.io/github/stars/d-neto/tetris.svg?style=flat) ![](https://img.shields.io/github/forks/d-neto/tetris.svg?style=flat) ![](https://img.shields.io/github/issues/d-neto/tetris.svg?style=flat) ![](https://img.shields.io/badge/PRs-welcome-brightgreen.svg?style=flat) ![](https://img.shields.io/github/downloads/d-neto/tetris/total?style=flat&color=4AF) ![](https://img.shields.io/github/license/d-neto/tetris?style=flat&color=4AF)

Making a tetris game in C language using binary operators.

<div align="center">
  <img src="./svg/tetris.svg" /> 
</div>

Is possible and simple make the tetris logic using bitwise operators like **|** or **&**. The examples below are simple, just using two or less rows to explain the logic inside, the source code is more complex and contain many loops to check each grid row.

##### Operation OR ⟶ |
<div align="center">
  <img src="./svg/or_op.svg" /> 
</div>

It's a bitwise operator that compares and sets value 1 if one of them has value 1.
Using | is possible sum two rows in the main grid.



Example:
```C 
int row1 = 0b1100;
int row2 = 0b0011;
int new_row = row1 | row2;
/* result = 0b1111 */
```

#

##### Operation AND ⟶ &
<div align="center">
  <img src="./svg/and_op.svg" /> 
</div>

This operation sets bit value 1 if all compared bits have value 1.
Using this, the result of `row1 & row2` will be greater than 0 if a row collides with another.

Example: 

```C 
int row1 = 0b1100;
int row2 = 0b0111;
int is_colliding = (row1 & row2) > 0;
/* result = 1 */
```