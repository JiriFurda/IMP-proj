/*
 * Author: Jiri Furda (xfurda00)
 * Changes: 90% (whole Tetris and LED matrix logic)
 * Last change: 2018-12-30
 */

/*******************************************************************************
   main.c: LCD + keyboard demo
   Copyright (C) 2009 Brno University of Technology,
                      Faculty of Information Technology
   Author(s): Zdenek Vasicek <vasicek AT stud.fit.vutbr.cz>

   LICENSE TERMS

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
   1. Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
   2. Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
   3. All advertising materials mentioning features or use of this software
      or firmware must display the following acknowledgement:

        This product includes software developed by the University of
        Technology, Faculty of Information Technology, Brno and its
        contributors.

   4. Neither the name of the Company nor the names of its contributors
      may be used to endorse or promote products derived from this
      software without specific prior written permission.

   This software or firmware is provided ``as is'', and any express or implied
   warranties, including, but not limited to, the implied warranties of
   merchantability and fitness for a particular purpose are disclaimed.
   In no event shall the company or contributors be liable for any
   direct, indirect, incidental, special, exemplary, or consequential
   damages (including, but not limited to, procurement of substitute
   goods or services; loss of use, data, or profits; or business
   interruption) however caused and on any theory of liability, whether
   in contract, strict liability, or tort (including negligence or
   otherwise) arising in any way out of the use of this software, even
   if advised of the possibility of such damage.

   $Id$


*******************************************************************************/

#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>
#include <stdlib.h>

char last_ch; // naposledy precteny znak

int field[9][10] =  // Static playing field (including boundaries, excluding current shape)
        {
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
                {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };

int led_matrix[8][8] =  // Playing field shown on led matrix (including current shape)
        {
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0},
                {0, 0, 0, 0, 0, 0, 0, 0}
        };

int sRot;   // Rotation of current shape
int sRow;   // y position of current shape
int sCol;   // x position fo current shape
int sCurr;  // Type of current shape
int sNext;  // Type of next shape

int gameOver = 0;   // Value indicating end of the game

int lettersCount = 7;   // Total count of shape types
char letters[] = "IJLOSTZ"; // Letter representation of shape types (order must correspond with shapes array)
int shapes[][4][4][4] = // Shape types with all their rotations
        {
            // Shape I
            {
                    {
                            {0, 0, 0, 0},
                            {1, 1, 1, 1},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 1, 0},
                            {0, 0, 1, 0},
                            {0, 0, 1, 0},
                            {0, 0, 1, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {0, 0, 0, 0},
                            {1, 1, 1, 1},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 1, 0, 0}
                    }
            },
            // Shape J
            {
                    {
                            {1, 0, 0, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 1, 0},
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {1, 1, 1, 0},
                            {0, 0, 1, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {1, 1, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            // Shape L
            {
                    {
                            {0, 0, 1, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {1, 1, 1, 0},
                            {1, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {1, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            // Shape O
            {
                    {
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 1, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            // Shape S
            {
                    {
                            {0, 1, 1, 0},
                            {1, 1, 0, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 1, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {0, 1, 1, 0},
                            {1, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {1, 0, 0, 0},
                            {1, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            // Shape T
            {
                    {
                            {0, 1, 0, 0},
                            {1, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {1, 1, 1, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {1, 1, 0, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    }
            },
            // Shape Z
            {
                    {
                            {1, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 1, 0},
                            {0, 1, 1, 0},
                            {0, 1, 0, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 0, 0, 0},
                            {1, 1, 0, 0},
                            {0, 1, 1, 0},
                            {0, 0, 0, 0}
                    },
                    {
                            {0, 1, 0, 0},
                            {1, 1, 0, 0},
                            {1, 0, 0, 0},
                            {0, 0, 0, 0}
                    }
            }
        };


/**
 * Checks whether position is occupied or not
 * @param row Y position to check
 * @param col X position to check
 * @return 1 if occupied, 0 if not
 */
int is_occupied(int row, int col)
{
    // Check if position is occupied by static field
    if(field[row][col] == 1)
        return 1;

    // Check if position is occupied by current shape
    if(row >= sRow && row < sRow+4 && col >= sCol && col < sCol+4)
        return shapes[sCurr][sRot][row-sRow][col-sCol];

    // Position is not occupied
    return 0;
}


/**
 * Merges visible part of static field and current shape into visible 8x8 field and prints it to terminal
 * Must be called every time the field has changed
 */
void field_changed(void)
{
    term_send_crlf();

    int row;
    int col;

    for(row = 0; row < 8; row++)
    {
        for(col = 1; col < 9; col++)
        {
            if (is_occupied(row, col))
            {
                term_send_str("#");
                led_matrix[row][col-1] = 1;
            }
            else
            {
                term_send_str("_");
                led_matrix[row][col-1] = 0;
            }
        }

        term_send_crlf();
    }
}


/**
 * Determine next shape to be spawn and prints it to LCD
 */
void generate_next_shape(void)
{
    sNext = (sCurr+1) % lettersCount;

    LCD_clear();
    LCD_append_string("Next shape: ");
    LCD_append_char(letters[sNext]);
}


/**
 * Ends the game and prints "GAME OVER!" to LCD
 */
void game_over(void)
{
    LCD_clear();
    LCD_append_string("GAME OVER!");
    gameOver = 1;
}


/**
 * Spawns next shape or end the game if there is no space to spawn
 */
void shape_spawn(void)
{
    // Default values for new current shape
    int newRotation = 0;
    int newRow = 0;
    int newCol = 3;
    int newShape = sNext;

    // Checks for collision
    int row;
    int col;
    for(row = 0; row < 4; row++)
    {
        for(col = 0; col < 4; col++)
        {
            if(shapes[newShape][newRotation][row][col] == 1 && field[newRow+row][newCol+col] == 1)
                return game_over();
        }
    }

    // Setup current shape
    sRot = newRotation;
    sRow = newRow;
    sCol = newCol;
    sCurr = newShape;

    generate_next_shape();
}


/**
 * Rotates the current shape if there is enough space
 */
void shape_rotate(void)
{
    int attempts;
    int row;
    int col;
    int newRotation;
    int failed = 1; // 1 to pass the first iteration

    // Check if any rotation can be done
    for(attempts = 1; attempts < 4 && failed; attempts++)
    {
        failed = 0;
        newRotation = (sRot+attempts) % 4;

        // Check for collision
        for(row = 0; row < 4 && !failed; row++)
        {
            for(col = 0; col < 4 && !failed; col++)
            {
                if(shapes[sCurr][newRotation][row][col] == 1)
                    if(field[sRow+row][sCol+col])
                        failed = 1;
            }
        }
    }

    // Revert rotation if cannot be done
    if(!failed)
        sRot = newRotation;
}


/**
 * Moves the current shape if there is no collision
 * @param rowChange Increment of Y position
 * @param colChange Increment of X position
 * @return 1 if move was successful, 0 if failed
 */
int shape_move(int rowChange, int colChange)
{
    int row;
    int col;

    for(row = 0; row < 4; row++)
    {
        for(col = 0; col < 4; col++)
        {
            if(shapes[sCurr][sRot][row][col] == 1)
                if(field[sRow+row+rowChange][sCol+col+colChange])
                    return 0;
        }
    }

    sRow += rowChange;
    sCol += colChange;

    return 1;
}


/**
 * Adds current shape to static field
 */
void shape_static()
{
    int row;
    int col;

    for(row = 0; row < 4; row++)
    {
        for(col = 0; col < 4; col++)
        {
            if(shapes[sCurr][sRot][row][col] == 1)
                field[sRow+row][sCol+col] = 1;
        }
    }
}


/**
 * Clears whole row except boundaries
 * @param row Y position
 */
void row_clear(int row)
{
    int col;

    for(col = 1; col < 9; col++)
    {
        field[row][col] = 0;
    }
}


/**
 * Checks whether row is full
 * @param row Y position
 * @return 1 if row is full, 0 if not
 */
int row_is_full(int row)
{
    int col;
    for(col = 1; col < 9; col++)
    {
        if(field[row][col] == 0)
            return 0;
    }
    return 1;
}


/**
 * Copies one row to other row
 * @param destRow destination Y position
 * @param srcRow source Y position
 */
void row_copy(int destRow, int srcRow)
{
    int col;
    for(col = 1; col < 9; col++)
    {
        field[destRow][col] = field[srcRow][col];
    }
}


/**
 * Moves all the rows starting from parameter "row"+1 to 0 by one row lower
 * @param row starting Y position
 */
void field_drop(int row)
{
    for(; row >= 0; row--)
    {
        if(row-1 >= 0)
        {
            row_copy(row, row-1);
            row_clear(row-1);
        }
        else
            row_clear(row);
    }
}


/**
 * Check all the rows (except bottom boundary) whether are full and drops the field above them
 */
void field_clear_full_rows()
{
    int row;

    for(row = 7; row >= 0; row--)
    {
        if (row_is_full(row))
        {
            field_drop(row);
            row++; // Check this row again because it changed
        }
    }
}


/**
 * Moves current shape one row lower or sets new current shape and clears full rows in field
 */
void shape_down()
{
    if(!shape_move(1, 0))
    {
        shape_static();
        field_clear_full_rows();
        shape_spawn();
    }
}


/**
 * Lights columns in the LED matrix for one specified row
 * @param led_row currently lit row
 */
void led_light_col(int led_row)
{
    P3DIR &= ~(BIT2);
    P2DIR = 0;

    if(led_matrix[led_row][0])
        P2DIR |= BIT0;

    if(led_matrix[led_row][1])
        P2DIR |= BIT1;

    if(led_matrix[led_row][2])
        P3DIR |= BIT2;

    if(led_matrix[led_row][3])
        P2DIR |= BIT3;

    if(led_matrix[led_row][4])
        P2DIR |= BIT4;

    if(led_matrix[led_row][5])
        P2DIR |= BIT5;

    if(led_matrix[led_row][6])
        P2DIR |= BIT6;

    if(led_matrix[led_row][7])
        P2DIR |= BIT7;
}


/**
 * Lights one specified row in the LED matrix
 * @param led_row row to be lit
 */
void led_light_row(int led_row)
{   // lights whole row
    switch(led_row)
    {
        case 0:
            P6OUT = ~BIT0;
            break;
        case 1:
            P6OUT = ~BIT1;
            break;
        case 2:
            P6OUT = ~BIT2;
            break;
        case 3:
            P6OUT = ~BIT3;
            break;
        case 4:
            P6OUT = ~BIT4;
            break;
        case 5:
            P6OUT = ~BIT5;
            break;
        case 6:
            P6OUT = ~BIT6;
            break;
        case 7:
            P6OUT = ~BIT7;
            break;
    }

    led_light_col(led_row);
}




/*******************************************************************************
 * Vypis uzivatelske napovedy (funkce se vola pri vykonavani prikazu "help")
*******************************************************************************/
void print_user_help(void)
{
    term_send_crlf();
    term_send_str("Tetris on 8x8 matrix by xfurda00");
    term_send_crlf();
    term_send_str("Controls:");
    term_send_crlf();
    term_send_str("key 4 to move left");
    term_send_crlf();
    term_send_str("key 6 to move right");
    term_send_crlf();
    term_send_str("key 5 to move down");
    term_send_crlf();
    term_send_str("key 2 to rotate");
}


/*******************************************************************************
 * Obsluha klavesnice
*******************************************************************************/
int keyboard_idle()
{
    if(gameOver)
        return 0;

    char ch;
    ch = key_decode(read_word_keyboard_4x4());
    if (ch != last_ch)
    {
        last_ch = ch;

        switch(ch)
        {
            case 0:
                break;

            case '2':
                shape_rotate();
                break;

            case '5':
                shape_down();
                break;
            case '4':
                shape_move(0, -1);
                break;
            case '6':
                shape_move(0, 1);
                break;
        }

        field_changed();
    }
    return 0;
}



/*******************************************************************************
 * Dekodovani a vykonani uzivatelskych prikazu
*******************************************************************************/
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
    return CMD_UNKNOWN;
}

/*******************************************************************************
 * Inicializace periferii/komponent po naprogramovani FPGA
*******************************************************************************/
void fpga_initialized()
{
    LCD_init();
    LCD_clear();
    LCD_append_string("Tetris");
}


/*******************************************************************************
 * Hlavni funkce
*******************************************************************************/
int main(void)
{
    // Setup hardware
    initialize_hardware();
    keyboard_init();
    P6DIR = 255; // 11111111

    // Setup software
    last_ch = 0;
    generate_next_shape();
    shape_spawn();
    field_changed();

    // Main cycle
    int led_row = 0;
    while (1)
    {
        delay_ms(1);    // Wait

        led_light_row(led_row); // Light up one row
        led_row = (led_row + 1) % 8;    // Determine next row to be lit

        keyboard_idle();                   // obsluha klavesnice
        terminal_idle();                   // obsluha terminalu
    }
}
